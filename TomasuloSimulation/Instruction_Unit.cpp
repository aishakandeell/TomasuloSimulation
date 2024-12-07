#include "Instruction_Unit.h"
#include <sstream>
#include <iostream>
#include <cstdlib>
using namespace std;

//extracts instructions, gets current stage of the instruction and the clock cycle

Instruction_Unit::Instruction_Unit(string line)//constructor takes instruction in assembly
    : issueCycle(-1), execCycle(-1), writeCycle(-1), commitCycle(-1), state(issue) {
     
    newstate = issue;
    parseInstruction(line);
}

void Instruction_Unit::parseInstruction(const string& line) {
    string templine = line; // Copy the instruction
    string opcode, destReg, srcReg1, srcReg2;

    // Extract opcode
    size_t spacePos = templine.find(' '); // Instructions must be properly spaced
    opcode = templine.substr(0, spacePos); // Extract the opcode (e.g., LOAD, STORE, BEQ, etc.)
    templine.erase(0, spacePos != string::npos ? spacePos + 1 : templine.size()); // Remove opcode from templine

    // handle different instruction formats based on the opcode
    if (opcode == "LOAD") {
        // LOAD rA, offset(rB) or STORE rA, offset(rB)

        // Extract destination register (rA)
        size_t commaPos = templine.find(',');
        destReg = templine.substr(0, commaPos);
        templine.erase(0, commaPos != string::npos ? commaPos + 2 : templine.size());

        // Extract offset and source register (offset(rB))
        size_t openingBracketPos = templine.find('(');
        size_t closingBracketPos = templine.find(')');
        srcReg1 = templine.substr(openingBracketPos + 1, closingBracketPos - openingBracketPos - 1); // Extract rB
        srcReg2 = templine.substr(0, openingBracketPos); // Extract offset
        templine.erase(0, closingBracketPos != string::npos ? closingBracketPos + 1 : templine.size());

        // Parsed representation for LOAD
        parsed = { 1, destReg[1] - '0', atoi(srcReg2.c_str()), srcReg1[1] - '0' };
    }
    else if (opcode == "STORE") {
        // STORE rA, offset(rB)
        // Extract destination register (rA)
        size_t commaPos = templine.find(',');
        destReg = templine.substr(0, commaPos);
        templine.erase(0, commaPos != string::npos ? commaPos + 2 : templine.size());

        // Extract offset and source register (offset(rB))
        size_t openingBracketPos = templine.find('(');
        size_t closingBracketPos = templine.find(')');
        srcReg1 = templine.substr(openingBracketPos + 1, closingBracketPos - openingBracketPos - 1); // Extract rB
        srcReg2 = templine.substr(0, openingBracketPos); // Extract offset
        templine.erase(0, closingBracketPos != string::npos ? closingBracketPos + 1 : templine.size());

        // Parsed representation for STORE
        parsed = { 2, destReg[1] - '0', atoi(srcReg2.c_str()), srcReg1[1] - '0' };
    }
    else if (opcode == "BEQ") {
        // BEQ rA, rB, offset
        // Extract first source register (rA)
        size_t commaPos = templine.find(',');
        destReg = templine.substr(0, commaPos);
        templine.erase(0, commaPos != string::npos ? commaPos + 2 : templine.size());

        // Extract second source register (rB)
        commaPos = templine.find(',');
        srcReg1 = templine.substr(0, commaPos);
        templine.erase(0, commaPos != string::npos ? commaPos + 2 : templine.size());

        // Extract offset
        srcReg2 = templine;
        parsed = { 3, destReg[1] - '0', srcReg1[1] - '0', atoi(srcReg2.c_str()) / 2 };

    }
    else if (opcode == "CALL") {
        // CALL label

        // Extract label (7-bit signed constant)
        srcReg1 = templine; // The label is the only remaining part of the instruction
        parsed = { 4, 0, 0, atoi(srcReg1.c_str()) };
    }
    else if (opcode == "RET") {
        parsed = { 5, 0, 0, 0 };
    }
    else if (opcode == "ADD") {
        // ADD rA, rB, rC 

        // Extract destination register (rA)
        size_t commaPos = templine.find(',');
        destReg = templine.substr(0, commaPos);
        templine.erase(0, commaPos != string::npos ? commaPos + 2 : templine.size());

        // Extract first source register (rB)
        commaPos = templine.find(',');
        srcReg1 = templine.substr(0, commaPos);
        templine.erase(0, commaPos != string::npos ? commaPos + 2 : templine.size());

        // Extract second source register (rC)
        srcReg2 = templine;

        // Parsed representation for ADD
        parsed = { 6, destReg[1] - '0', srcReg1[1] - '0', srcReg2[1] - '0' };
    }
    else if (opcode == "ADDI") {
        // ADDI rA, rB, imm

        // Extract destination register (rA)
        size_t commaPos = templine.find(',');
        destReg = templine.substr(0, commaPos);
        templine.erase(0, commaPos != string::npos ? commaPos + 2 : templine.size());

        // Extract first source register (rB)
        commaPos = templine.find(',');
        srcReg1 = templine.substr(0, commaPos);
        templine.erase(0, commaPos != string::npos ? commaPos + 2 : templine.size());

        // Extract immediate value (imm)
        srcReg2 = templine;
        parsed = { 7, destReg[1] - '0', srcReg1[1] - '0', atoi(srcReg2.c_str()) };
    }
    else if (opcode == "NAND") {
        // NAND rA, rB, rC

        // Extract destination register (rA)
        size_t commaPos = templine.find(',');
        destReg = templine.substr(0, commaPos);
        templine.erase(0, commaPos != string::npos ? commaPos + 2 : templine.size());

        // Extract first source register (rB)
        commaPos = templine.find(',');
        srcReg1 = templine.substr(0, commaPos);
        templine.erase(0, commaPos != string::npos ? commaPos + 2 : templine.size());

        // Extract second source register (rC)
        srcReg2 = templine;

        // Parsed representation for NAND
        parsed = { 8, destReg[1] - '0', srcReg1[1] - '0', srcReg2[1] - '0' };

    }
    else if (opcode == "MUL") {
        // MUL rA, rB, rC

        // Extract destination register (rA)
        size_t commaPos = templine.find(',');
        destReg = templine.substr(0, commaPos);
        templine.erase(0, commaPos != string::npos ? commaPos + 2 : templine.size());

        // Extract first source register (rB)
        commaPos = templine.find(',');
        srcReg1 = templine.substr(0, commaPos);
        templine.erase(0, commaPos != string::npos ? commaPos + 2 : templine.size());

        // Extract second source register (rC)
        srcReg2 = templine;

        // Parsed representation for MUL
        parsed = { 9, destReg[1] - '0', srcReg1[1] - '0', srcReg2[1] - '0' };
    }
    else {
        cerr << "Invalid instruction format: " << opcode << endl;
    }
}

void Instruction_Unit::updateState(InstructionState newState, int clockCycle) {
    // Update the current state

    state = newState;
    cout << "state=" << state << "\n";
    newstate = newState;
    cout << "state=" << newstate << "\n";
    // Log the timing for the corresponding state
    switch (newState) {
    case issue:
        issueCycle = clockCycle;
        break;
    case execute:
        execCycle = clockCycle;
        break;
    case write:
        writeCycle = clockCycle;
        break;
    case commit:
        commitCycle = clockCycle;
        break;
    }
}

InstructionState Instruction_Unit::getCurrentState() const {
    return state; // Return the current state of the instruction
}

void Instruction_Unit::print() const {
    // Print instruction details
    cout << "Instruction: " << opcode << " Parsed: {";
    for (int i : parsed) {
        std::cout << i << " ";
    }
    cout << "}\n";

    cout << "State: " << state
        << " | Issue: " << issueCycle
        << ", Exec: " << execCycle
        << ", Write: " << writeCycle
        << ", Commit: " << commitCycle << "\n";
}

