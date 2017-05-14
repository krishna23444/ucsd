/* 
 * mdb.h : part of the Mace toolkit for building distributed systems
 * 
 * Copyright (c) 2006, Charles Killian, Dejan Kostic, Ryan Braud, James W. Anderson, John Fisher-Ogden, Calvin Hubble, Duy Nguyen, Justin Burke, David Oppenheimer, Amin Vahdat, Adolfo Rodriguez, Sooraj Bhat
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 * 
 *    * Redistributions of source code must retain the above copyright
 *      notice, this list of conditions and the following disclaimer.
 *    * Redistributions in binary form must reproduce the above copyright
 *      notice, this list of conditions and the following disclaimer in
 *      the documentation and/or other materials provided with the
 *      distribution.
 *    * Neither the names of Duke University nor The University of
 *      California, San Diego, nor the names of the authors or contributors
 *      may be used to endorse or promote products derived from
 *      this software without specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE
 * USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * 
 * ----END-OF-LEGAL-STUFF---- */
#ifndef _MACE_DB_H
#define _MACE_DB_H

#include <db_cxx.h>
#include <string>
#include <sstream>
#include <cassert>
#include "ScopedLock.h"
#include "massert.h"
#include "FileUtil.h"

namespace mace {


// Mace-friendly object oriented interface to Berkeley DB.  

// Assumes that Key and Data type are Serializable.

template<class Key, class Data, size_t MAX_DATA_SIZE = 4096, size_t MAX_KEY_SIZE = 4096>
class DB {
public:
  class const_iterator {
    friend class DB<Key, Data, MAX_DATA_SIZE, MAX_KEY_SIZE>;

  public:
    typedef std::pair<Key, Data> ipair;

    const ipair* operator->() const {
      ASSERT(c);
      return &cur;
    }

    const ipair& operator*() const {
      ASSERT(c);
      return cur;
    }

    const const_iterator& operator++() const {
      next();
      return *this;
    }

    const const_iterator& operator++(int _i) const {
      next();
      return *this;
    }

    bool operator==(const const_iterator& right) const {
      return c == right.c;
    }

    bool operator!=(const const_iterator& right) const {
      return c != right.c;
    }

    virtual ~const_iterator() {
      close();
    }

    const_iterator() : c(0), k(kbuf, MAX_KEY_SIZE), v(vbuf, MAX_DATA_SIZE) {
      setFlags();
    }

    const_iterator(const const_iterator& other) :
      c(0),
      k(kbuf, MAX_KEY_SIZE),
      v(vbuf, MAX_DATA_SIZE) {

      setFlags();
      copy(other);
    }

    const_iterator& operator=(const const_iterator& other) {
      copy(other);
      return *this;
    }

  private:
    void copy(const const_iterator& other) {
      if (other.c) {
	other.c->dup(&c, DB_POSITION);
      }

      if (c) {
	setCur(other.cur.first);
      }
    }

    const_iterator(Dbc* c) :
      c(c), k(kbuf, MAX_KEY_SIZE), v(vbuf, MAX_DATA_SIZE) {

      setFlags();
      next();
    } // const_iterator

    const_iterator(Dbc* c, const Key& sk, u_int32_t flags = DB_SET) :
      c(c), k(kbuf, MAX_KEY_SIZE), v(vbuf, MAX_DATA_SIZE) {

      ASSERT(c);
      setFlags();
      setCur(sk, flags);
    } // const_iterator

    void setFlags() const {
      k.set_ulen(MAX_KEY_SIZE);
      k.set_flags(DB_DBT_USERMEM);

      v.set_ulen(MAX_DATA_SIZE);
      v.set_flags(DB_DBT_USERMEM);
    } // setFlags

    void setCur(const Key& kt, u_int32_t flags = DB_SET) {
      std::string sk = serialize(&kt, kt);
      Dbt t((void*)sk.data(), sk.size());
      int r = c->get(&t, &v, flags);
      if (r == DB_NOTFOUND) {
	close();
      }
      else {
	ASSERT(r == 0);

	fillCur(t, v);
      }
    } // setCur

    void next() const {
      if (c) {
	int r = c->get(&k, &v, DB_NEXT);
	if (r == DB_NOTFOUND) {
	  close();
	  return;
	}
	ASSERT(r == 0);

	fillCur(k, v);
      }
    } // next

    void fillCur(const Dbt& kt, const Dbt& vt) const {
      std::string sk((const char*)kt.get_data(), kt.get_size());
      deserialize(sk, &cur.first, cur.first);
      std::string s((const char*)vt.get_data(), vt.get_size());
      deserialize(s, &cur.second, cur.second);
    } // fillCur

    void close() const {
      if (c) {
	c->close();
	c = 0;
      }
    } // close

  private:
    mutable Dbc* c;
    mutable Dbt k;
    mutable Dbt v;
    mutable ipair cur;
    char kbuf[MAX_KEY_SIZE];
    char vbuf[MAX_DATA_SIZE];
  }; // class const_iterator

public:
  DB(uint32_t cacheSize = 0,
     const std::string& errorPrefix = "mace::DB: ") :
    isOpen(false), privateEnv(true), dbenv(0), db(0) {

    dbenv = new DbEnv(0);

    init(cacheSize, true, errorPrefix);
  } // DB

