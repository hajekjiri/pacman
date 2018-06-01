/// Setting class
/**
 * @file Setting.h
 */

#ifndef SETTING_H
#define SETTING_H

#include <string>

/// Setting class with 3 setting interpretations => std::string, int, bool
class Setting {
  public:
    /**
     * @brief Check if given setting combination is valid, throw exception if not
     * @param key Type/key
     * @param String vlue
     */
    static void Validate( const std::string & key,
                          const std::string & value );

    /**
     * @brief Constructor
     * @param key Type/key
     */
    Setting( const std::string & key );

    /// Destructor
    ~Setting();

    /// Get string value
    const std::string & GetStrConst() const;

    /// Get integer value
    const int GetIntConst() const;

    /// Get boolean value
    const bool GetBoolConst() const;

  private:
    /// Raw value as std::string
    std::string m_Value;
 };

 #endif // SETTING_H
