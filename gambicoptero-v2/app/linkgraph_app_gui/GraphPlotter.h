/*
 * GraphPlotter.h
 *
 *  Created on: 02/04/2012
 *      Author: arliones
 */

#ifndef GRAPHPLOTTER_H_
#define GRAPHPLOTTER_H_

#include "AbstractSubject.h"
#include "Packet.h"
#include "exceptions.h"


extern "C" {
    #include <UbigraphAPI.h>
};
#include <map>
#include <set>
#include <iostream>
#include <sstream>
#include <boost/thread.hpp>
#include <boost/thread/mutex.hpp>

#include <unistd.h>
#include <errno.h>
#include <signal.h>
#include <time.h>

using namespace std;

template<typename ID_Type>
class GraphPlotter : public AbstractSubject<Packet<ID_Type> >::Observer
{
public:
    virtual ~GraphPlotter() {}
    virtual void notify(const Packet<ID_Type> & __pkt) = 0;
};

#define make_edge_id(origin,destination) ((origin*1000000)+destination)

template<typename ID_Type>
class UbiGraphPlotter : public GraphPlotter<ID_Type>
{
public:
    UbiGraphPlotter(string ubi_server_path, int __update_period = 5, time_t __timeout = 25) throw()
    : pid(0), update_period(__update_period), timeout(__timeout)
    {
        pid = fork();

        stringstream err;
        if(pid == 0)
        {
            // start UbiGraph server
//            cout << "Starting UbiGraph Server" << endl;
            execl((ubi_server_path+"/ubigraph_server").c_str(), "ubigraph_server", (char*)0);
            err.str("");
            err << "Error: failed to start UbiGraph server (errno " << errno << ")";
            throw OSExecption(err.str().c_str());
        }

        if(pid < 0)
        {
            err.str("");
            err << "Error: failed to start UbiGraph server." << endl;
            throw OSExecption(err.str().c_str());
        }

        sleep(3);

        ubigraph_clear();
    }

    virtual ~UbiGraphPlotter() {
        kill(pid, SIGKILL);
        sleep(1);
    }

    virtual void notify(const Packet<ID_Type> & __pkt) {
        MutexGuard guard(mutex);
        nodes[__pkt.source] = Node();
        nodes[__pkt.source].neighbors.insert(__pkt.neighbors.begin(), __pkt.neighbors.end());
        nodes[__pkt.source].age = time(0);

        typename set<ID_Type>::iterator sit;
        for(sit = __pkt.neighbors.begin(); sit != __pkt.neighbors.end(); ++sit)
            nodes[*sit].age = time(0);
        changed = true;
    }

    void run() {
        running = true;
        while(running) {
            if(changed) redraw();
            sleep(update_period);
        }
    }

    void start() {
        callable x;
        x.plotter = this;
        thread = boost::thread(x);
    }
    void stop()  { running = false; }
    void join() {
        thread.join();
    }

private:
    void create_node(ID_Type id) {
//        cout << "GraphPlotter::create_node(" << id << ")" << endl;

        // skip rendering if data is old
        time_t now = time(0);
        if((now - nodes[id].age) >= timeout) return;

        ubigraph_new_vertex_w_id(id);
        stringstream label;
        label << "ID: " << id;
        ubigraph_set_vertex_attribute(id, "label", label.str().c_str());
        ubigraph_set_vertex_attribute(id, "size", "1.0");
    }

    void create_edge(ID_Type src, ID_Type dst) {
//        cout << "GraphPlotter::create_edge(" << src << "," << dst << ")" << endl;
        ubigraph_new_edge_w_id(make_edge_id(src,dst), src, dst);
        ubigraph_set_edge_attribute(make_edge_id(src,dst), "width", "3.0");
        ubigraph_set_edge_attribute(make_edge_id(src,dst), "color", "#FFFFFF");
        ubigraph_set_edge_attribute(make_edge_id(src,dst), "strength", "1.0");
    }

    struct MutexGuard {
        MutexGuard(boost::mutex & _m) : m(_m) { m.lock(); }
        ~MutexGuard() { m.unlock(); }
    private:
        boost::mutex & m;
    };
    void redraw() {
        changed = false;
        ubigraph_clear();

        typename NodesMap_t::iterator mit;
        typename set<ID_Type>::iterator sit;

        MutexGuard guard(mutex);

        for(mit = nodes.begin(); mit != nodes.end(); ++mit)
        {
            create_node(mit->first);
            for(sit = mit->second.neighbors.begin(); sit != mit->second.neighbors.end(); ++sit)
            {
                create_node(*sit);
                create_edge(mit->first, *sit);
            }
        }
    }

    struct Node {
        set<ID_Type> neighbors;
        time_t age;
    };
    typedef map<ID_Type, Node > NodesMap_t;
    mutable NodesMap_t nodes;
    volatile bool changed;
    int pid;

    struct callable {
        void operator()() { plotter->run(); }
        UbiGraphPlotter * plotter;
    };
    boost::thread thread;
    boost::mutex mutex;
    volatile bool running;
    int update_period;
    time_t timeout;
};

#endif /* GRAPHPLOTTER_H_ */
