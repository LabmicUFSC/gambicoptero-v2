/*
 * SysParams.h
 *
 *  Created on: 02/04/2012
 *      Author: arliones
 */

#ifndef SYSPARAMS_H_
#define SYSPARAMS_H_

#include <string>
#include "exceptions.h"

class SysParams {
public:
    SysParams() {}
    SysParams(int argc, const char* argv[]) throw() {
        if(argc != 5)
            throw BadParameterExecption("Wrong quantity of parameters!");
        if(std::string(argv[1]) != "--serial")
            throw BadParameterExecption("No serial port provided.");
        this->serial = argv[2];
        if(std::string(argv[3]) != "--ubi_path")
            throw BadParameterExecption("No path to Ubi Server provided.");
        this->ubi_server_path = argv[4];
    }

    std::string serial;
    std::string ubi_server_path;
};

#endif /* SYSPARAMS_H_ */
