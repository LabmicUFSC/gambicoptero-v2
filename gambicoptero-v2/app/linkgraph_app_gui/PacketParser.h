/*
 * PacketParser.h
 *
 *  Created on: 02/04/2012
 *      Author: arliones
 */

#ifndef PACKETPARSER_H_
#define PACKETPARSER_H_

#include "AbstractSubject.h"
#include "Packet.h"

#include <sstream>
#include <iostream>

using namespace std;

template<typename ID_Type>
class PacketParser : public AbstractSubject<unsigned char>::Observer,
                     public AbstractSubject<Packet<ID_Type> >
{
public:
    PacketParser() {}
    virtual ~PacketParser() {}

    virtual void notify(const unsigned char & __c) {
//        cout << "PacketParser::notify(" << __c << ")\n";

        // parse packets and notify observers

        static int state = 0;
        static stringstream buffer;

        switch(state) {
        case 0:
            if(__c == '!') state++;
            break;
        case 1:
            if(__c == '|')
            {
                _pkt.source = atoi(buffer.str().c_str());
//                cout << "\tSource: " << _pkt.source << "\n";
                state++;
                buffer.str("");
            }
            else
                buffer << __c;
            break;
        case 2:
            if(__c == '|')
            {
//                int len = atoi(buffer.str().c_str());
//                cout << "\tLength: " << len << "\n";
                state++;
                buffer.str("");
            }
            else
                buffer << __c;
            break;
        case 3:
            if(__c == '!')
            {
                do_notify();
                state = 0;
                notify('!');
                return;
            }
            else if(__c == '\n')
            {
                do_notify();
                state = 0;
                return;
            }
            else if(__c == '|')
            {
                int neighbor = atoi(buffer.str().c_str());
                _pkt.neighbors.insert(neighbor);
//                cout << "\tNeighbor: " << neighbor << "\n";
                buffer.str("");
            }
            else
                buffer << __c;

            break;
        }
    }

private:
    void do_notify() {
        notify_all(_pkt);
        _pkt.clear();
    }

    Packet<ID_Type> _pkt;
};

#endif /* PACKETPARSER_H_ */
