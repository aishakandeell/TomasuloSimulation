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
        registerFile.writeRegister(i, i );  
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
        if (!rs.isBusy() && rob.addEntry(&  instr, instr.parsed[1])) {
            rs.setBusy(true);//set busy in reservation station
            //rs.op = static_cast<Operation>(instr.parsed[0]); // Assign operation

            

            // source register 1
            if (registerFile.isReady(instr.parsed[2])) {
                rs.Vj = registerFile.readRegister(instr.parsed[2]);
                rs.Qj = -1;
            }
            else {
                rs.Qj = registerFile.readRegister(instr.parsed[2]);
            }
            //source register 2
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
            rs.setOperands(rs.Vj, rs.Vk);


            cout << "Issued in cycle " << instr.issueCycle<<"\n";
            return;
        }
    }

    cout << "Issue Stage: No free reservation station or ROB entry available.\n";
}

int getExecutionCycles(int operation) {
    switch (operation) {
    case 1:
        return 6;  
    case 2:
        return 6;  
    case 3:
        return 1;  
    case 4:
        return 2; 
    case 5:
        return 2;  
    case 6:
        return 1;  
    case 7:
        return 8;  
   
    }
}

// Execute Stage
void executeStage() {
    for (auto& instr : pipelineQueue) {
        if (instr.getCurrentState() == issue) {
            int regIndex1 = instr.parsed[2];  // Source register 1 (rA or rB)
            int regIndex2 = instr.parsed[3];  // Source register 2 (rB or rC)
            cout << "regindex2: " << regIndex2 << "\n";
            // Check if both operands are ready in the register file
            if (registerFile.isReady(regIndex1) && registerFile.isReady(regIndex2)) {
                cout << "op: " << instr.parsed[0] << "\n";
                // Perform execution based on the opcode (instr.parsed[0])
                switch (instr.parsed[0]) {
                case 1:  // LOAD instruction
                {
                    int offset = instr.parsed[3];  
                    int address = registerFile.readRegister(regIndex2) + offset;
                    int value = memory.readData(address);  // Load the value from memory
                    // Mark ROB entry as ready and store the value
                    rob.markReady(value);
                    break;
                }
                break;

                case 2:  // STORE instruction
                {
                    int offset = instr.parsed[2]; 
                    int address = registerFile.readRegister(regIndex2) + offset;
                    int value = registerFile.readRegister(regIndex1);  // Read value from rA
                    memory.writeData(address, value);  // Write value to memory
                    // Mark ROB entry as ready, no value to store in the register file
                    rob.markReady(value);
                    break;
                }
                break;

                case 3: {  // Branch if equal instruction
                    // Compare rA and rB, if equal branch to PC + 1 + offset
                    int offset = instr.parsed[3];  
                    int valueA = registerFile.readRegister(regIndex1);
                    int valueB = registerFile.readRegister(regIndex2);
                    //if (valueA == valueB) {
                      //  programCounter += offset;  // Branch
                    //}
                    //else {
                      //  programCounter++;  // Normal increment if branch is not taken
                    //}
                    // Mark ROB entry as ready, as the comparison is done
                    //rob.markReady(0);  // No specific value needed for branch operation
                    break;
                }

                case 6:  // ADD instruction
                {
                    int valueB = registerFile.readRegister(regIndex2);
                    int valueC = registerFile.readRegister(regIndex1);  
                    int result = valueB + valueC;
                    cout << " result is :" << result << "\n";
                    registerFile.writeRegister(instr.parsed[1], result);
                    // Mark ROB entry as ready with the result
                    rob.markReady(result);
                    break;
                }
                break;

                case 7:  // ADD immediate instruction
                {
                    int valueB = registerFile.readRegister(regIndex2);
                    int immediate = instr.parsed[3];  
                    int result = valueB + immediate;
                    registerFile.writeRegister(instr.parsed[1], result);
                    // Mark ROB entry as ready with the result
                    rob.markReady(result);
                    break;
                }
                break;

                case 8:  // NAND instruction
                {
                    int valueB = registerFile.readRegister(regIndex2);
                    int valueC = registerFile.readRegister(regIndex1); 
                    int result = ~(valueB & valueC);
                    // Mark ROB entry as ready with the result
                    rob.markReady(result);
                    break;
                }
                break;

                case 9:  // MUL instruction
                {
                    int valueB = registerFile.readRegister(regIndex2);
                    int valueC = registerFile.readRegister(regIndex1);  // Assuming parsed[3] is rC
                    int result = valueB * valueC;
                    // Mark ROB entry as ready with the least significant 16 bits of the result
                    rob.markReady(result & 0xFFFF);  // 16-bit result (assuming we only need lower 16 bits)
                    break;
                }
                break;

                default:
                    cerr << "Unknown operation type: " << instr.parsed[0] << endl;
                    break;
                }

                instr.execCycle = clockCycle + getExecutionCycles(instr.parsed[0]);  
                cout << "execcycle: " << instr.execCycle << "\n";
                if (clockCycle >= instr.execCycle) {
                    cout << "instruction state is execute now";
                    instr.updateState(execute, clockCycle);  // Move instruction to execute state after the required cycles
                    cout << "Executed instruction: " << instr.parsed[0] << " in cycle " << instr.execCycle << endl;
                }

                
            }
            else {
                // If operands are not ready (dependency exists), the instruction can't execute yet
                cout << "Operands for instruction not ready yet. Waiting...\n";
            }
        }
    }
}

