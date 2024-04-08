// Lacy Harrington HW 2 Problem 2

#include <iostream>
#include <thread>
#include <mutex>
#include <chrono>
#include <condition_variable>
#include <vector>

const int NUM_ROBOTS = 5;
std::mutex mutex_tool[NUM_ROBOTS]; // Mutexes for each tool
std::mutex mutex_task_counter;
std::mutex mutex_ready_counter;
std::condition_variable cv_task; // Condition variable for task synchronization
std::condition_variable cv_ready; // Condition variable for ready synchronization
bool tool_available[NUM_ROBOTS] = { true, true, true, true, true }; // Tool availability
std::vector<int> task_counter(NUM_ROBOTS, 0); // Counter for the number of tasks being performed for each robot
int ready_counter = 0; // Counter for the number of robots ready to perform tasks

bool canStartTask(int robotId) {
    // Check if adjacent robots are not performing tasks
    int leftNeighbor = (robotId + NUM_ROBOTS - 1) % NUM_ROBOTS;
    int rightNeighbor = (robotId + 1) % NUM_ROBOTS;
    return task_counter[leftNeighbor] == 0 && task_counter[rightNeighbor] == 0;
}

void performTask(int robotId) {
    // Indicate readiness to perform task
    {
        std::unique_lock<std::mutex> lk(mutex_ready_counter);
        ++ready_counter;
        // Wait for another robot to be ready
        cv_ready.wait(lk, [&] { return ready_counter >= 2; });
        cv_ready.notify_one(); // Notify another robot
    }

    {
        std::unique_lock<std::mutex> lk(mutex_task_counter);
        // Wait if two robots are already performing tasks or adjacent robots are busy
        cv_task.wait(lk, [&] { return std::count(task_counter.begin(), task_counter.end(), 1) < 2 && canStartTask(robotId); });
        task_counter[robotId] = 1;
        --ready_counter; // No longer in ready state
    }

    std::cout << "Robot " << robotId << " is collecting data.\n";

    // Tools required by the robot
    int leftTool = robotId;
    int rightTool = (robotId + 1) % NUM_ROBOTS;

    // Wait and acquire tools
    std::unique_lock<std::mutex> lk1(mutex_tool[leftTool], std::defer_lock);
    std::unique_lock<std::mutex> lk2(mutex_tool[rightTool], std::defer_lock);
    std::lock(lk1, lk2); // Correctly locks both mutexes simultaneously
    tool_available[leftTool] = false;
    tool_available[rightTool] = false;

    std::cout << "Robot " << robotId << " acquired tools and started performing tasks.\n";

    // Performing the task (6 seconds total)
    std::this_thread::sleep_for(std::chrono::seconds(6));

    // Task finished, return tools and notify
    tool_available[leftTool] = true;
    tool_available[rightTool] = true;
    lk1.unlock();
    lk2.unlock();

    std::cout << "Robot " << robotId << " finished the task and returning the tools.\n";

    {
        std::lock_guard<std::mutex> lk(mutex_task_counter);
        task_counter[robotId] = 0;
        cv_task.notify_all();
    }
}

int main() {
    auto start = std::chrono::high_resolution_clock::now(); // Start time


    std::thread robots[NUM_ROBOTS];

    // Create threads for each robot
    for (int i = 0; i < NUM_ROBOTS; ++i) {
        robots[i] = std::thread(performTask, i);
    }

    // Wait for all robots to finish
    for (auto& robot : robots) {
        robot.join();
    }

    auto end = std::chrono::high_resolution_clock::now(); // End time
    auto duration = std::chrono::duration_cast<std::chrono::seconds>(end - start);
    std::cout << "All tasks completed.\n";
    std::cout << "Total simulation time: " << duration.count() << " seconds.\n";

    return 0;
}