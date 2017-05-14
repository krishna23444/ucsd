#include "Printable.h"
#include "m_map.h"

namespace mace {

template<typename K, typename D, typename V,
	 typename Compare = std::less<K> >
class VersionedMap : public PrintPrintable {
public:
  class Element {
  public:
    Element() : contained(false) { }
    Element(const D& d, const V& v) : value(d), version(v), contained(true) { }

  public:
    D value;
    V version;
    bool contained;
  };

  typedef mace::map<K, Element, Compare> DataMap;
  typedef typename DataMap::const_iterator const_iterator;

  virtual ~VersionedMap() {
  }

  virtual const Element& get(const K& k) const {
    static Element n;
    const_iterator i = m.find(k);
    if (i == m.end()) {
      return n;
    }
    return i->second;
  }

  virtual void put(const K& k, const D& d, const V& v) {
    m[k] = Element(d, v);
  }

  virtual void remove(const K& k) {
    m.erase(k);
  }

  void print(std::ostream& printer) const {
  }

protected:
  DataMap m;
}; // VersionedMap

} // namespace mace
