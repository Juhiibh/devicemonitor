#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct {
    int id;
    int burstTime;
    int priority;
} Process;

/*-------------------- FCFS Scheduling --------------------*/

void findWaitingTimeFCFS(int processes[], int n, int burstTime[], int waitingTime[]) {
    waitingTime[0] = 0; // First process has no waiting time
    for (int i = 1; i < n; i++) {
        waitingTime[i] = burstTime[i - 1] + waitingTime[i - 1];
    }
}

/*-------------------- End of FCFS Scheduling --------------------*/

/*-------------------- SJN Scheduling --------------------*/

void findWaitingTimeSJN(int processes[], int n, int burstTime[], int waitingTime[]) {
    int completed = 0, time = 0, minBurstTime, shortest = 0;
    bool found = false;
    int remainingTime[n];

    for (int i = 0; i < n; i++) {
        remainingTime[i] = burstTime[i];
    }

    while (completed != n) {
        minBurstTime = 1e9;
        found = false;

        for (int i = 0; i < n; i++) {
            if (remainingTime[i] > 0 && remainingTime[i] < minBurstTime) {
                minBurstTime = remainingTime[i];
                shortest = i;
                found = true;
            }
        }

        if (!found) {
            time++;
            continue;
        }

        time += remainingTime[shortest];
        waitingTime[shortest] = time - burstTime[shortest];
        remainingTime[shortest] = 0;
        completed++;
    }
}

/*-------------------- End of SJN Scheduling --------------------*/

/*-------------------- Round Robin Scheduling --------------------*/

void findWaitingTimeRR(int processes[], int n, int burstTime[], int waitingTime[], int quantum) {
    int remainingTime[n];
    for (int i = 0; i < n; i++) {
        remainingTime[i] = burstTime[i];
    }

    int time = 0;
    while (1) {
        int done = 1;
        for (int i = 0; i < n; i++) {
            if (remainingTime[i] > 0) {
                done = 0;
                if (remainingTime[i] > quantum) {
                    time += quantum;
                    remainingTime[i] -= quantum;
                } else {
                    time += remainingTime[i];
                    waitingTime[i] = time - burstTime[i];
                    remainingTime[i] = 0;
                }
            }
        }
        if (done) break;
    }
}

/*-------------------- End of Round Robin Scheduling --------------------*/

/*-------------------- Priority Scheduling --------------------*/

void findWaitingTimePriority(Process processes[], int n, int waitingTime[]) {
    waitingTime[0] = 0;
    for (int i = 1; i < n; i++) {
        waitingTime[i] = processes[i - 1].burstTime + waitingTime[i - 1];
    }
}

/*-------------------- End of Priority Scheduling --------------------*/

/*-------------------- Helper Functions --------------------*/

void findTurnAroundTime(int processes[], int n, int burstTime[], int waitingTime[], int turnAroundTime[]) {
    for (int i = 0; i < n; i++) {
        turnAroundTime[i] = burstTime[i] + waitingTime[i];
    }
}

void findAverageTimeFCFS(int processes[], int n, int burstTime[]) {
    int waitingTime[n], turnAroundTime[n];
    int totalWaitingTime = 0, totalTurnAroundTime = 0;

    findWaitingTimeFCFS(processes, n, burstTime, waitingTime);
    findTurnAroundTime(processes, n, burstTime, waitingTime, turnAroundTime);

    printf("Processes  Burst Time  Waiting Time  Turnaround Time\n");
    for (int i = 0; i < n; i++) {
        totalWaitingTime += waitingTime[i];
        totalTurnAroundTime += turnAroundTime[i];
        printf(" %d\t\t%d\t\t%d\t\t%d\n", processes[i], burstTime[i], waitingTime[i], turnAroundTime[i]);
    }
    printf("Average Waiting Time = %.2f\n", (float)totalWaitingTime / n);
    printf("Average Turnaround Time = %.2f\n", (float)totalTurnAroundTime / n);
}

