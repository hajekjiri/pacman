/// Implementation of Setting
/**
 * @file Setting.cpp
 */

#include <algorithm>
#include <sstream>
#include "MyException.h"
#include "Setting.h"

void Setting::Validate( const std::string & key,
                        const std::string & value ) {
  /*
   * go through all possible setting types and
   *  check if conversion to desired value is possible
   *  ( if not, the converting method will throw an exception )
   */

  if ( key == "map" ) {
    // do nothing
    return;
  }

  if ( key == "username" ) {
    // do nothing
    return;
  }

  if ( key == "max_turns_classic" ) {
    Setting( value ).GetIntConst();
    return;
  }

  if ( key == "max_turns_survival" ) {
    Setting( value ).GetIntConst();
    return;
  }

  if ( key == "bonus_interval" ) {
    Setting( value ).GetIntConst();
    return;
  }

  if ( key == "bonus_duration" ) {
    Setting( value ).GetIntConst();
    return;
  }

  if ( key == "ghost_aggressive_range" ) {
    Setting( value ).GetIntConst();
    return;
  }

  if ( key == "ghosts_portals_allowed" ) {
    Setting( value ).GetBoolConst();
    return;
  }


  std::ostringstream oss;
  oss << "Invalid cfg file - unknown setting '" << key << "'";
  throw MyException( oss.str() );
}

Setting::Setting( const std::string & key )
                : m_Value( key ) {
  // do nothing
}

Setting::~Setting() {
  // do nothing
}

const std::string & Setting::GetStrConst() const {
  return m_Value;
}

const int Setting::GetIntConst() const {
  int res;
  try {
    res = stoi( m_Value );
  } catch ( std::exception & e ) {
    std::ostringstream oss;
    oss << "Invalid cfg file - could not convert '" << m_Value << "' to integer";
    throw MyException( oss.str() );
  }
  return res;
}

const bool Setting::GetBoolConst() const {
  std::string value = m_Value;
  // transform string to lowercase
  std::transform( value.begin(),
                  value.end(),
                  value.begin(),
                  []( const unsigned char & c ) { return std::tolower( c ); } );
  if ( value == "yes" ||
       value == "y" ||
       value == "1" ) {
    return true;
  }

  if ( value == "no" ||
       value == "n" ||
       value == "0" ) {
    return false;
  }

  std::ostringstream oss;
  oss << "Invalid cfg file - could not convert value '"
      << m_Value << "' to bool\nUse yes, y, or 1 for 'yes' / no, n, or 0 for 'no' ( not case sensitive )";
  throw MyException( oss.str() );
}
