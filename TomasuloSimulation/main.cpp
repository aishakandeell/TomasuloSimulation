#include "Reservation_Station.h"
#include <vector>
#include <iostream>
using namespace std;
// Create the reservation stations
vector<Reservation_Station> reservationStations;

void initializeReservationStations() {
    // Add 2 load units
    reservationStations.push_back(Reservation_Station(LOAD));
    reservationStations.push_back(Reservation_Station(LOAD));

    // Add 1 store unit
    reservationStations.push_back(Reservation_Station(STORE));

    // Add 1 BEQ unit
    reservationStations.push_back(Reservation_Station(BEQ));

    // Add 1 CALL/RET unit
    reservationStations.push_back(Reservation_Station(CALL));

    // Add 4 ADD/ADDI units
    reservationStations.push_back(Reservation_Station(ADD));
    reservationStations.push_back(Reservation_Station(ADD));
    reservationStations.push_back(Reservation_Station(ADDI));
    reservationStations.push_back(Reservation_Station(ADDI));

    // Add 2 NAND units
    reservationStations.push_back(Reservation_Station(NAND));
    reservationStations.push_back(Reservation_Station(NAND));

    // Add 1 MUL unit
    reservationStations.push_back(Reservation_Station(MUL));
}

int main() {
    

    return 0;
}
