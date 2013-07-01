/*
 * CommandParser.h
 *
 *  Created on: 05/04/2012
 *      Author: arliones
 */

#ifndef COMMANDPARSER_H_
#define COMMANDPARSER_H_

#include "CommandMonitor.h"
#include <string>
#include <map>

class CommandParser : public CommandMonitor::Observer
{
public:
    struct Method {
        virtual ~Method() {}
        virtual void operator()() = 0;
    };

    CommandParser();
    virtual ~CommandParser();

    virtual void notify(const std::string & __str);

    void register_command(const std::string & __cmd, Method * __handler);

private:
    typedef std::map<std::string, Method*> CmdMap_t;
    CmdMap_t commands;
};

#endif /* COMMANDPARSER_H_ */
