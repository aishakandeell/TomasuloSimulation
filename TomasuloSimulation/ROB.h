#pragma once
#include <queue>
#include "Instruction_Unit.h" 
#define ROB_H

using namespace std;

struct ROBEntry {
    Instruction_Unit* instruction; 
    int destination;             
    int value;                 
    bool ready;                 

    // Constructor
    ROBEntry(Instruction_Unit* instr = nullptr, int dest = -1, int val = 0, bool r = false)
        : instruction(instr), destination(dest), value(val), ready(r) {}
};


class ROB {
private:
    queue<ROBEntry> entries; // Queue for the ROB
    int maxEntries;               // pass size 6 

public:
    // Constructor
    ROB(int size = 6);

    // add an entry to the ROB
    bool addEntry(Instruction_Unit* instruction, int destination);

    // mark as ready
    bool markReady(int value);

    // commit if ready
    bool commit();

    // check if full
    bool isFull() const;

    // check if empty empty
    bool isEmpty() const;

    void print() const;
};

