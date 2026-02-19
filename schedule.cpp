#include <iostream>
#include <vector>

using namespace std;

struct Process {
    int id;
    int arrivalTime;
    int burstTime;
    
    // Stats
    int startTime;
    int finishTime;
    int waitingTime;
    int turnaroundTime;
    int responseTime;
    
    // Logic flag for SJF
    bool isCompleted;
};

// Bubble Sort
// Sorts by Arrival Time (Ascending), then by ID (Ascending)
void sortProcesses(vector<Process> &procs) {
    int n = procs.size();
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            
            bool shouldSwap = false;
            
            // Check 1: Arrival Time
            if (procs[j].arrivalTime > procs[j+1].arrivalTime) {
                shouldSwap = true;
            } 
            // Check 2: Tie-Breaker (ID)
            else if (procs[j].arrivalTime == procs[j+1].arrivalTime) {
                if (procs[j].id > procs[j+1].id) {
                    shouldSwap = true;
                }
            }

            // Perform Swap
            if (shouldSwap) {
                Process temp = procs[j];
                procs[j] = procs[j+1];
                procs[j+1] = temp;
            }
        }
    }
}

void fcfs(vector<Process> procs, int testCaseNum) {
    // USE MANUAL SORT
    sortProcesses(procs);

    cout << testCaseNum << " FCFS" << endl;
    
    int currentTime = 0;
    
    // Gantt Chart
    for (int i = 0; i < procs.size(); i++) {
        // IDLE CHECK
        if (currentTime < procs[i].arrivalTime) {
            currentTime = procs[i].arrivalTime;
        }
        
        // Start & Finish logic
        procs[i].startTime = currentTime;
        
        cout << currentTime << " " << procs[i].id << " " << procs[i].burstTime << "X" << endl;
        
        currentTime = currentTime + procs[i].burstTime;
        procs[i].finishTime = currentTime;
        
        // Calculate Metrics
        procs[i].turnaroundTime = procs[i].finishTime - procs[i].arrivalTime;
        procs[i].waitingTime = procs[i].startTime - procs[i].arrivalTime;
        procs[i].responseTime = procs[i].startTime - procs[i].arrivalTime;
    }
    
    // --- METRICS OUTPUT ---
    cout << "Total time elapsed: " << currentTime << "ns" << endl;
    
    double totalWait = 0;
    double totalTurnaround = 0;
    double totalResponse = 0;
    int totalBurst = 0;
    
    for(int i = 0; i < procs.size(); i++) {
        totalWait = totalWait + procs[i].waitingTime;
        totalTurnaround = totalTurnaround + procs[i].turnaroundTime;
        totalResponse = totalResponse + procs[i].responseTime;
        totalBurst = totalBurst + procs[i].burstTime;
    }
    
    cout << "Total CPU burst time: " << totalBurst << "ns" << endl;
    
    double utilization = ((double)totalBurst / currentTime) * 100.0;
    cout << "CPU Utilization: " << utilization << "%" << endl;
    
    cout << "Throughput: " << (double)procs.size() / currentTime << " processes/ns" << endl;

    cout << "Waiting times:" << endl;
    for(int i=0; i<procs.size(); i++) {
        cout << " Process " << procs[i].id << ": " << procs[i].waitingTime << "ns" << endl;
    }
    cout << "Average waiting time: " << totalWait / procs.size() << "ns" << endl;

    cout << "Turnaround times:" << endl;
    for(int i=0; i<procs.size(); i++) {
        cout << " Process " << procs[i].id << ": " << procs[i].turnaroundTime << "ns" << endl;
    }
    cout << "Average turnaround time: " << totalTurnaround / procs.size() << "ns" << endl;

    cout << "Response times:" << endl;
    for(int i=0; i<procs.size(); i++) {
        cout << " Process " << procs[i].id << ": " << procs[i].responseTime << "ns" << endl;
    }
    cout << "Average response time: " << totalResponse / procs.size() << "ns" << endl;
}

