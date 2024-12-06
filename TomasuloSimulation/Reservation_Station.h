#pragma once
#define RESERVATION_STATION_H

#include <string>

enum Operation {
    ADD,ADDI, SUB, MUL, DIV, NAND, LOAD, STORE, BEQ, CALL, RET
};

// Define the Reservation Station class
class Reservation_Station {
public:
    // Properties
    Operation op;          // operation based on opcode
    int Vj, Vk;            // source operands
    int Qj, Qk;            // reservation stations that produce operands 
    int A;                 // Address information for ld/sw
    bool busy;             // busy flag for reservation station
    int Qi;                // indicates which reservation station will provide the value for the register if applicable

    // Constructor
    Reservation_Station(Operation operation);

    // Methods to update and access reservation station state
    void setOperands(int vj, int vk);
    void setOperandSources(int qj, int qk);
    void setAddress(int address);
    void setBusy(bool isBusy);
    void reset();

    // Methods to check if the station is ready
    bool isReady() const;

    // Getters
    bool isBusy() const;
    int getAddress() const;
    int getVj() const;
    int getVk() const;
    int getQj() const;
    int getQk() const;
    int getQi() const;
};



