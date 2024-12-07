#include "Register_File.h"
#include <stdexcept>
using namespace std;

Register_File::Register_File(int size) : registers(size) {}

void Register_File::setDependency(int regIndex, int robIndex) {
    if (regIndex >= registers.size()) {
        cerr << "Index out of range: set dependency";
    }
    registers[regIndex].robIndex = robIndex;
}

void Register_File::writeRegister(int regIndex, int value) {
    if (regIndex >= registers.size()) {
        cerr << "Index out of range: write register";
    }
    registers[regIndex].value = value;
    registers[regIndex].robIndex = -1; // clear dependency
}
void Register_File::freeRegistersFromROB( ROB& rob) {
    if (rob.isEmpty()) {
        cerr << "ROB is empty, no instructions to commit.\n";
        return;
    }

    ROBEntry frontEntry = rob.entries.front(); 

    int destination = frontEntry.destination;

    if (destination >= 0 && destination < registers.size()) {
        registers[destination].robIndex = -1; 
    }

    rob.commit();  

    cout << "Committed instruction to register " << destination << "\n";
}

bool Register_File::isReady(int regIndex) const {
    if (regIndex < 0 || regIndex >= registers.size()) {
        cerr << "Index out of range: register file is ready for index " << regIndex << endl;
        return false; // Return false if the index is out of range
    }
    return registers[regIndex].robIndex == -1;
}

int Register_File::readRegister(int regIndex) const {
    if (regIndex >= registers.size()) {
        cerr << "Index out of range: read register" << regIndex << endl;
    }
    if (registers[regIndex].robIndex != -1) {
        cerr << "Value not ready(ROB dependency)";
    }
    return registers[regIndex].value;
}


void Register_File::print() const {
    for (size_t i = 0; i < registers.size(); ++i) {
        std::cout << "R" << i << ": Value=" << registers[i].value
            << ", ROB Index=" << registers[i].robIndex << "\n";
    }
}
