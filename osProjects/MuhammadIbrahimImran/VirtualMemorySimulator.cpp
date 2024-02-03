#include "VirtualMemorySimulator.hpp"

VirtualMemorySimulator::VirtualMemorySimulator(const string traceFile, int pageFrameCount) {
    this->traceFile = traceFile;
    this->pageFrameCount = pageFrameCount;
}

void VirtualMemorySimulator::handleMemoryTrace(bool debugMode, int policy, int primarySegmentSize, int secondarySegmentSize) {
    ifstream file(traceFile);
    if (!file.is_open()) {
        std::cerr << "Error: Unable to open trace file: " << traceFile << std::endl;
        return;
    }

    string currentLine;
    while (getline(file, currentLine)) {
        stringstream strStream(currentLine);
        unsigned int pageNumber;
        char accessType;
        strStream >> hex >> pageNumber >> accessType;
        pageNumber >>= 12;
        traceCount++;

        switch (policy) {
            case FIFO:
                policyFIFO(pageNumber, accessType, debugMode);
                break;
            case LRU:
                policyLRU(pageNumber, accessType, debugMode);
                break;
            case SEGMENTED_FIFO:
                if (primarySegmentSize == 0) 
                    policyFIFO(pageNumber, accessType, debugMode);
                else if (secondarySegmentSize == 0)
                    policyLRU(pageNumber, accessType, debugMode);
                break;
            default:
                break;
        }        
    }
}

// |-------------------------- FIFO Policy Functions --------------------------|

void VirtualMemorySimulator::policyFIFO(int pageNumber, char accessType, bool debugMode) {
    if (pageTable.find(pageNumber) != pageTable.end()) {
        policyFIFO_HIT(pageNumber, accessType);
        if (debugMode) debugPolicy(HIT, pageNumber);
    } else {
        policyFIFO_MISS(pageNumber, accessType);
        if (debugMode) debugPolicy(MISS, pageNumber);
    }
}

void VirtualMemorySimulator::policyFIFO_HIT(int pageNumber, char accessType) {
    if (accessType == 'W') pageTable[pageNumber] = DIRTY;
}

void VirtualMemorySimulator::policyFIFO_MISS(int pageNumber, char accessType) {
    if (pageTable.size() < pageFrameCount) {
        accessType == 'W' ? pageTable[pageNumber] = DIRTY : pageTable[pageNumber] = !DIRTY;
        pageFrameDeque.push_back(pageNumber);
    } else {
        policyFIFO_REPLACE(pageNumber, accessType);
    }
    diskReads++;
}

void VirtualMemorySimulator::policyFIFO_REPLACE(int pageNumber, char accessType) {
    int pageToRemove = pageFrameDeque.front();
    pageFrameDeque.pop_front();
    pageFrameDeque.push_back(pageNumber);

    if (pageTable[pageToRemove] == DIRTY) {
        diskWrites++;
    }

    pageTable.erase(pageToRemove);
    accessType == 'W' ? pageTable[pageNumber] = DIRTY : pageTable[pageNumber] = !DIRTY;
}

// |--------------------------- LRU Policy Functions --------------------------|

void VirtualMemorySimulator::policyLRU(int pageNumber, char accessType, bool debugMode) {
    if (pageTable.find(pageNumber) != pageTable.end()) {
        policyLRU_HIT(pageNumber, accessType);
        if (debugMode) debugPolicy(HIT, pageNumber);
    } else {
        policyLRU_MISS(pageNumber, accessType);
        if (debugMode) debugPolicy(MISS, pageNumber);
    }
}

void VirtualMemorySimulator::policyLRU_HIT(int pageNumber, char accessType) {
    deque<int>::iterator it = find(pageFrameDeque.begin(), pageFrameDeque.end(), pageNumber);
    pageFrameDeque.erase(it);
    pageFrameDeque.push_back(pageNumber);
    if (accessType == 'W') pageTable[pageNumber] = DIRTY;
}

void VirtualMemorySimulator::policyLRU_MISS(int pageNumber, char accessType) {
    if (pageTable.size() < pageFrameCount) {
        accessType == 'W' ? pageTable[pageNumber] = DIRTY : pageTable[pageNumber] = !DIRTY;
        pageFrameDeque.push_back(pageNumber);
    } else {
        policyLRU_REPLACE(pageNumber, accessType);
    }
    diskReads++;
}

void VirtualMemorySimulator::policyLRU_REPLACE(int pageNumber, char accessType) {
   int pageToRemove = pageFrameDeque.front();
    pageFrameDeque.pop_front();
    pageFrameDeque.push_back(pageNumber);

    if (pageTable[pageToRemove] == DIRTY) {
        diskWrites++;
    }

    pageTable.erase(pageToRemove);
    accessType == 'W' ? pageTable[pageNumber] = DIRTY : pageTable[pageNumber] = !DIRTY;
}

// |--------------------------- Segmentated FIFO Policy Functions --------------------------|


// |--------------------------- Debug Functions -------------------------------|

void VirtualMemorySimulator::debugPolicy(bool pageFault, int pageNumber) {
    string fault = pageFault ? "\033[31m" : "\033[32m";
    string colorCode = (pageTable.size() >= pageFrameCount) ? "\033[31m" : "\033[32m";
    cout << "Size: " << colorCode << pageTable.size() << "\033[0m | Page: " << fault << pageNumber
    << "\033[0m | Trace: " << traceCount << endl;
}

void VirtualMemorySimulator::printStats() {
    cout << "Disk Reads: " << diskReads << endl;
    cout << "Disk Writes: " << diskWrites << endl;
    cout << "Total Traces: " << traceCount << endl;
}

void VirtualMemorySimulator::outputFile(string fileName) {
    std::ofstream outputFile(fileName, std::ios::app);
    
    if (!outputFile) {
        std::cerr << "Error opening output file." << std::endl;
        return;
    }
    outputFile << "Disk Reads: " << diskReads << std::endl;
    outputFile << "Disk Writes: " << diskWrites << std::endl;
    outputFile << "Total Traces: " << traceCount << std::endl;
    outputFile << "----------------" << std::endl;
    outputFile.close();
}