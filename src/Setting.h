/// Setting class
/**
 * @file Setting.h
 * class representing a setting - boolean/string/int
 */

#ifndef SETTING_H
#define SETTING_H

#include <ostream>
#include <string>

 class Setting {
  public:
    /// Constructor
    Setting( const std::string & key );

    /// Destructor
    ~Setting();

    /// Get string value
    const std::string & GetStr() const;

    /// Get integer value
    const int GetInt() const;

    /// Get boolean value
    const bool GetBool() const;

  private:
    std::string m_Value;
 };

 #endif // SETTING_H