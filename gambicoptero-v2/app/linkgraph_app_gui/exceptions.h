/*
 * exceptions.h
 *
 *  Created on: 04/04/2012
 *      Author: arliones
 */

#ifndef EXCEPTIONS_H_
#define EXCEPTIONS_H_

#include <exception>

class ExplainableException : public std::exception
{
public:
    ExplainableException(const char * __what) throw() : _what(__what) {}
    ~ExplainableException() throw() {}
    virtual const char* what() const throw()
    {
        return _what.c_str();
    }
private:
    std::string _what;
};

class BadParameterExecption : public ExplainableException
{
public:
    BadParameterExecption(const char * __what) throw() : ExplainableException(__what) {}
    ~BadParameterExecption() throw() {}
    virtual const char* what() const throw()
    {
        return (std::string("BadParameterExecption (") + ExplainableException::what() + ")").c_str();
    }
};

class OSExecption : public ExplainableException
{
public:
    OSExecption(const char * __what) throw() : ExplainableException(__what) {}
    ~OSExecption() throw() {}
    virtual const char* what() const throw()
    {
        return (std::string("OSExecption (") + ExplainableException::what() + ")").c_str();
    }
};


#endif /* EXCEPTIONS_H_ */
