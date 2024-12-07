#include "Instruction_Unit.h"
#include "Register_File.h"
#include "Reservation_Station.h"
#include "ROB.h"
#include "Memory.h"
#include <iostream>
#include <vector>
#include <queue>
#include <deque>
#include <iomanip> // For formatting output

using namespace std;

// Global components
vector<Reservation_Station> reservationStations; // Reservation stations
ROB rob(6);                                     // ROB with 6 entries
Memory memory(128, 8);                          // Memory: 128 words, 8 registers
Register_File registerFile(9);                  // Register file with 16 registers
queue<Instruction_Unit> instructionQueue;       // Instruction queue
deque<Instruction_Unit> pipelineQueue;          // Instruction queue through pipeline
int clockCycle = 0;                             // Current clock cycle

int instructionsCommitted = 0; // Count of instructions committed


void initializeRegisters(Register_File& registerFile) {
    for (int i = 0; i < 8; ++i) {
        registerFile.writeRegister(i, i + 1);  // R0 = 1, R1 = 2, ..., R7 = 8
    }
    cout << "Registers initialized.\n";
}

// Initialize reservation stations
void initializeReservationStations() {
    reservationStations.push_back(Reservation_Station(LOAD));
    reservationStations.push_back(Reservation_Station(LOAD));
    reservationStations.push_back(Reservation_Station(STORE));
    reservationStations.push_back(Reservation_Station(BEQ));
    reservationStations.push_back(Reservation_Station(CALL));
    reservationStations.push_back(Reservation_Station(ADD));
    reservationStations.push_back(Reservation_Station(ADD));
    reservationStations.push_back(Reservation_Station(ADDI));
    reservationStations.push_back(Reservation_Station(ADDI));
    reservationStations.push_back(Reservation_Station(NAND));
    reservationStations.push_back(Reservation_Station(NAND));
    reservationStations.push_back(Reservation_Station(MUL));
}

// Issue Stage
void issueStage() {
    if (instructionQueue.empty()) return;

    Instruction_Unit& instr = instructionQueue.front();

    for (auto& rs : reservationStations) {
        if (!rs.isBusy() && rob.addEntry(&instr, instr.parsed[1])) {
            rs.setBusy(true);
            rs.op = static_cast<Operation>(instr.parsed[0]); // Assign operation

            // Handle operands from registers or ROB
            if (registerFile.isReady(instr.parsed[2])) {
                rs.Vj = registerFile.readRegister(instr.parsed[2]);
                rs.Qj = -1;
            }
            else {
                rs.Qj = registerFile.readRegister(instr.parsed[2]);
            }

            if (registerFile.isReady(instr.parsed[3])) {
                rs.Vk = registerFile.readRegister(instr.parsed[3]);
                rs.Qk = -1;
            }
            else {
                rs.Qk = registerFile.readRegister(instr.parsed[3]);
            }

            // Update dependency for destination
            registerFile.setDependency(instr.parsed[1], rob.getLatestIndex());

            instr.issueCycle = clockCycle; // Update issue cycle
            pipelineQueue.push_back(instr); // Add instruction to pipeline queue
            instructionQueue.pop(); // Remove from the queue
            instr.updateState(issue, clockCycle); // Update instruction state
            cout << "Issued: " << instr.opcode << "\n";
            return;
        }
    }

    cout << "Issue Stage: No free reservation station or ROB entry available.\n";
}

// Execute Stage
void executeStage() {
    for (auto& rs : reservationStations) {
        if (rs.isBusy() && rs.isReady()) {
            // Simulate execution (for ADD)
            rs.Vj += rs.Vk; // Example for ADD operation
            for (auto& instr : pipelineQueue) {
                if (instr.getCurrentState() == issue) {
                    instr.execCycle = clockCycle; // Record execution cycle
                }
            }
            cout << "Executed: " << rs.op << "\n";
        }
    }
}

// Write-Back Stage
void writeBackStage() {
    for (auto& rs : reservationStations) {
        if (rs.isBusy() && rs.isReady()) {
            rob.markReady(rs.Vj); // Write result to ROB
            registerFile.writeRegister(rs.Qi, rs.Vj); // Write to destination register
            rs.reset(); // Free reservation station
            for (auto& instr : pipelineQueue) {
                if (instr.getCurrentState() == execute) {
                    instr.writeCycle = clockCycle; // Record write cycle
                }
            }
            cout << "Write-Back: Result " << rs.Vj << " written.\n";
        }
    }
}

// Commit Stage
void commitStage() {
    if (!rob.isEmpty()) {
        if (rob.commit()) {
            instructionsCommitted++; // Increment the commit counter
            registerFile.freeRegistersFromROB(rob); // Clear register dependencies
            for (auto& instr : pipelineQueue) {
                if (instr.getCurrentState() == write) {
                    instr.commitCycle = clockCycle; // Record commit cycle
                }
            }
            cout << "Commit Stage: Committed instruction from ROB.\n";
        }
    }
}

// Print current state
void printState() {
    cout << "\n--- Cycle: " << clockCycle << " ---\n";
    rob.print();
    registerFile.print();
    memory.printDataMemory();
    cout << "-----------------------\n";
}

// Print summary of results
void printResults() {
    // Calculate IPC (Instructions per Cycle)
    double IPC = (instructionsCommitted > 0) ? static_cast<double>(instructionsCommitted) / clockCycle : 0;

    

    cout << "\n--- Simulation Results ---\n";
    cout << "Instructions Committed: " << instructionsCommitted << "\n";
    cout << "Total Execution Time: " << clockCycle << " cycles\n";
    cout << "IPC (Instructions per Cycle): " << fixed << setprecision(2) << IPC << "\n";
}
const int MAX_CYCLES = 15;
// Run simulation
void runSimulation() {
    while (!instructionQueue.empty() || !rob.isEmpty()) {
        clockCycle++;
        cout << "================== Clock Cycle " << clockCycle << " ==================\n";
        issueStage();
        executeStage();
        writeBackStage();
        commitStage();
        printState();
        if (clockCycle > MAX_CYCLES) {
            cerr << "Exiting after " << clockCycle << " cycles.\n";
            break;
        }
    }

    printResults(); // Print the results at the end of the simulation
}

int main() {
    
    initializeRegisters(registerFile);
    // Initialize reservation stations
    initializeReservationStations();

    // Test instructions
    vector<string> program = {
        "LOAD R1, 0(R2)",  // Load from memory
        "ADD R3, R1, R4",  // Add
        "STORE R3, 8(R2)", // Store to memory
        "NAND R5, R6, R7", // NAND operation
        "MUL R8, R1, R2"   // Multiply
    };

    // Load instructions into the instruction queue
    for (const auto& instr : program) {
        instructionQueue.emplace(instr); // Add instructions to queue
    }

    // Run simulation
    runSimulation();

    return 0;
}