// Write-Back Stage
void writeBackStage() {
    for (auto& instr : pipelineQueue) {
        cout << "state" << instr.getCurrentState() << "\n";
        if (instr.getCurrentState() == 2) {//
            for (auto& rs : reservationStations) {
                if (rs.isBusy() && rs.isReady()) {
                    rob.markReady(rs.Vj); // Write result to ROB
                    registerFile.writeRegister(rs.Qi, rs.Vj); // Write to destination register
                    rs.reset(); // Free reservation station
                    for (auto& instr : pipelineQueue) {
                        if (instr.getCurrentState() == execute) {
                            instr.writeCycle = clockCycle; 
                        }
                    }
                    cout << "Write-Back: Result " << rs.Vj << " written.\n";
                }
            }
            instr.updateState(write, clockCycle);
        }
    }
}


// Commit Stage
void commitStage() {
    for (auto& instr : pipelineQueue) {
        if (instr.getCurrentState() == execute) {
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
}}}

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

    cout << "Choose an option:\n";
    cout << "1. Run predefined test case\n";
    cout << "2. Enter your own assembly instructions\n";
    int choice;
    cin >> choice;
    cin.ignore(); // To ignore any extra newline left by the previous input

    vector<string> program;

    if (choice == 1) {
        // Predefined test instructions
        program = { "ADD R1, R0, R2",  // NOP (no operation)
                   "ADD R1, R3, R2",  // Load from memory
                   "LOAD R3, 0(R4)",  // Add
                   //"STORE R3, 8(R2)", // Store to memory
                   "NAND R5, R6, R7", // NAND operation
                   "MUL R8, R1, R2" };  // Multiply
    }
    else if (choice == 2) {
        // User input instructions
        cout << "Enter your assembly instructions (enter 'done' to finish):\n";
        string input;
        while (true) {
            cout << "Enter instruction: ";
            getline(cin, input);
            if (input == "done") {
                break;
            }
            program.push_back(input);
        }
    }
    else {
        cout << "Invalid option chosen. Exiting...\n";
        return 1;
    }

    // Load instructions into the instruction queue
    for (const auto& instr : program) {
        instructionQueue.emplace(instr); // Add instructions to queue
    }

    
    
    // Run simulation
    runSimulation();

    return 0;
}
