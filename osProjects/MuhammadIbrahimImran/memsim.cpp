#include <iostream>
#include <string>
#include "VirtualMemorySimulator.hpp"


using namespace std;

int getPolicy(string policy) {
    if (policy == "fifo") return FIFO;
    if (policy == "lru") return LRU;
    if (policy == "vms") return SEGMENTED_FIFO;
    return -1;
}

bool getDebugMode(string debug) {
    if (debug == "debug") return ENABLE;
    if (debug == "quiet") return DISABLE;
    return -1;
}

int main(int argc, char* argv[]) {

    if (argc < 5) {
        cerr << "Usage: memsim <tracefile> <nframes> <lru|fifo|vms> (p if vms) <debug|quiet> " << endl;
        return 1;
    }

    string traceFile = argv[1];
    int pageFrameCount = atoi(argv[2]);
    int policy = getPolicy(argv[3]);
    bool debug = getDebugMode(argv[4]);
    int primarySegmentPercent = 0;

    if (policy == SEGMENTED_FIFO) {
        if (argc < 6) {
            cerr << "Usage: memsim <tracefile> <nframes> <lru|fifo|vms> <p> <debug|quiet> " << endl;
            return 1;
        }
        primarySegmentPercent = atoi(argv[4]);
        debug = getDebugMode(argv[5]);
    } else if (argc > 5) {
        cerr << "Usage: memsim <tracefile> <nframes> <lru|fifo|vms> <debug|quiet> " << endl;
        return 1;
    }
    
    int primarySegmentSize = (pageFrameCount * primarySegmentPercent) / 100;
    int secondarySegmentSize = pageFrameCount - primarySegmentSize;

    VirtualMemorySimulator vms(traceFile, pageFrameCount);
    vms.handleMemoryTrace(debug, policy, primarySegmentSize, secondarySegmentSize);
    vms.printStats();
    return 0;
}