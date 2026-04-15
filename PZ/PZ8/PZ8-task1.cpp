#include <algorithm>
#include <iomanip>
#include <iostream>
#include <vector>

struct Process {
    int id;
    int basePriority;
    int currentPriority;
    int burstTime;
    int remainingTime;
    int finishTime;
};

int chooseProcess(const std::vector<Process>& processes)
{
    int selected = -1;

    for (int i = 0; i < static_cast<int>(processes.size()); ++i) {
        if (processes[i].remainingTime <= 0) {
            continue;
        }

        if (selected == -1 || processes[i].currentPriority > processes[selected].currentPriority ||
            (processes[i].currentPriority == processes[selected].currentPriority && processes[i].id < processes[selected].id)) {
            selected = i;
        }
    }

    return selected;
}

int main()
{
    int n;
    std::cout << "Number of processes: ";
    std::cin >> n;

    if (!std::cin || n <= 0) {
        std::cerr << "Invalid number of processes.\n";
        return 1;
    }

    std::vector<Process> processes;
    processes.reserve(n);

    std::cout << "Enter for each process: base_priority burst_time\n";
    for (int i = 0; i < n; ++i) {
        int basePriority;
        int burstTime;

        std::cout << "P" << (i + 1) << ": ";
        std::cin >> basePriority >> burstTime;

        if (!std::cin || burstTime <= 0) {
            std::cerr << "Invalid input data for process P" << (i + 1) << ".\n";
            return 1;
        }

        processes.push_back({i + 1, basePriority, basePriority, burstTime, burstTime, -1});
    }

    int time = 0;
    int completed = 0;
    std::vector<int> timeline;

    std::cout << "\nStep-by-step simulation (each step = 1 time quantum):\n";

    while (completed < n) {
        const int selected = chooseProcess(processes);

        if (selected == -1) {
            break;
        }

        std::cout << "t=" << std::setw(2) << time << " -> CPU is assigned to P" << processes[selected].id << " | ";

        for (int i = 0; i < n; ++i) {
            if (processes[i].remainingTime <= 0) {
                continue;
            }

            if (i == selected) {
                processes[i].remainingTime -= 1;
                processes[i].currentPriority = processes[i].basePriority;
            }
            else {
                processes[i].currentPriority += 1;
            }

            std::cout << "P" << processes[i].id << "(pr=" << processes[i].currentPriority
                      << ", rem=" << processes[i].remainingTime << ") ";
        }

        std::cout << '\n';
        timeline.push_back(processes[selected].id);

        if (processes[selected].remainingTime == 0) {
            processes[selected].finishTime = time + 1;
            completed += 1;
        }

        time += 1;
    }

    std::cout << "\nExecution order: ";
    for (int pid : timeline) {
        std::cout << "P" << pid << ' ';
    }

    std::cout << "\n\nSummary:\n";
    std::cout << "Process  Burst  Finish  Waiting\n";

    double avgWaiting = 0.0;
    for (const auto& process : processes) {
        const int waitingTime = process.finishTime - process.burstTime;
        avgWaiting += waitingTime;

        std::cout << "P" << std::setw(5) << process.id << std::setw(7) << process.burstTime << std::setw(8)
                  << process.finishTime << std::setw(9) << waitingTime << '\n';
    }

    avgWaiting /= static_cast<double>(n);
    std::cout << "Average waiting time: " << std::fixed << std::setprecision(2) << avgWaiting << '\n';

    return 0;
}
