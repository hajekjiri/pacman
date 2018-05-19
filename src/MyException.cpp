/// Implementation of MyException
/**
 * @file MyException.cpp
 */

#include "MyException.h"

MyException::MyException( const std::string & m )
                        : m_ErrStr( m ) {
  // do nothing
}

MyException::~MyException() {
  // do nothing
}

const char * MyException::what() const throw() {
  return m_ErrStr.data();
}
