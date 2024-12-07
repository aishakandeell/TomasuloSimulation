#pragma once
#define REGISTER_FILE_H
#include <vector>
#include <iostream>
#include "ROB.h"
using namespace std;

class Register_File {
private:
    struct Register {
        int value;    // value of the register
        int robIndex; // ROB index producing this register's value 

        Register() : value(0), robIndex(-1) {}
    };

    vector<Register> registers; // Vector to hold register values

public:
    // Constructor to initialize the register file with a given size
    Register_File(int size = 16);

    // Associate a register with a ROB entry
    void setDependency(int regIndex, int robIndex);

    // Method to write a value to a register and clear its ROB dependency
    void writeRegister(int regIndex, int value);

    // Free registers based on committed ROB entries
    void freeRegistersFromROB(ROB& rob);

    // Check if a register is ready (no ROB dependency)
    bool isReady(int regIndex) const;

    // Method to read a register's value
    int readRegister(int regIndex) const;

    // Debug: Print the state of the register file
    void print() const;
};