  DB(DbEnv* env, uint32_t cacheSize = 0, bool setError = true,
     const std::string& errorPrefix = "mace::DB:" ) :
    isOpen(false), privateEnv(false), dbenv(env), db(0) {

    ASSERT(dbenv);
    init(cacheSize, setError, errorPrefix);
  } // DB

  virtual ~DB() {
    close();
    if (privateEnv) {
      delete dbenv;
    }
    dbenv = 0;
    delete db;
    db = 0;
  } // ~DB

  virtual void open(const std::string& dir, const std::string& file,
		    DBTYPE dbtype = DB_BTREE, bool create = false) {
    ScopedLock sl(lock);

    if (create && !FileUtil::fileExists(dir)) {
      FileUtil::mkdir(dir, 0755, true);
    }
    
    ASSERT(!isOpen);
    // want DB_INIT_TXN, DB_RECOVER flags?
    // need a way to enable DB_INIT_LOCK if necessary
    ASSERT(dbenv->open(dir.c_str(),
		       DB_CREATE | DB_INIT_LOG | DB_INIT_MPOOL, 0) == 0);

    _open(file, dbtype);
  } // open

  virtual void open(const std::string& file, DBTYPE dbtype = DB_BTREE) {
    ScopedLock sl(lock);

    ASSERT(!isOpen);
    ASSERT(!privateEnv);

    _open(file, dbtype);
  } // open

  virtual void close() {
    ScopedLock sl(lock);
    if (isOpen) {
      isOpen = false;
      db->close(0);
      if (privateEnv) {
	dbenv->close(0);
      }
    }
  } // close

  virtual bool get(const Key& kt, Data& d) const {
    ASSERT(isOpen);

    std::string sk = serialize(&kt, kt);
    Dbt k((void*)sk.data(), sk.size());

    char* buf = new char[MAX_DATA_SIZE];
    Dbt v(buf, MAX_DATA_SIZE);

    v.set_ulen(MAX_DATA_SIZE);
    v.set_flags(DB_DBT_USERMEM);

    int r = db->get(NULL, &k, &v, 0);
    if (r == DB_NOTFOUND) {
      return false;
    }
    ASSERT(r == 0);

    std::string s(buf, v.get_size());
    deserialize(s, &d, d);

    delete buf;

    return true;
  } // get

  virtual bool containsKey(const Key& kt) const {
    Data d;
    return get(kt, d);
  } // containsKey

  virtual void put(const Key& kt, const Data& d) {
    ASSERT(isOpen);

    std::string sk = serialize(&kt, kt);
    ASSERT(sk.size() <= MAX_KEY_SIZE);

    Dbt k((void*)sk.data(), sk.size());

    std::string s = serialize(&d, d);
    ASSERT(s.size() <= MAX_DATA_SIZE);

    Dbt v((void*)s.data(), s.size());
	
    ASSERT(db->put(NULL, &k, &v, 0) == 0);
  } // put

  virtual bool erase(const Key& kt) {
    ASSERT(isOpen);

    std::string sk = serialize(&kt, kt);

    Dbt k((void*)sk.data(), sk.size());
    int r = db->del(NULL, &k, 0);
    ASSERT(r == 0 || r == DB_NOTFOUND);
    return r == 0;
  } // erase

  const_iterator begin() const {
    Dbc* c;
    ASSERT(db->cursor(NULL, &c, 0) == 0);
    const_iterator r(c);
    return r;
  } // begin

  const_iterator end() const {
    return const_iterator(0);
  } // end

  const_iterator find(const Key& kt) const {
    Dbc* c;
    ASSERT(db->cursor(NULL, &c, 0) == 0);
    const_iterator r(c, kt);
    return r;
  } // find

  const_iterator lower_bound(const Key& kt) const {
    Dbc* c;
    ASSERT(db->cursor(NULL, &c, 0) == 0);
    const_iterator r(c, kt, DB_SET_RANGE);
    return r;
  } // lower_bound

protected: 
  void init(uint32_t cacheSize, bool setError, const std::string& errorPrefix) {
    pthread_mutex_init(&lock, 0);

    if (setError) {
      dbenv->set_error_stream(&std::cerr);
      dbenv->set_errpfx(errorPrefix.c_str());
    }

    if (cacheSize) {
      ASSERT(dbenv->set_cachesize(0, cacheSize, 1) == 0);
    }
  } // init
  
  virtual void _open(const std::string& file, DBTYPE dbtype) {
    db = new Db(dbenv, 0);
    db->set_error_stream(&std::cerr);

    // want DB_AUTO_COMMIT flag?
    ASSERT(db->open(NULL, file.c_str(), NULL, dbtype, DB_CREATE, 0644) == 0);

    isOpen = true;
  } // _open


private:
  bool isOpen;
  bool privateEnv;
  DbEnv *dbenv;
  Db *db;

  pthread_mutex_t lock;

}; // class DB

} // namespace mace

#endif // _MACE_DB_H
