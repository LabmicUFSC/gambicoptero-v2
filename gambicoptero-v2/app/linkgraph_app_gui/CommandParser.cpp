/*
 * CommandParser.cpp
 *
 *  Created on: 05/04/2012
 *      Author: arliones
 */

#include "CommandParser.h"

using namespace std;

CommandParser::CommandParser() {}

CommandParser::~CommandParser() {}

void CommandParser::notify(const string & __str) {
    if(commands.find(__str) == commands.end())
        cerr << "Unknown command." << endl;
    else
        (*commands[__str])();
}

void CommandParser::register_command(const std::string & __cmd, Method * __handler) {
    commands.insert(make_pair(__cmd, __handler));
}
