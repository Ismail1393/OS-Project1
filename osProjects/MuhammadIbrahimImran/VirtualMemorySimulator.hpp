#ifndef VIRTUAL_MEMORY_SIMULATOR_HPP
#define VIRTUAL_MEMORY_SIMULATOR_HPP


#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <unordered_map>
#include <deque>
#include <algorithm>

#define PAGE_SIZE 4096

#define ENABLE 1
#define DISABLE 0

#define FIFO 1
#define LRU 2
#define SEGMENTED_FIFO 3

#define HIT 0
#define MISS 1

#define DIRTY 1

using namespace std;

class VirtualMemorySimulator {
public:
    VirtualMemorySimulator(const string traceFile, int pageFrameCount);
    void handleMemoryTrace(bool debugMode, int policy, int primarySegmentSize, int secondarySegmentSize);
    void debugPolicy(bool pageFault, int pageNumber);
    void printStats();

    void outputFile(string fileName);
private:
    string traceFile;
    int traceCount = 0;
    int diskReads = 0;
    int diskWrites = 0;

    int pageFrameCount;

    deque<int> pageFrameDeque;
    unordered_map<int, bool> pageTable;

    void policyFIFO(int pageNumber, char accessType, bool debugMode);
    void policyFIFO_HIT(int pageNumber, char accessType);
    void policyFIFO_MISS(int pageNumber, char accessType);
    void policyFIFO_REPLACE(int pageNumber, char accessType);
    
    void policyLRU(int pageNumber, char accessType, bool debugMode);
    void policyLRU_HIT(int pageNumber, char accessType);
    void policyLRU_MISS(int pageNumber, char accessType);
    void policyLRU_REPLACE(int pageNumber, char accessType);
};


#endif // VIRTUAL_MEMORY_SIMULATOR_HPP
