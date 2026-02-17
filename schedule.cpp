#include <iostream>
#include <vector>
#include <algorithm> // Needed for sort()

using namespace std;

struct Process {
    int id;    
    int arrivalTime;    
    int burstTime;     

    int remainingTime;  
    int startTime;   
    int finishTime;    
    int waitingTime;
};

bool compareArrival(const Process &a, const Process &b) {
    if (a.arrivalTime != b.arrivalTime)
        return a.arrivalTime < b.arrivalTime;
    return a.id < b.id;
}