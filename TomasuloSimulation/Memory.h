#pragma once
#ifndef MEMORY_H
#define MEMORY_H

#include <vector>
#include <string>
#include <iostream>
using namespace std;

class Memory {
private:
    vector<int> dataMemory;         // Data memory (word-addressable)
    vector<int> registers;          // Registers
    vector<string> instructionMemory; // Instruction memory

public:
    // Constructor
    Memory(int memSize, int numRegisters);

    // Load instructions into instruction memory
    void loadInstructions(const vector<string>& instructions);

    // Get an instruction from instruction memory
    string getInstruction(int address) const;

    // Read data from data memory
    int readData(int address) const;

    // Write data to data memory
    void writeData(int address, int value);

    // Read value from a register
    int readRegister(int reg) const;

    // Write value to a register
    void writeRegister(int reg, int value);

    // Print the state of data memory
    void printDataMemory() const;

    // Print the state of registers
    void printRegisters() const;
};

#endif
