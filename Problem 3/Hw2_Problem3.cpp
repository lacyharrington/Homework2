// Lacy Harrington HW 2 Problem 3


#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <vector>
#include <queue>

using namespace std;

mutex mtx;
condition_variable cv;
int currentPattern = 0;
const int Patmax = 3;
int timeSeconds = 0;
queue<int> landingQueue;

void request_landing(int id) {
    unique_lock<mutex> lock(mtx);
    cout << "Aircraft #" << id << " requesting landing." << endl;

    if (currentPattern < Patmax) {
        currentPattern++;
        landingQueue.push(id);
        cv.notify_one();  // add aircraft to the pattern
    }
    else {
        cout << "Approach pattern is full. Aircraft #" << id << " redirected to another airport." << endl;
        cout << "Aircraft #" << id << " flying to another airport." << endl;
    }
}

void atc_function() {
    unique_lock<mutex> lock(mtx);

    while (timeSeconds < 5) {
        cv.wait(lock, [] { return !landingQueue.empty(); });

        while (!landingQueue.empty()) {
            int aircraftId = landingQueue.front();
            landingQueue.pop();

            cout << "Aircraft #" << aircraftId << " is cleared to land." << endl;
            currentPattern--;
            timeSeconds++;
            cout << "Runway is now free." << endl;
            cv.notify_all();
            this_thread::sleep_for(chrono::seconds(1)); //  landing time
        }
    }
}

int main() {
    vector<thread> aircraftThreads;
    thread atcThread(atc_function);

    // initiate threads for first 2 aircraft
    aircraftThreads.push_back(thread(request_landing, 0));
    aircraftThreads.push_back(thread(request_landing, 1));

    // grant the first 2 aircraft landing 
    aircraftThreads[0].join();
    aircraftThreads[1].join();

    // Other 8
    for (int i = 2; i < 10; ++i) {
        aircraftThreads.push_back(thread(request_landing, i));
    }

    // Wait for the rest of the aircraft threads to complete
    for (size_t i = 2; i < aircraftThreads.size(); ++i) {
        if (aircraftThreads[i].joinable()) {
            aircraftThreads[i].join();
        }
    }

    atcThread.join();
    cout << "Duration: " << timeSeconds << " seconds." << endl;

    return 0;
}