/// MyException class
/**
 * @file MyException.h
 */

#ifndef MYEXCEPTION_H
#define MYEXCEPTION_H

#include <exception>
#include <string>

/// Custom exception
class MyException : public std::exception {
  public:
    /**
     * @brief Constructor
     * @param m Message
     */
    MyException( const std::string & m );

    /// Destructor
    ~MyException();

    /**
     * @brief Function telling us what happened
     * @return Error message
     */
    const char * what() const throw();

  private:
    /// Error message
    std::string m_ErrStr;
};

#endif // MYEXCEPTION_H
