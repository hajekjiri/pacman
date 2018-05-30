/// Implementation of Setting
/**
 * @file Setting.cpp
 */

#include <ostream>
#include <sstream>
#include "MyException.h"
#include "Setting.h"

Setting::Setting( const std::string & key )
                : m_Value( key ) {
  // do nothing
}

Setting::~Setting() {
  // do nothing
}

const std::string & Setting::GetStr() const {
  return m_Value;
}

const int Setting::GetInt() const {
  return stoi( m_Value );
}

const bool Setting::GetBool() const {
  if ( m_Value == "yes" ||
       m_Value == "y" ||
       m_Value == "1" ) {
    return true;
  }

  if ( m_Value == "no" ||
       m_Value == "n" ||
       m_Value == "0" ) {
    return false;
  }

  std::ostringstream oss;
  oss << "Could not convert value '" << m_Value << "' to bool";
  throw MyException( oss.str() );
}
