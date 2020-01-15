/* 
 * File:   main.cpp
 * Author: kt & om
 *
 * Created on April 18, 2019, 11:03 AM
 */

#include <cstdlib>
#include <iostream>
#include <fstream>
#include <sstream>
#include "BitMapAllocator.h"

using namespace std;

/**
 * TorF - Parses a boolean and returns a string
 * @param Status - boolean value to turn to string
 * @return -T or F(for True or False respectively)
 */
string TorF(bool status) {
    if (status == true) {
        return "T";
    } else {
        return "F";
    }
}

int main(int argc, char** argv) {
    fstream file;
    file.open(argv[1], ios_base::in);
    if (!file.is_open()) {
        cerr << "ERROR: failed to open file: " << argv[1] << "\n";
        exit(2);
    }
    //Reads first line
    string line;
    getline(file,line);
    stringstream firstLine;
    firstLine << hex << line;
    uint32_t frame_count;
    firstLine >> frame_count;
    cout << "+" << hex << frame_count << "\n";
    //Create BitMapAllocator
    BitMapAllocator mem(frame_count);
    vector< vector<uint32_t> > processes(4);

    //Process lines
    while (std::getline(file, line)) {
        stringstream stream(line);
        //Operation to do ("F","G",or "B")
        string op;
       
        //Process number to allocate/deallocate
        uint32_t process;
        //Number of page frames
        uint32_t count;
        //Parse line
        stream >> op;
        stream >> hex >> process;
        stream >> hex >> count;

        //Allocates here
        if (op.find("G") != string::npos) {
            cout << "+" << line << "\n";
            bool status = mem.GetFrames(count, processes.at(process));
            cout << " " << TorF(status) << " " << hex << mem.get_free_count() << "\n";
        } else if (op.find("F") != string::npos) {
            cout << "+" << line << "\n";
            bool status = mem.FreeFrames(count, processes.at(process));
            cout << " " << TorF(status) << " " << hex << mem.get_free_count() << "\n";
        } else {
            cout << "+B\n";
            cout << mem.get_bit_map_string() << "\n";
        }

    }

    return 0;
}




