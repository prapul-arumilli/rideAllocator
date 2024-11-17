#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <algorithm>

struct Driver {
    std::string name;
    int capacity;
    int initialCapacity;
    std::vector<std::string> riders;

    Driver(const std::string& name, int capacity)
        : name(name), capacity(capacity), initialCapacity(capacity) {}
};

struct Rider {
    std::string name;
    int groupID;

    Rider(const std::string& name, int groupID = -1)
        : name(name), groupID(groupID) {}
};

// Function to calculate the fill ratio of a driver
double calculateFillRatio(const Driver& driver) {
    return 1.0 - static_cast<double>(driver.capacity) / driver.initialCapacity;
}

// Function to assign riders while balancing fill ratios
void assignRidersToDrivers(std::vector<Driver>& drivers, std::vector<Rider>& riders, std::vector<Rider>& unassigned) {
    // Sort riders by groupID to ensure group preferences are respected
    std::sort(riders.begin(), riders.end(), [](const Rider& a, const Rider& b) {
        return a.groupID < b.groupID;
    });

    // Assign riders one by one to the driver with the lowest fill ratio
    for (const Rider& rider : riders) {
        auto it = std::min_element(drivers.begin(), drivers.end(), [](const Driver& a, const Driver& b) {
            if (a.capacity == 0) return false; // Skip full drivers
            if (b.capacity == 0) return true;
            return calculateFillRatio(a) < calculateFillRatio(b);
        });

        if (it != drivers.end() && it->capacity > 0) {
            it->riders.push_back(rider.name);
            it->capacity--;
        } else {
            // If no drivers have capacity, add the rider to the unassigned list
            unassigned.push_back(rider);
        }
    }
}

void outputResults(const std::vector<Driver>& drivers, const std::vector<Rider>& unassigned) {
    for (const Driver& driver : drivers) {
        std::cout << "Driver [" << driver.name << "]:\n";
        for (const std::string& rider : driver.riders) {
            std::cout << "  Rider [" << rider << "]\n";
        }
        std::cout << "\n";
    }

    if (!unassigned.empty()) {
        std::cout << "Driver [NULL]:\n";
        for (const Rider& rider : unassigned) {
            std::cout << "  Rider [" << rider.name << "]\n";
        }
    }
}

int main() {
    std::ifstream inputFile("input.txt");

    if (!inputFile.is_open()) {
        std::cerr << "Error: Unable to open input file.\n";
        return 1;
    }

    std::vector<Driver> drivers;
    std::vector<Rider> riders;
    std::vector<Rider> unassigned;

    // Read drivers and riders
    std::string line;
    bool readingDrivers = true;
    while (std::getline(inputFile, line)) {
        // Detect blank line to switch to riders
        if (line.empty()) {
            readingDrivers = false;
            continue;
        }

        std::istringstream ss(line);
        if (readingDrivers) {
            std::string name;
            int capacity;
            ss >> name >> capacity;
            drivers.emplace_back(name, capacity);
        } else {
            std::string name;
            int groupID = -1;
            ss >> name;
            if (ss >> groupID) {
                riders.emplace_back(name, groupID);
            } else {
                riders.emplace_back(name);
            }
        }
    }

    inputFile.close();

    // Assign riders to drivers
    assignRidersToDrivers(drivers, riders, unassigned);

    // Output results
    outputResults(drivers, unassigned);

    return 0;
}
