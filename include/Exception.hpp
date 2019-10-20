#ifndef EXCEPTION_H
#define EXCEPTION_H

#include <exception>
#include <string>

class AgendaException: public std::exception {
private:
    std::string msg;
public:
    AgendaException(std::string msg);
    virtual const char *what() const throw ();
};

#endif