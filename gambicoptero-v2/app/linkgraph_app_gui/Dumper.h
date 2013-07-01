/*
 * Dumper.h
 *
 *  Created on: 09/04/2012
 *      Author: arliones
 */

#ifndef DUMPER_H_
#define DUMPER_H_

#include <iostream>
#include <sstream>
#include "Serial.h"

class Dumper : public Serial::Observer
{
public:
    virtual ~Dumper() {}
    virtual void notify(const unsigned char & c){
//        (std::cout << c).flush();
        strm << c;
        if(c == '\n')
        {
            cout << strm.str();
            strm.str("");
        }
    }
private:
    std::stringstream strm;
};

#endif /* DUMPER_H_ */
