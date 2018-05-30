/// Implementation of Setting
/**
 * @file Setting.cpp
 */

#include <algorithm>
#include <cctype>
#include <ostream>
#include <sstream>
#include <string>
#include "MyException.h"
#include "Setting.h"

Setting::Setting( const std::string & key )
                : m_Value( key ) {
  // do nothing
}

Setting::~Setting() {
  // do nothing
}

void Setting::Validate( const std::string & key,
                        const std::string & value ) {
  if ( key == "map" ) {
    // do nothing
    return;
  }

  if ( key == "username" ) {
    // do nothing
    return;
  }

  if ( key == "max_turns_classic" ) {
    Setting( value ).GetInt();
    return;
  }

  if ( key == "max_turns_survival" ) {
    Setting( value ).GetInt();
    return;
  }

  if ( key == "bonus_interval" ) {
    Setting( value ).GetInt();
    return;
  }

  if ( key == "bonus_duration" ) {
    Setting( value ).GetInt();
    return;
  }

  if ( key == "ghost_aggressive_range" ) {
    Setting( value ).GetInt();
    return;
  }

  if ( key == "ghosts_portals_allowed" ) {
    Setting( value ).GetBool();
    return;
  }

  std::ostringstream oss;
  oss << "Invalid cfg file - unknown setting '" << key << "'";
  throw MyException( oss.str().data() );
}



const std::string & Setting::GetStr() const {
  return m_Value;
}

const int Setting::GetInt() const {
  int res;
  try {
    res = stoi( m_Value );
  } catch ( std::exception & e ) {
    std::ostringstream oss;
    oss << "Invalid cfg file - could not convert '" << m_Value << "' to integer";
    throw MyException( oss.str().data() );
  }
  return res;
}

const bool Setting::GetBool() const {
  std::string value = m_Value;
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
      << m_Value << "' to bool\nUse yes, y, or 1 for 'yes' / no, n, or 0 for 'no' ( case insensitive )";
  throw MyException( oss.str() );
}
