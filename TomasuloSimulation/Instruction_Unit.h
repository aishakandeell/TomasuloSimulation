
#pragma once
#define INSTRUCTION_UNIT_H
using namespace std;

#include <string>
#include <vector>
#include <map>


enum InstructionState {
    issue,
    execute,
    write,
    commit
};
class Instruction_Unit
{
public:
    string opcode;
    vector<int> parsed;
    int issueCycle, execCycle, writeCycle, commitCycle; // timing of each state
    InstructionState state, newstate;      // Current state 

    Instruction_Unit(string line);   //constructor        
    void updateState(InstructionState newState, int clockCycle); // Update instruction state
    InstructionState getCurrentState() const;    // Return the current state of the instruction
    void print() const; //print instruction details
private:
    void parseInstruction(const std::string& line); //parse assmebly instruction
};

