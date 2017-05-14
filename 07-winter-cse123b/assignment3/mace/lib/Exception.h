/* 
 * Exception.h : part of the Mace toolkit for building distributed systems
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
#include <string>
#include <map>
#include "Printable.h"

#ifndef EXCEPTION_H
#define EXCEPTION_H

class Exception : public std::exception, public mace::Printable {
public:
  Exception() { }
  virtual ~Exception() throw() { }
  Exception(const std::string& m) : message(m) { }
  virtual const char* what() const throw() { return message.c_str(); }
  virtual void print(std::ostream& o) const { o << message; }
  virtual void printState(std::ostream& o) const { print(o); }
  virtual std::string toString() const { return message; }
  virtual std::string toStateString() const { return toString(); }
  virtual void rethrow() const { throw *this; }

protected:
  std::string message;

}; // Exception

class CollectionException : public Exception {
public:
  CollectionException(const std::string& m) : Exception(m) { }
  void rethrow() const { throw *this; }
}; // CollectionException

class NotFoundException : public CollectionException {
public:
  NotFoundException(const std::string& m) : CollectionException(m) { }
  void rethrow() const { throw *this; }
}; // NotFoundException

class IOException : public Exception {
public:
  IOException(const std::string& m) : Exception(m) { }
  void rethrow() const { throw *this; }
}; // IOException

class FileException : public IOException {
public:
  FileException(const std::string& m) : IOException(m) { }
  void rethrow() const { throw *this; }
}; // FileException

class BadFileTypeException : public FileException {
public:
  BadFileTypeException(const std::string& m) : FileException(m) { }
  void rethrow() const { throw *this; }
}; // BadFileTypeException

class FileNotFoundException : public FileException {
public:  
  FileNotFoundException(const std::string& m) : FileException(m) { }
  void rethrow() const { throw *this; }
}; // FileNotFoundException

class InvalidPathException : public FileException {
public:
  InvalidPathException(const std::string& m) : FileException(m) { }
  void rethrow() const { throw *this; }
}; // InvalidPathException

class PermissionAccessException : public FileException {
public:
  PermissionAccessException(const std::string& m) : FileException(m) { }
  void rethrow() const { throw *this; }
}; // PermissionAccessException

class LinkLoopException : public FileException {
public:
  LinkLoopException(const std::string& m) : FileException(m) { }
  void rethrow() const { throw *this; }
}; // LinkLoopException

class ReadException : public IOException {
public:
  ReadException(const std::string& m) : IOException(m) { }
  void rethrow() const { throw *this; }
}; // ReadException

class WriteException : public IOException {
public:
  WriteException(const std::string& m) : IOException(m) { }
  void rethrow() const { throw *this; }
}; // WriteException

class PipeClosedException : public WriteException {
public:
  PipeClosedException(const std::string& m) : WriteException(m) { }
  void rethrow() const { throw *this; }
}; // PipeClosedException

class IncomparableException : public Exception {
public:
  IncomparableException(const std::string& m) : Exception(m) { }
  void rethrow() const { throw *this; }
}; // IncomparableException

class AddressException : public Exception {
public:
  AddressException(const std::string& m) : Exception(m) { }
  void rethrow() const { throw *this; }
}; // AddressException

class UnreachablePrivateAddressException : public AddressException {
public:
  UnreachablePrivateAddressException(const std::string& m) : AddressException(m) { }
  void rethrow() const { throw *this; }
}; // UnreachablePrivateAddressException

#endif // EXCEPTION_H