// --- SJF ALGORITHM ---
void sjf(vector<Process> procs, int testCaseNum) {
    // USE MANUAL SORT
    sortProcesses(procs);

    cout << testCaseNum << " SJF" << endl;

    int currentTime = 0;
    int completedCount = 0;
    int n = procs.size();

    while (completedCount < n) {
        int shortestIdx = -1;
        
        // Find best process in Ready Queue
        for (int i = 0; i < n; i++) {
            if (procs[i].arrivalTime <= currentTime && !procs[i].isCompleted) {
                
                if (shortestIdx == -1) {
                    shortestIdx = i;
                } 
                else {
                    // SJF Logic: Compare Burst
                    if (procs[i].burstTime < procs[shortestIdx].burstTime) {
                        shortestIdx = i;
                    }
                    // Tie-breaker 1: Arrival Time
                    else if (procs[i].burstTime == procs[shortestIdx].burstTime) {
                        if (procs[i].arrivalTime < procs[shortestIdx].arrivalTime) {
                            shortestIdx = i;
                        } 
                        // Tie-breaker 2: ID
                        else if (procs[i].arrivalTime == procs[shortestIdx].arrivalTime) {
                            if (procs[i].id < procs[shortestIdx].id) {
                                shortestIdx = i;
                            }
                        }
                    }
                }
            }
        }

        if (shortestIdx != -1) {
            // EXECUTE
            procs[shortestIdx].startTime = currentTime;
            
            cout << currentTime << " " << procs[shortestIdx].id << " " << procs[shortestIdx].burstTime << "X" << endl;

            currentTime = currentTime + procs[shortestIdx].burstTime;
            procs[shortestIdx].finishTime = currentTime;
            procs[shortestIdx].isCompleted = true;
            completedCount++;
            
            // Calculate Stats
            procs[shortestIdx].turnaroundTime = procs[shortestIdx].finishTime - procs[shortestIdx].arrivalTime;
            procs[shortestIdx].waitingTime = procs[shortestIdx].startTime - procs[shortestIdx].arrivalTime;
            procs[shortestIdx].responseTime = procs[shortestIdx].startTime - procs[shortestIdx].arrivalTime;

        } else {
            // IDLE
            currentTime++;
        }
    }
    
    // --- METRICS OUTPUT ---
    cout << "Total time elapsed: " << currentTime << "ns" << endl;
    
    double totalWait = 0;
    double totalTurnaround = 0;
    double totalResponse = 0;
    int totalBurst = 0;
    
    // Manual Bubble Sort by ID for final output
    for(int i=0; i<n-1; i++) {
        for(int j=0; j<n-i-1; j++) {
            if(procs[j].id > procs[j+1].id) {
                Process temp = procs[j];
                procs[j] = procs[j+1];
                procs[j+1] = temp;
            }
        }
    }

    for(int i = 0; i < n; i++) {
        totalWait = totalWait + procs[i].waitingTime;
        totalTurnaround = totalTurnaround + procs[i].turnaroundTime;
        totalResponse = totalResponse + procs[i].responseTime;
        totalBurst = totalBurst + procs[i].burstTime;
    }
    
    cout << "Total CPU burst time: " << totalBurst << "ns" << endl;
    double utilization = ((double)totalBurst / currentTime) * 100.0;
    cout << "CPU Utilization: " << utilization << "%" << endl;
    cout << "Throughput: " << (double)n / currentTime << " processes/ns" << endl;

    cout << "Waiting times:" << endl;
    for(int i=0; i<n; i++) {
        cout << " Process " << procs[i].id << ": " << procs[i].waitingTime << "ns" << endl;
    }
    cout << "Average waiting time: " << totalWait / n << "ns" << endl;

    cout << "Turnaround times:" << endl;
    for(int i=0; i<n; i++) {
        cout << " Process " << procs[i].id << ": " << procs[i].turnaroundTime << "ns" << endl;
    }
    cout << "Average turnaround time: " << totalTurnaround / n << "ns" << endl;

    cout << "Response times:" << endl;
    for(int i=0; i<n; i++) {
        cout << " Process " << procs[i].id << ": " << procs[i].responseTime << "ns" << endl;
    }
    cout << "Average response time: " << totalResponse / n << "ns" << endl;
}

int main() {
    int testCases;
    if (!(cin >> testCases)) return 0;

    for (int t = 1; t <= testCases; t++) {
        int numProcesses;
        string algorithm;
        cin >> numProcesses >> algorithm;

        vector<Process> processes;
        for (int i = 0; i < numProcesses; i++) {
            int arrival, burst, priority;
            cin >> arrival >> burst >> priority;
            
            Process p;
            p.id = i + 1; 
            p.arrivalTime = arrival;
            p.burstTime = burst;
            p.isCompleted = false;
            processes.push_back(p);
        }

        if (algorithm == "FCFS") {
            fcfs(processes, t);
        } 
        else if (algorithm == "SJF") {
            sjf(processes, t);
        }
    }
    return 0;
}