#include "ROB.h"
#include <iostream>
using namespace std;

ROB::ROB(int size) : maxEntries(size) {}

bool ROB::addEntry(Instruction_Unit* instruction, int destination) {
    if (isFull()) {
        cerr << "ROB is full.\n";
        return false;
    }

    // Create a new ROB entry and push it to the queue
    ROBEntry entry(instruction, destination, 0, false);
    entries.push(entry);
    return true;
}

bool ROB::markReady(int value) {
    if (isEmpty()) {
        cerr << "ROB is empty.\n";
        return false;
    }

   
    ROBEntry& frontEntry = const_cast<ROBEntry&>(entries.front());

  
    frontEntry.value = value;
    frontEntry.ready = true;
    return true;
}

bool ROB::commit() {
    if (isEmpty()) {
        cerr << "ROB is empty.\n";
        return false;
    }

    const ROBEntry& frontEntry = entries.front();
    if (!frontEntry.ready) {
        cerr << "not ready\n";
        return false;
    }

    // Commit the instruction 
    cout << "Committing instruction: " << frontEntry.instruction->opcode
        << " to destination " << frontEntry.destination
        << " with value " << frontEntry.value << "\n";

    // remove the committed entry from the queue
    entries.pop();
    return true;
}

bool ROB::isFull() const {
  
    return entries.size() >= maxEntries;
}

bool ROB::isEmpty() const {
   
    return entries.empty();
}

void ROB::print() const {
    cout << "ROB State:\n";
    queue<ROBEntry> tempQueue = entries; // Create a copy of the queue to traverse
    int index = 0;

    while (!tempQueue.empty()) {
        const ROBEntry& entry = tempQueue.front();
        cout << "  [Index: " << index++ << "] Instruction: "
            << (entry.instruction ? entry.instruction->opcode : "N/A")
            << ", Dest: " << entry.destination
            << ", Value: " << entry.value
            << ", Ready: " << (entry.ready ? "Yes" : "No") << "\n";

        tempQueue.pop(); // Remove the front element
    }
}
