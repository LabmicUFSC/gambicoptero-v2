//============================================================================
// Name        : emote2-demo.cpp
// Author      : Arliones Hoeller Jr.
// Copyright   : Copyright by LISHA/UFSC.
//============================================================================

#include "SysParams.h"
#include "Serial.h"
#include "PacketParser.h"
#include "GraphPlotter.h"
#include "CommandMonitor.h"
#include "CommandParser.h"
#include "Dumper.h"

#include <iostream>

using namespace std;

typedef unsigned short ID_Type;


struct QuitCommand : public CommandParser::Method {
    QuitCommand(Serial* _s, CommandMonitor * _m, UbiGraphPlotter<ID_Type> * _u)
            : s(_s), m(_m), u(_u) {}
    virtual ~QuitCommand() {}
    void operator()() {
        (cout << "Quitting...").flush();
        s->stop();
        m->stop();
        u->stop();
    }
private:
    Serial * s;
    CommandMonitor * m;
    UbiGraphPlotter<ID_Type> * u;
};


int main(int argc, const char* argv[]) {

    SysParams sys_params;
    try
    {
        new (&sys_params) SysParams(argc,argv);
    }
    catch(std::exception & e)
    {
        cerr << "Error: " << e.what() << endl;
        return 1;
    }


    cout << "Starting EPOSMote Demo..." << endl;

    try
    {
        Serial serial(sys_params.serial);
        PacketParser<ID_Type> parser;
        Dumper dumper;
        UbiGraphPlotter<ID_Type> plotter(sys_params.ubi_server_path);
        CommandMonitor monitor;
        CommandParser cmd_parser;

        serial.attach_observer(&dumper);
        serial.attach_observer(&parser);
        parser.attach_observer(&plotter);
        QuitCommand quit(&serial, &monitor, &plotter);
        cmd_parser.register_command("quit", &quit);
        monitor.attach_observer(&cmd_parser);

        serial.start();
        monitor.start();
        plotter.start();
        serial.join();
        monitor.join();
        plotter.join();
    }
    catch(std::exception & e)
    {
        cerr << "Error: " << e.what() << endl;
        return 4;
    }

    cout << endl << endl << "Finished!" << endl;

    return 0;
}
