#include <deque>
#include <iomanip>
#include <iostream>
#include <limits>
#include <vector>

struct Process {
	int id;
	int burstTime;
	int remainingTime;
	int currentQueue;
	int finishTime;
};

int main()
{
	const int queueCount = 4;
	std::vector<int> quantum = {1, 2, 4, std::numeric_limits<int>::max()};

	int n;
	std::cout << "Number of processes: ";
	std::cin >> n;

	if (!std::cin || n <= 0) {
		std::cerr << "Invalid number of processes.\n";
		return 1;
	}

	std::vector<Process> processes;
	processes.reserve(n);

	std::vector<std::deque<int>> queues(queueCount);

	std::cout << "Enter burst time for each process:\n";
	for (int i = 0; i < n; ++i) {
		int burst;
		std::cout << "P" << (i + 1) << ": ";
		std::cin >> burst;

		if (!std::cin || burst <= 0) {
			std::cerr << "Invalid burst time for process P" << (i + 1) << ".\n";
			return 1;
		}

		processes.push_back({i + 1, burst, burst, 0, -1});
		queues[0].push_back(i);
	}

	int time = 0;
	int completed = 0;
	std::vector<int> timeline;

	std::cout << "\nSimulation:\n";

	while (completed < n) {
		int selectedQueue = -1;
		for (int q = 0; q < queueCount; ++q) {
			if (!queues[q].empty()) {
				selectedQueue = q;
				break;
			}
		}

		if (selectedQueue == -1) {
			break;
		}

		const int processIndex = queues[selectedQueue].front();
		queues[selectedQueue].pop_front();
		Process& p = processes[processIndex];

		const int runTime = (selectedQueue == 3) ? p.remainingTime : std::min(quantum[selectedQueue], p.remainingTime);

		std::cout << "t=" << std::setw(3) << time << " -> P" << p.id << " from Q" << (selectedQueue + 1)
				  << " runs for " << runTime << "\n";

		for (int tick = 0; tick < runTime; ++tick) {
			timeline.push_back(p.id);
		}

		time += runTime;
		p.remainingTime -= runTime;

		if (p.remainingTime == 0) {
			p.finishTime = time;
			completed += 1;
			std::cout << "  P" << p.id << " finished at t=" << p.finishTime << "\n";
			continue;
		}

		if (selectedQueue < 3) {
			p.currentQueue = selectedQueue + 1;
			queues[selectedQueue + 1].push_back(processIndex);
			std::cout << "  P" << p.id << " moved to Q" << (selectedQueue + 2)
					  << " (remaining=" << p.remainingTime << ")\n";
		}
		else {
			queues[3].push_back(processIndex);
		}
	}

	std::cout << "\nExecution order by CPU ticks:\n";
	for (int pid : timeline) {
		std::cout << "P" << pid << ' ';
	}
	std::cout << "\n\nResults:\n";
	std::cout << "Process  Burst  Finish  Waiting\n";

	double averageWaiting = 0.0;
	for (const auto& p : processes) {
		const int waiting = p.finishTime - p.burstTime;
		averageWaiting += waiting;
		std::cout << "P" << std::setw(5) << p.id << std::setw(7) << p.burstTime << std::setw(8) << p.finishTime
				  << std::setw(9) << waiting << '\n';
	}

	averageWaiting /= static_cast<double>(n);
	std::cout << "Average waiting time: " << std::fixed << std::setprecision(2) << averageWaiting << '\n';

	return 0;
}
