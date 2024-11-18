#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include <string>
#include <algorithm>

using namespace std;

class Driver {
public:
    string name;
    int capacity;
    int initial_capacity;
    int group_id; // -1 indicates no specific group to pick up
    vector<string> riders;

    Driver(string name, int capacity, int initial_capacity, int group_id = -1)
        : name(name), capacity(capacity), initial_capacity(initial_capacity), group_id(group_id) {}

    double calculateFillRatio() const {
        return (initial_capacity == 0) ? 1.0 : 1.0 - (static_cast<double>(capacity) / initial_capacity);
    }
};

class Rider {
public:
    string name;
    int group_id; // -1 indicates no specific group

    Rider(string name, int group_id = -1) : name(name), group_id(group_id) {}
};

map<int, vector<Rider>> groupRidersByGroupId(const vector<Rider>& riders) {
    map<int, vector<Rider>> grouped_riders;
    for (const auto& rider : riders) {
        grouped_riders[rider.group_id].push_back(rider);
    }
    return grouped_riders;
}

void assignGroupedRidersToDrivers(vector<Driver>& drivers, map<int, vector<Rider>>& grouped_riders, vector<Rider>& unassigned) {
    for (auto& [group_id, riders] : grouped_riders) {
        int group_size = riders.size();

        // Handle ungrouped riders later
        if (group_id == -1) {
            unassigned.insert(unassigned.end(), riders.begin(), riders.end());
            continue;
        }

        // Find a driver with the same group ID and enough capacity
        Driver* selected_driver = nullptr;
        for (auto& driver : drivers) {
            if (driver.group_id == group_id && driver.capacity >= group_size) {
                selected_driver = &driver;
                break;
            }
        }

        // If no matching group ID, find the first available driver with enough capacity
        if (!selected_driver) {
            for (auto& driver : drivers) {
                if (driver.capacity >= group_size) {
                    selected_driver = &driver;
                    break;
                }
            }
        }

        if (selected_driver) {
            for (const auto& rider : riders) {
                selected_driver->riders.push_back(rider.name);
            }
            selected_driver->capacity -= group_size;
        } else {
            // No driver has enough capacity, add riders to unassigned
            unassigned.insert(unassigned.end(), riders.begin(), riders.end());
        }
    }
}

vector<Rider> assignUngroupedRiders(vector<Driver>& drivers, vector<Rider>& unassigned) {
    vector<Rider> remaining_riders;

    for (const auto& rider : unassigned) {
        Driver* selected_driver = nullptr;
        int min_riders = INT_MAX;

        for (auto& driver : drivers) {
            if (driver.capacity > 0 && driver.riders.size() < static_cast<size_t>(min_riders)) {
                selected_driver = &driver;
                min_riders = driver.riders.size();
            }
        }

        if (selected_driver) {
            selected_driver->riders.push_back(rider.name);
            selected_driver->capacity--;
        } else {
            remaining_riders.push_back(rider);
        }
    }

    return remaining_riders;
}

void outputResults(const vector<Driver>& drivers, const vector<Rider>& unassigned) {
    int total_capacity_used = 0;
    int total_empty_seats = 0;

    cout << "\n";
    for (const auto& driver : drivers) {
        cout << driver.name << ":\n";
        if (driver.riders.empty()) {
            cout << "  No Riders Assigned\n";
        } else {
            for (const auto& rider : driver.riders) {
                cout << "  " << rider << "\n";
            }
            total_capacity_used += (driver.initial_capacity - driver.capacity);
        }
        total_empty_seats += driver.capacity;
        cout << "\n";
    }

    if (!unassigned.empty()) {
        cout << "NULL:\n";
        for (const auto& rider : unassigned) {
            cout << "  " << rider.name << "\n";
        }
        cout << "\n";
    }

    cout << "Summary:\n";
    cout << "  Total Capacity Used: " << total_capacity_used << "\n";
    cout << "  Total Empty Seats Remaining: " << total_empty_seats << "\n";
    cout << "  Total Riders Unassigned: " << unassigned.size() << "\n";
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        cerr << "Usage: " << argv[0] << " <input_file>\n";
        return 1;
    }

    ifstream input_file(argv[1]);
    if (!input_file) {
        cerr << "Error: Unable to open input file: " << argv[1] << "\n";
        return 1;
    }

    vector<Driver> drivers;
    vector<Rider> riders;
    vector<Rider> unassigned;
    bool reading_drivers = true;

    string line;
    while (getline(input_file, line)) {
        if (line.empty()) {
            reading_drivers = false;
            continue;
        }

        istringstream iss(line);
        string name;
        int capacity, group_id = -1;

        if (reading_drivers) {
            iss >> name >> capacity;
            if (iss >> group_id) {
                drivers.emplace_back(name, capacity, capacity, group_id);
            } else {
                drivers.emplace_back(name, capacity, capacity);
            }
        } else {
            iss >> name;
            if (iss >> group_id) {
                riders.emplace_back(name, group_id);
            } else {
                riders.emplace_back(name);
            }
        }
    }

    auto grouped_riders = groupRidersByGroupId(riders);
    assignGroupedRidersToDrivers(drivers, grouped_riders, unassigned);
    unassigned = assignUngroupedRiders(drivers, unassigned);
    outputResults(drivers, unassigned);

    return 0;
}
