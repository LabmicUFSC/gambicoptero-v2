// main.cpp
//
// Created by Daniel Schwartz-Narbonne on 13/04/07.
// Modified by Christian Bienia
//
// Copyright 2007-2008 Princeton University
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions
// are met:
// 1. Redistributions of source code must retain the above copyright
//    notice, this list of conditions and the following disclaimer.
// 2. Redistributions in binary form must reproduce the above copyright
//    notice, this list of conditions and the following disclaimer in the
//    documentation and/or other materials provided with the distribution.
//
// THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
// ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
// FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
// DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
// OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
// HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
// LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
// OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
// SUCH DAMAGE.

// Modified to EPOS by Giovani Gracioli <giovani@lisha.ufsc.br>

#include <utility/ostream.h>
#include <utility/string.h>
#include "canneal/include/epos_wrappers.h"
#include "canneal/include/netlist.h"
#include "canneal/include/annealer_thread.h"

System::OStream cout;

int entry_pt(void*);

int argc = 5; // TODO verificar
char * argv[5]; //TODO verificar

const int num_threads = 5;
const int swaps_per_temp = 5;
const int start_temp = 100;
const int number_temp_steps = 1;

int main(void)
{
    cout << "PARSEC Benchmark Suite ported to EPOS" << endl;
    
    srandom(3);
    
    /*if(argc != 5 && argc != 6) {
        cout << "Usage: " << argv[0] << " NTHREADS NSWAPS TEMP NETLIST [NSTEPS]" << endl;
        exit(1);
    } */  
    
        //argument 1 is numthreads
    //int num_threads = atoi(argv[1]);
    cout << "Threadcount: " << num_threads << endl;
#ifndef ENABLE_THREADS
    if (num_threads != 1){
        cout << "NTHREADS must be 1 (serial version)" <<endl;
        exit(1);
    }
#endif

    //argument 2 is the num moves / temp
    //int swaps_per_temp = atoi(argv[2]);
    cout << swaps_per_temp << " swaps per temperature step" << endl;

    //argument 3 is the start temp
    //int start_temp =  atoi(argv[3]);
    cout << "start temperature: " << start_temp << endl;
    
    //argument 4 is the netlist filename
    //string filename(argv[4]);
    char filename[10];
    //strcpy(filename, argv[4]);
    //cout << "netlist filename: " << filename << endl;
    
    //argument 5 (optional) is the number of temperature steps before termination
    //int number_temp_steps = -1;
    //if(argc == 6) {
        //number_temp_steps = atoi(argv[5]);
        cout << "number of temperature steps: " << number_temp_steps << endl;
   // }
    
    //now that we've read in the commandline, run the program
    netlist my_netlist(filename);
    
    annealer_thread a_thread(&my_netlist,num_threads,swaps_per_temp,start_temp,number_temp_steps);
    
#ifdef ENABLE_THREADS
    //vector<pthread_t> threads(num_threads);
    pthread_t *threads[num_threads];
    void* thread_in = static_cast<void*>(&a_thread);
    for(int i=0; i<num_threads; i++){
        //pthread_create(&threads[i], NULL, entry_pt,thread_in);
        threads[i] = new System::Thread(entry_pt, thread_in);
    }
    for (int i=0; i<num_threads; i++){
        //cout << "main pthread_join() this=(" << (void *) System::Thread::self() << ")\n";
        //System::Thread *t = &threads[i];
        //t->join();
        int status = threads[i]->join();
        //pthread_join(threads[i], NULL);
    }
#else
    a_thread.Run();
#endif
    cout << "Final routing is: " << (unsigned long) my_netlist.total_routing_cost() << endl; //TODO print double and float
    
    return 0;
}

int entry_pt(void* data)
{
    annealer_thread* ptr = static_cast<annealer_thread*>(data);
    ptr->Run();
    return 0;
}
