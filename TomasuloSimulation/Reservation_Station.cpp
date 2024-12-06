//2 load units, 1 store unit, 1 beq unit, 1 call / ret unit, 4 add / addi units, 2 nand units and 1 mul unit
//should be initialized in simulation
#include "Reservation_Station.h"
#include <iostream>

Reservation_Station::Reservation_Station(Operation operation)
    : op(operation), Vj(0), Vk(0), Qj(0), Qk(0), A(0), busy(false), Qi(0) {}

void Reservation_Station::setOperands(int vj, int vk) {
    Vj = vj;
    Vk = vk;
}

void Reservation_Station::setOperandSources(int qj, int qk) {
    Qj = qj;
    Qk = qk;
}

void Reservation_Station::setAddress(int address) {
    A = address;
}

void Reservation_Station::setBusy(bool isBusy) {
    busy = isBusy;
}

void Reservation_Station::reset() {
    Vj = Vk = 0;
    Qj = Qk = 0;
    A = 0;
    busy = false;
    Qi = 0;
}

bool Reservation_Station::isReady() const {
    // A station is ready if both Qj and Qk are 0 (no pending instructions to write to the registers)
    return Qj == 0 && Qk == 0;
}

// Getter methods
bool Reservation_Station::isBusy() const {
    return busy;
}

int Reservation_Station::getAddress() const {
    return A;
}

int Reservation_Station::getVj() const {
    return Vj;
}

int Reservation_Station::getVk() const {
    return Vk;
}

int Reservation_Station::getQj() const {
    return Qj;
}

int Reservation_Station::getQk() const {
    return Qk;
}

int Reservation_Station::getQi() const {
    return Qi;
}
