/// MyException class
/**
 * @file MyException.cpp
 * Custom exception class
 */

#ifndef MY_EXCEPTION_H
#define MY_EXCEPTION_H

#include <exception>
#include <string>

class MyException : public std::exception {
  public:
    MyException( const std::string & m );

    ~MyException();

    const char * what() const throw();

  private:
    std::string m_ErrStr;
};

#endif // MY_EXCEPTION_H