void findAverageTimeSJN(int processes[], int n, int burstTime[]) {
    int waitingTime[n], turnAroundTime[n];
    int totalWaitingTime = 0, totalTurnAroundTime = 0;

    findWaitingTimeSJN(processes, n, burstTime, waitingTime);
    findTurnAroundTime(processes, n, burstTime, waitingTime, turnAroundTime);

    printf("Processes  Burst Time  Waiting Time  Turnaround Time\n");
    for (int i = 0; i < n; i++) {
        totalWaitingTime += waitingTime[i];
        totalTurnAroundTime += turnAroundTime[i];
        printf(" %d\t\t%d\t\t%d\t\t%d\n", processes[i], burstTime[i], waitingTime[i], turnAroundTime[i]);
    }
    printf("Average Waiting Time = %.2f\n", (float)totalWaitingTime / n);
    printf("Average Turnaround Time = %.2f\n", (float)totalTurnAroundTime / n);
}

void findAverageTimeRR(int processes[], int n, int burstTime[], int quantum) {
    int waitingTime[n], turnAroundTime[n];
    int totalWaitingTime = 0, totalTurnAroundTime = 0;

    findWaitingTimeRR(processes, n, burstTime, waitingTime, quantum);
    findTurnAroundTime(processes, n, burstTime, waitingTime, turnAroundTime);

    printf("Processes  Burst Time  Waiting Time  Turnaround Time\n");
    for (int i = 0; i < n; i++) {
        totalWaitingTime += waitingTime[i];
        totalTurnAroundTime += turnAroundTime[i];
        printf(" %d\t\t%d\t\t%d\t\t%d\n", processes[i], burstTime[i], waitingTime[i], turnAroundTime[i]);
    }
    printf("Average Waiting Time = %.2f\n", (float)totalWaitingTime / n);
    printf("Average Turnaround Time = %.2f\n", (float)totalTurnAroundTime / n);
}

void findAverageTimePriority(Process processes[], int n) {
    int waitingTime[n], turnAroundTime[n];
    int totalWaitingTime = 0, totalTurnAroundTime = 0;

    findWaitingTimePriority(processes, n, waitingTime);
    findTurnAroundTime(processes, n, (int *)processes, waitingTime, turnAroundTime);

    printf("Process  Burst Time  Priority  Waiting Time  Turnaround Time\n");
    for (int i = 0; i < n; i++) {
        totalWaitingTime += waitingTime[i];
        totalTurnAroundTime += turnAroundTime[i];
        printf(" %d\t\t%d\t\t%d\t\t%d\t\t%d\n", processes[i].id, processes[i].burstTime, processes[i].priority, waitingTime[i], turnAroundTime[i]);
    }
    printf("Average Waiting Time = %.2f\n", (float)totalWaitingTime / n);
    printf("Average Turnaround Time = %.2f\n", (float)totalTurnAroundTime / n);
}

/*-------------------- Main Function --------------------*/

int main() {
    int choice;
    int quantum = 4;
    int processes[] = {1, 2, 3};
    int burstTime[] = {24, 3, 3};
    int n = sizeof(processes) / sizeof(processes[0]);

    Process priorityProcesses[] = {{1, 10, 2}, {2, 5, 0}, {3, 8, 1}};
    int priorityN = sizeof(priorityProcesses) / sizeof(priorityProcesses[0]);

    printf("Select the Scheduling Algorithm:\n");
    printf("1. First-Come-First-Serve (FCFS)\n");
    printf("2. Shortest Job Next (SJN)\n");
    printf("3. Round Robin (RR)\n");
    printf("4. Priority Scheduling\n");
    printf("Enter your choice: ");
    scanf("%d", &choice);

    switch (choice) {
        case 1:
            printf("\n--- FCFS Scheduling ---\n");
            findAverageTimeFCFS(processes, n, burstTime);
            printf("--- End of FCFS Scheduling ---\n");
            break;
        case 2:
            printf("\n--- SJN Scheduling ---\n");
            findAverageTimeSJN(processes, n, burstTime);
            printf("--- End of SJN Scheduling ---\n");
            break;
        case 3:
            printf("Enter the time quantum: ");
            scanf("%d", &quantum);
            printf("\n--- Round Robin Scheduling ---\n");
            findAverageTimeRR(processes, n, burstTime, quantum);
            printf("--- End of Round Robin Scheduling ---\n");
            break;
        case 4:
            printf("\n--- Priority Scheduling ---\n");
            findAverageTimePriority(priorityProcesses, priorityN);
            printf("--- End of Priority Scheduling ---\n");
            break;
        default:
            printf("Invalid choice!\n");
            break;
    }

    return 0;
}
