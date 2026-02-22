#include <iostream>
#include <vector>
#include <queue>

using namespace std;

// Structure to represent a Process
// Holds all input parameters, tracking variables
struct Process {
    int id;                 
    int arrivalTime;        
    int burstTime;          
    int remainingTime;      
    int startTime;          
    int finishTime;         
    int waitingTime;        
    int turnaroundTime;     
    int responseTime;       
    bool isCompleted;
    int niceLevel;
};

// Performs a bubble sort on the processes vector. 
// It sorts primarily by arrival time (Ascending), and uses the process id
// (Ascending) as a tie-breaker if multiple processes arrive at the exact same time.
void sortProcesses(vector<Process> &procs) {
    int n = procs.size();
    
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            bool shouldSwap = false;
            
            if (procs[j].arrivalTime > procs[j+1].arrivalTime) {
                shouldSwap = true;
            } 
            else if (procs[j].arrivalTime == procs[j+1].arrivalTime) {
                if (procs[j].id > procs[j+1].id) {
                    shouldSwap = true;
                }
            }

            if (shouldSwap) {
                Process temp = procs[j];
                procs[j] = procs[j+1];
                procs[j+1] = temp;
            }
        }
    }
}

void fcfs(vector<Process> procs, int testCaseNum) {
    sortProcesses(procs);
    cout << testCaseNum << " FCFS" << endl;
    int currentTime = 0;
    
    for (int i = 0; i < procs.size(); i++) {
        if (currentTime < procs[i].arrivalTime) {
            currentTime = procs[i].arrivalTime;
        }
        
        procs[i].startTime = currentTime;
        cout << currentTime << " " << procs[i].id << " " << procs[i].burstTime << "X" << endl;
        
        currentTime = currentTime + procs[i].burstTime;
        procs[i].finishTime = currentTime;
        
        procs[i].turnaroundTime = procs[i].finishTime - procs[i].arrivalTime;
        procs[i].waitingTime = procs[i].startTime - procs[i].arrivalTime;
        procs[i].responseTime = procs[i].startTime - procs[i].arrivalTime;
    }
    
    cout << "Total time elapsed: " << currentTime << "ns" << endl;
    
    double totalWait = 0, totalTurnaround = 0, totalResponse = 0;
    int totalBurst = 0;
    
    for (int i = 0; i < procs.size(); i++) {
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
    for (int i = 0; i < procs.size(); i++) cout << " Process " << procs[i].id << ": " << procs[i].waitingTime << "ns" << endl;
    cout << "Average waiting time: " << totalWait / procs.size() << "ns" << endl;

    cout << "Turnaround times:" << endl;
    for (int i = 0; i < procs.size(); i++) cout << " Process " << procs[i].id << ": " << procs[i].turnaroundTime << "ns" << endl;
    cout << "Average turnaround time: " << totalTurnaround / procs.size() << "ns" << endl;

    cout << "Response times:" << endl;
    for (int i = 0; i < procs.size(); i++) cout << " Process " << procs[i].id << ": " << procs[i].responseTime << "ns" << endl;
    cout << "Average response time: " << totalResponse / procs.size() << "ns" << endl;
}

void sjf(vector<Process> procs, int testCaseNum) {
    sortProcesses(procs);
    cout << testCaseNum << " SJF" << endl;

    int currentTime = 0;
    int completedCount = 0;
    int n = procs.size();

    while (completedCount < n) {
        int shortestIdx = -1;
        
        for (int i = 0; i < n; i++) {
            if (procs[i].arrivalTime <= currentTime && !procs[i].isCompleted) {
                if (shortestIdx == -1) {
                    shortestIdx = i;
                } 
                else {
                    if (procs[i].burstTime < procs[shortestIdx].burstTime) {
                        shortestIdx = i;
                    }
                    else if (procs[i].burstTime == procs[shortestIdx].burstTime) {
                        if (procs[i].arrivalTime < procs[shortestIdx].arrivalTime) {
                            shortestIdx = i;
                        } 
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
            procs[shortestIdx].startTime = currentTime;
            cout << currentTime << " " << procs[shortestIdx].id << " " << procs[shortestIdx].burstTime << "X" << endl;

            currentTime = currentTime + procs[shortestIdx].burstTime;
            procs[shortestIdx].finishTime = currentTime;
            procs[shortestIdx].isCompleted = true;
            completedCount++;
            
            procs[shortestIdx].turnaroundTime = procs[shortestIdx].finishTime - procs[shortestIdx].arrivalTime;
            procs[shortestIdx].waitingTime = procs[shortestIdx].startTime - procs[shortestIdx].arrivalTime;
            procs[shortestIdx].responseTime = procs[shortestIdx].startTime - procs[shortestIdx].arrivalTime;
        } else {
            currentTime++;
        }
    }
    
    cout << "Total time elapsed: " << currentTime << "ns" << endl;
    
    double totalWait = 0, totalTurnaround = 0, totalResponse = 0;
    int totalBurst = 0;
    
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (procs[j].id > procs[j+1].id) {
                Process temp = procs[j];
                procs[j] = procs[j+1];
                procs[j+1] = temp;
            }
        }
    }

    for (int i = 0; i < n; i++) {
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
    for (int i = 0; i < n; i++) cout << " Process " << procs[i].id << ": " << procs[i].waitingTime << "ns" << endl;
    cout << "Average waiting time: " << totalWait / n << "ns" << endl;

    cout << "Turnaround times:" << endl;
    for (int i = 0; i < n; i++) cout << " Process " << procs[i].id << ": " << procs[i].turnaroundTime << "ns" << endl;
    cout << "Average turnaround time: " << totalTurnaround / n << "ns" << endl;

    cout << "Response times:" << endl;
    for (int i = 0; i < n; i++) cout << " Process " << procs[i].id << ": " << procs[i].responseTime << "ns" << endl;
    cout << "Average response time: " << totalResponse / n << "ns" << endl;
}

void srtf(vector<Process> procs, int testCaseNum) {
    sortProcesses(procs);
    cout << testCaseNum << " SRTF" << endl;

    int currentTime = 0;
    int completedCount = 0;
    int n = procs.size();

    int lastProcessId = -1;
    int startBlockTime = 0;
    int currentBurst = 0;

    while (completedCount < n) {
        int shortestIdx = -1;
        
        for (int i = 0; i < n; i++) {
            if (procs[i].arrivalTime <= currentTime && !procs[i].isCompleted) {
                if (shortestIdx == -1) {
                    shortestIdx = i;
                } 
                else {
                    if (procs[i].remainingTime < procs[shortestIdx].remainingTime) {
                        shortestIdx = i;
                    }
                    else if (procs[i].remainingTime == procs[shortestIdx].remainingTime) {
                        if (procs[i].arrivalTime < procs[shortestIdx].arrivalTime) {
                            shortestIdx = i;
                        } 
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
            if (lastProcessId != -1 && procs[shortestIdx].id != lastProcessId) {
                cout << startBlockTime << " " << lastProcessId << " " << currentBurst << endl;
                startBlockTime = currentTime;
                currentBurst = 0;
            }

            lastProcessId = procs[shortestIdx].id;

            if (procs[shortestIdx].startTime == -1) {
                procs[shortestIdx].startTime = currentTime;
            }

            procs[shortestIdx].remainingTime--;
            currentBurst++;
            currentTime++;

            if (procs[shortestIdx].remainingTime == 0) {
                procs[shortestIdx].finishTime = currentTime;
                procs[shortestIdx].isCompleted = true;
                completedCount++;
                
                cout << startBlockTime << " " << procs[shortestIdx].id << " " << currentBurst << "X" << endl;
                
                procs[shortestIdx].turnaroundTime = procs[shortestIdx].finishTime - procs[shortestIdx].arrivalTime;
                procs[shortestIdx].waitingTime = procs[shortestIdx].turnaroundTime - procs[shortestIdx].burstTime;
                procs[shortestIdx].responseTime = procs[shortestIdx].startTime - procs[shortestIdx].arrivalTime;

                lastProcessId = -1; 
                startBlockTime = currentTime;
                currentBurst = 0;
            }
        } else {
            if (lastProcessId != -1) {
                 cout << startBlockTime << " " << lastProcessId << " " << currentBurst << endl;
                 lastProcessId = -1;
                 currentBurst = 0;
            }
            
            currentTime++;
            if (currentBurst == 0) {
                startBlockTime = currentTime;
            }
        }
    }
    
    cout << "Total time elapsed: " << currentTime << "ns" << endl;
    
    double totalWait = 0, totalTurnaround = 0, totalResponse = 0;
    int totalBurst = 0;
    
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (procs[j].id > procs[j+1].id) {
                Process temp = procs[j];
                procs[j] = procs[j+1];
                procs[j+1] = temp;
            }
        }
    }

    for (int i = 0; i < n; i++) {
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
    for (int i = 0; i < n; i++) cout << " Process " << procs[i].id << ": " << procs[i].waitingTime << "ns" << endl;
    cout << "Average waiting time: " << totalWait / n << "ns" << endl;

    cout << "Turnaround times:" << endl;
    for (int i = 0; i < n; i++) cout << " Process " << procs[i].id << ": " << procs[i].turnaroundTime << "ns" << endl;
    cout << "Average turnaround time: " << totalTurnaround / n << "ns" << endl;

    cout << "Response times:" << endl;
    for (int i = 0; i < n; i++) cout << " Process " << procs[i].id << ": " << procs[i].responseTime << "ns" << endl;
    cout << "Average response time: " << totalResponse / n << "ns" << endl;
}

// --- P ALGORITHM ---
void p(vector<Process> procs, int testCaseNum) {
    sortProcesses(procs);
    cout << testCaseNum << " P " << endl;

    int currentTime = 0;
    int completedCount = 0;
    int n = procs.size();

    int lastProcessId = -1;
    int startBlockTime = 0;
    int currentBurst = 0;

    while (completedCount < n) {
        int highestPriorityIdx = -1;

        // Find the process with the highest priority (lowest nice level) in the ready queue
        for (int i = 0; i < n; i++) {
            if (procs[i].arrivalTime <= currentTime && !procs[i].isCompleted) {
                if (highestPriorityIdx == -1) {
                    highestPriorityIdx = i;
                } else {
                    // Lower nice level, higher priority
                    if (procs[i].niceLevel < procs[highestPriorityIdx].niceLevel) {
                        highestPriorityIdx = i;
                    // Tie-break 1: Earlier arrival time, higher priority
                    } else if (procs[i].niceLevel == procs[highestPriorityIdx].niceLevel) {
                        if (procs[i].arrivalTime < procs[highestPriorityIdx].arrivalTime) {
                            highestPriorityIdx = i;
                        // Tie-break 2: Lower process index, higher priority 
                        } else if (procs[i].arrivalTime == procs[highestPriorityIdx].arrivalTime) {
                            if (procs[i].id < procs[highestPriorityIdx].id) {
                                highestPriorityIdx = i;
                            }
                        }
                    }
                }
            }
        }

        // Context switch check
        if (highestPriorityIdx != -1) {
             // context switch: flush previous block (non-X)
            if (lastProcessId != -1 && procs[highestPriorityIdx].id != lastProcessId) {
                cout << startBlockTime << " " << lastProcessId << " " << currentBurst << endl;
                startBlockTime = currentTime;
                currentBurst = 0;
            }

            lastProcessId = procs[highestPriorityIdx].id;

            if (procs[highestPriorityIdx].startTime == -1 ) {
                procs[highestPriorityIdx].startTime = currentTime;
            }

            // runs 1ns
            procs[highestPriorityIdx].remainingTime--;
            currentBurst++;
            currentTime++;

            // 
            if (procs[highestPriorityIdx].remainingTime == 0) {
                procs[highestPriorityIdx].finishTime = currentTime;
                procs[highestPriorityIdx].isCompleted = true;
                completedCount++;

                cout << startBlockTime << " " << procs[highestPriorityIdx].id << " " << currentBurst << "X" << endl;

                procs[highestPriorityIdx].turnaroundTime = procs[highestPriorityIdx].finishTime - procs[highestPriorityIdx].arrivalTime;
                procs[highestPriorityIdx].waitingTime = procs[highestPriorityIdx].turnaroundTime - procs[highestPriorityIdx].burstTime;
                procs[highestPriorityIdx].responseTime = procs[highestPriorityIdx].startTime - procs[highestPriorityIdx].arrivalTime;

                lastProcessId = -1; 
                startBlockTime = currentTime;
                currentBurst = 0;               
            }
        } else {
            if (lastProcessId != -1) {
                 cout << startBlockTime << " " << lastProcessId << " " << currentBurst << endl;
                 lastProcessId = -1;
                 currentBurst = 0;
            }
            
            currentTime++;
            if (currentBurst == 0) {
                startBlockTime = currentTime;
            }            
        }
    
    }

    // Output 
    cout << "Total time elapsed: " << currentTime << "ns" << endl;
    
    double totalWait = 0, totalTurnaround = 0, totalResponse = 0;
    int totalBurst = 0;
    
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (procs[j].id > procs[j+1].id) {
                Process temp = procs[j];
                procs[j] = procs[j+1];
                procs[j+1] = temp;
            }
        }
    }

    for (int i = 0; i < n; i++) {
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
    for (int i = 0; i < n; i++) cout << " Process " << procs[i].id << ": " << procs[i].waitingTime << "ns" << endl;
    cout << "Average waiting time: " << totalWait / n << "ns" << endl;

    cout << "Turnaround times:" << endl;
    for (int i = 0; i < n; i++) cout << " Process " << procs[i].id << ": " << procs[i].turnaroundTime << "ns" << endl;
    cout << "Average turnaround time: " << totalTurnaround / n << "ns" << endl;

    cout << "Response times:" << endl;
    for (int i = 0; i < n; i++) cout << " Process " << procs[i].id << ": " << procs[i].responseTime << "ns" << endl;
    cout << "Average response time: " << totalResponse / n << "ns" << endl;
}

void rr(vector<Process> procs, int testCaseNum, int quantum) {
    
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
            
            p.remainingTime = burst; 
            p.startTime = -1;        
            p.isCompleted = false;

            p.niceLevel = priority;
            
            processes.push_back(p);
        }

        if (algorithm == "FCFS") {
            fcfs(processes, t);
        } 
        else if (algorithm == "SJF") {
            sjf(processes, t);
        }
        else if (algorithm == "SRTF") {
            srtf(processes, t);
        }
        else if (algorithm == "P") {
            p(processes, t);
        }
    }
    return 0;
}