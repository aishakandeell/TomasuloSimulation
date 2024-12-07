#include "Memory.h"
using namespace std;

// Constructor
Memory::Memory(int memSize, int numRegisters) {
    dataMemory.resize(memSize, 0);  // Initialize data memory with zeros
    registers.resize(numRegisters, 0); // Initialize registers with zeros
}

// Load instructions into instruction memory
void Memory::loadInstructions(const vector<string>& instructions) {
    instructionMemory = instructions;
}

// Get an instruction from instruction memory
string Memory::getInstruction(int address) const {
    if (address < 0 || address >= static_cast<int>(instructionMemory.size())) {
        cerr << "Instruction Memory Address Out of Bounds: " << address << "\n";
        return "";
    }
    return instructionMemory[address];
}

// Read data from data memory
int Memory::readData(int address) const {
    if (address < 0 || address >= static_cast<int>(dataMemory.size())) {
        cerr << "Data Memory Address Out of Bounds: " << address << "\n";
        return -1;
    }
    return dataMemory[address];
}

// Write data to data memory
void Memory::writeData(int address, int value) {
    if (address < 0 || address >= static_cast<int>(dataMemory.size())) {
        cerr << "Data Memory Address Out of Bounds: " << address << "\n";
        return;
    }
    dataMemory[address] = value;
}

// Read value from a register
int Memory::readRegister(int reg) const {
    if (reg < 0 || reg >= static_cast<int>(registers.size())) {
        cerr << "Register Out of Bounds: readregister " << reg << "\n";
        return -1;
    }
    return registers[reg];
}

// Write value to a register
void Memory::writeRegister(int reg, int value) {
    if (reg < 0 || reg >= static_cast<int>(registers.size())) {
        cerr << "Register Out of Bounds: writeregister" << reg << "\n";
        return;
    }
    registers[reg] = value;
}

// Print the state of data memory
void Memory::printDataMemory() const {
    cout << "Data Memory (non-zero addresses):\n";
    for (size_t i = 0; i < dataMemory.size(); ++i) {
        if (dataMemory[i] != 0) { 
            cout << "Address " << i << ": " << dataMemory[i] << "\n";
        }
    }
}

// Print the state of registers
void Memory::printRegisters() const {
    cout << "Registers:\n";
    for (size_t i = 0; i < registers.size(); ++i) {
        cout << "R" << i << ": " << registers[i] << "\n";
    }
}
