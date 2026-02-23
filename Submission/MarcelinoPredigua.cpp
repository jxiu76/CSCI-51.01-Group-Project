// MARCELINO, Jaren Paolo
// PREDIGUA, Dave

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

// Helper function #1
// Performs a bubble sort on the processes vector for the ready queue
// It sorts primarily by arrival time (Ascending), and uses the process id
// Ascending process id as a tie-breaker if multiple processes arrive at the exact same time.
void sortProcesses(vector<Process> &procs) {
    int n = procs.size();
    
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            bool shouldSwap = false;
            
            // Sort by earliest arrival time 
            if (procs[j].arrivalTime > procs[j+1].arrivalTime) {
                shouldSwap = true;
            } 

            // If arrrival times are the same, lower process id goes first
            else if (procs[j].arrivalTime == procs[j+1].arrivalTime) {
                if (procs[j].id > procs[j+1].id) {
                    shouldSwap = true;
                }
            }
            
            // Swapping
            if (shouldSwap) {
                Process temp = procs[j];
                procs[j] = procs[j+1];
                procs[j+1] = temp;
            }
        }
    }
}

// Helper function #2
// For printing the output to avoid redundancy
void printOutput(vector<Process> procs, int currentTime) {
    int n = procs.size();

    // Output 
    cout << "Total time elapsed: " << currentTime << "ns" << endl;
    
    double totalWait = 0, totalTurnaround = 0, totalResponse = 0;
    int totalBurst = 0;
    
    // Re-sort the process by ID so the final output list is in order
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

    // Ternary operator to prevent division by 0
    double utilization = (currentTime > 0) ? ((double)totalBurst / currentTime) * 100.0 : 0.0; 
    cout << "CPU Utilization: " << utilization << "%" << endl; 

    double throughput = (currentTime > 0) ? ((double)n / currentTime) : 0.0;
    cout << "Throughput: " << throughput << " processes/ns" << endl;

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

// First Come First Serve
void fcfs(vector<Process> procs, int testCaseNum) {
    sortProcesses(procs);
    cout << testCaseNum << " FCFS" << endl;
    int currentTime = 0;
    
    for (int i = 0; i < procs.size(); i++) {
        // If the CPU is idle, jump time forward
        if (currentTime < procs[i].arrivalTime) {
            currentTime = procs[i].arrivalTime;
        }
        
        procs[i].startTime = currentTime;
        cout << currentTime << " " << procs[i].id << " " << procs[i].burstTime << "X" << endl;
        
        // Fast forward time by the burst time amount
        currentTime = currentTime + procs[i].burstTime;
        procs[i].finishTime = currentTime;
        
        // Calculate the outputs
        procs[i].turnaroundTime = procs[i].finishTime - procs[i].arrivalTime;
        procs[i].waitingTime = procs[i].startTime - procs[i].arrivalTime;
        procs[i].responseTime = procs[i].startTime - procs[i].arrivalTime;
    }
    
    printOutput(procs, currentTime);
}

// Shortest Job First
void sjf(vector<Process> procs, int testCaseNum) {
    sortProcesses(procs);
    cout << testCaseNum << " SJF" << endl;

    int currentTime = 0;
    int completedCount = 0;
    int n = procs.size();

    while (completedCount < n) {
        int shortestIdx = -1;
        
        // Scan the ready queue for process that have arrived and aren't done
        for (int i = 0; i < n; i++) {
            if (procs[i].arrivalTime <= currentTime && !procs[i].isCompleted) {

                // If this is the first valid process, set it as shortest
                if (shortestIdx == -1) {
                    shortestIdx = i;
                } 
                else {
                    // Find process with the smallest total burst time
                    if (procs[i].burstTime < procs[shortestIdx].burstTime) {
                        shortestIdx = i;
                    }
                    // Tie breaker 1: If burst time matches, pick the earliest arrival
                    else if (procs[i].burstTime == procs[shortestIdx].burstTime) {
                        if (procs[i].arrivalTime < procs[shortestIdx].arrivalTime) {
                            shortestIdx = i;
                        }
                        // Tie breaker 2: If arrival match, pick the lower ID
                        else if (procs[i].arrivalTime == procs[shortestIdx].arrivalTime) {
                            if (procs[i].id < procs[shortestIdx].id) {
                                shortestIdx = i;
                            }
                        }
                    }
                }
            }
        }

        // If a process was found in the ready queue
        if (shortestIdx != -1) {
            procs[shortestIdx].startTime = currentTime;
            cout << currentTime << " " << procs[shortestIdx].id << " " << procs[shortestIdx].burstTime << "X" << endl;

            // Burst
            currentTime = currentTime + procs[shortestIdx].burstTime;
            procs[shortestIdx].finishTime = currentTime;
            procs[shortestIdx].isCompleted = true;
            completedCount++;
            
            // Calculate output
            procs[shortestIdx].turnaroundTime = procs[shortestIdx].finishTime - procs[shortestIdx].arrivalTime;
            procs[shortestIdx].waitingTime = procs[shortestIdx].startTime - procs[shortestIdx].arrivalTime;
            procs[shortestIdx].responseTime = procs[shortestIdx].startTime - procs[shortestIdx].arrivalTime;
        } else {
            // If the ready queue is empty, jump time to the next process arrival
            int nextArrival = -1;
            for (int i = 0; i < n; i++) {
                if (!procs[i].isCompleted && procs[i].arrivalTime > currentTime) {
                    if (nextArrival == -1 || procs[i].arrivalTime < nextArrival) {
                        nextArrival = procs[i].arrivalTime;
                    }
                }
            }
            
            if (nextArrival != -1) {
                currentTime = nextArrival;
            } else {
                currentTime++;
            }
        }
    }
    
   printOutput(procs, currentTime);
}

// Shortest Remaining Time First
void srtf(vector<Process> procs, int testCaseNum) {
    sortProcesses(procs);
    cout << testCaseNum << " SRTF" << endl;

    int currentTime = 0;
    int completedCount = 0;
    int n = procs.size();

    int lastProcessId = -1;
    int startBlockTime = 0;
    int currentBurst = 0;

    // Simulate time nanosecond by nanosecond to check for preemptions
    while (completedCount < n) {
        int shortestIdx = -1;
        
        // Scan the ready queue for the process with the shortest remaining time 
        for (int i = 0; i < n; i++) {
            if (procs[i].arrivalTime <= currentTime && !procs[i].isCompleted) {
                if (shortestIdx == -1) {
                    shortestIdx = i;
                } 
                else {
                    // Condition 1: A new process has less remaining time than the current one
                    if (procs[i].remainingTime < procs[shortestIdx].remainingTime) {
                        shortestIdx = i;
                    }
                    // Tie breakers with arrival time and process ID
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
            // Context switch
            if (lastProcessId != -1 && procs[shortestIdx].id != lastProcessId) {
                cout << startBlockTime << " " << lastProcessId << " " << currentBurst << endl;
                startBlockTime = currentTime;
                currentBurst = 0;
            }

            lastProcessId = procs[shortestIdx].id;

            // Only log the start time the very first time the process gets the CPU
            if (procs[shortestIdx].startTime == -1) {
                procs[shortestIdx].startTime = currentTime;
            }

            // Run for 1 nanosecond
            procs[shortestIdx].remainingTime--;
            currentBurst++;
            currentTime++;

            // Check if process finished its total burst
            if (procs[shortestIdx].remainingTime == 0) {
                procs[shortestIdx].finishTime = currentTime;
                procs[shortestIdx].isCompleted = true;
                completedCount++;
                
                cout << startBlockTime << " " << procs[shortestIdx].id << " " << currentBurst << "X" << endl;
                
                // Calculate output
                procs[shortestIdx].turnaroundTime = procs[shortestIdx].finishTime - procs[shortestIdx].arrivalTime;
                procs[shortestIdx].waitingTime = procs[shortestIdx].turnaroundTime - procs[shortestIdx].burstTime;
                procs[shortestIdx].responseTime = procs[shortestIdx].startTime - procs[shortestIdx].arrivalTime;

                // Reset variables
                lastProcessId = -1; 
                startBlockTime = currentTime;
                currentBurst = 0;
                }
            } else {
            // If CPU is idle
            if (lastProcessId != -1) {
                 cout << startBlockTime << " " << lastProcessId << " " << currentBurst << endl;
                 lastProcessId = -1;
                 currentBurst = 0;
            }
            // Fast forward time to the next arrival
            int nextArrival = -1;
            for (int i = 0; i < n; i++) {
                if (!procs[i].isCompleted && procs[i].arrivalTime > currentTime) {
                    if (nextArrival == -1 || procs[i].arrivalTime < nextArrival) {
                        nextArrival = procs[i].arrivalTime;
                    }
                }
            }
            
            if (nextArrival != -1) {
                currentTime = nextArrival;
            } else {
                currentTime++;
            }
            
            // Sync block start time to account for the gap due to idle
            if (currentBurst == 0) {
                startBlockTime = currentTime;
            }
        }
    }
    
    printOutput(procs, currentTime);
}

// Priority
void p(vector<Process> procs, int testCaseNum) {
    sortProcesses(procs);
    cout << testCaseNum << " P" << endl;

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
             // Context switch
            if (lastProcessId != -1 && procs[highestPriorityIdx].id != lastProcessId) {
                cout << startBlockTime << " " << lastProcessId << " " << currentBurst << endl;
                startBlockTime = currentTime;
                currentBurst = 0;
            }

            lastProcessId = procs[highestPriorityIdx].id;

            if (procs[highestPriorityIdx].startTime == -1 ) {
                procs[highestPriorityIdx].startTime = currentTime;
            }

            // Runs 1ns
            procs[highestPriorityIdx].remainingTime--;
            currentBurst++;
            currentTime++;

            // Completing the process
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
        
            int nextArrival = -1;
            for (int i = 0; i < n; i++) {
                if (!procs[i].isCompleted && procs[i].arrivalTime > currentTime) {
                    if (nextArrival == -1 || procs[i].arrivalTime < nextArrival) {
                        nextArrival = procs[i].arrivalTime;
                    }
                }
            }
        
            if (nextArrival != -1) {
                currentTime = nextArrival;
            } else {
                currentTime++;
            }
        
            if (currentBurst == 0) {
                startBlockTime = currentTime;
            }            
        }
    }

   printOutput(procs, currentTime);
}

void rr(vector<Process> procs, int testCaseNum, int quantum) {
    sortProcesses(procs);
    cout << testCaseNum << " RR" << endl;

    int currentTime = 0;
    int completeCount = 0;
    int n = procs.size();

    // Queueing exclusively the preempted processes
    queue<int> preemptedQueue;

    while (completeCount < n) {
        int currentIdx = -1;

        // Priority 1: Check for new arrivals
        for (int i = 0; i < n; i++) {
            if (procs[i].arrivalTime <= currentTime && !procs[i].isCompleted && procs[i].startTime == -1) {
                currentIdx = i;
                break; // Since procs is sorted, this guarantees the earliest arrival
            }
        }

        // Priority 2: If there is no new arrivals, proceed to preempted queue
        if (currentIdx == -1 && !preemptedQueue.empty()) {
            currentIdx = preemptedQueue.front();
            preemptedQueue.pop();
        }

        // Run the process once it is found
        if (currentIdx != -1) {
            // Record the start time
            if (procs[currentIdx].startTime == -1) {
                procs[currentIdx].startTime = currentTime;
            }

            // Processes can only run for their time slice
            int runTime = min(procs[currentIdx].remainingTime, quantum);
            int startBlockTime = currentTime;

            currentTime += runTime;
            procs[currentIdx].remainingTime -= runTime;

            // Thhis prints the execution block
            cout << startBlockTime << " " << procs[currentIdx].id << " " << runTime;

            // Check if the process is finished
            if (procs[currentIdx].remainingTime == 0) {
                cout << "X" << endl;
                procs[currentIdx].finishTime = currentTime;
                procs[currentIdx].isCompleted = true;
                completeCount++;

                procs[currentIdx].turnaroundTime = procs[currentIdx].finishTime - procs[currentIdx].arrivalTime;
                procs[currentIdx].waitingTime = procs[currentIdx].turnaroundTime - procs[currentIdx].burstTime;
                procs[currentIdx].responseTime = procs[currentIdx].startTime - procs[currentIdx].arrivalTime;
            } else {
                cout << endl;
                // If the process didn't finish, it gets preempted
                // Move it to the tail end of the queue
                preemptedQueue.push(currentIdx);
            }
        } else {
            // If the CPU is in idle, we need to advance time to the next process
            int nextArrival = -1;
            for (int i = 0; i < n; i++) {
                if (!procs[i].isCompleted && procs[i].arrivalTime > currentTime) {
                    if (nextArrival == -1 || procs[i].arrivalTime < nextArrival) {
                        nextArrival = procs[i].arrivalTime;
                    }
                }
            }
            
            if (nextArrival != -1) {
                currentTime = nextArrival;
            } else {
                currentTime++;
            }
        }
    }

   printOutput(procs, currentTime);
}

int main() {
    int testCases;
    
    if (!(cin >> testCases)) return 0;

    for (int t = 1; t <= testCases; t++) {
        int numProcesses;
        string algorithm;
        int quantum = 0;
        cin >> numProcesses >> algorithm;

        if (algorithm == "RR") {
            cin >> quantum;
        }

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
        else if (algorithm == "RR") {
            rr(processes, t, quantum);
        }
    }
    return 0;
}