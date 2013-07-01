/*
 * Packet.h
 *
 *  Created on: 02/04/2012
 *      Author: arliones
 */

#ifndef PACKET_H_
#define PACKET_H_

#include <set>

template<typename ID_Type>
class Packet
{
public:
    Packet() {}
    virtual ~Packet() {}

    void clear()
    {
        source = 0;
        neighbors.clear();
    }

    ID_Type source;
    std::set<ID_Type> neighbors;
};

#endif /* PACKET_H_ */
