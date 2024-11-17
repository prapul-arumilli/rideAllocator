#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <unordered_map>
#include <algorithm>

struct Driver {
    std::string name;
    int capacity;
    int initialCapacity;
    std::vector<std::string> riders;
};

struct Rider {
    std::string name;
    int groupID = -1;

    Rider(const std::string& riderName, int riderGroupID = -1)
        : name(riderName), groupID(riderGroupID) {}
};

// Function to calculate the fill ratio of a driver
double calculateFillRatio(const Driver& driver) {
    return 1.0 - (double(driver.capacity) / driver.initialCapacity);
}

// Function to group riders by their group ID
std::unordered_map<int, std::vector<Rider>> groupRidersByGroupID(const std::vector<Rider>& riders) {
    std::unordered_map<int, std::vector<Rider>> groupedRiders;
    for (const auto& rider : riders) {
        groupedRiders[rider.groupID].push_back(rider);
    }
    return groupedRiders;
}

// Function to assign grouped riders to drivers
void assignGroupedRidersToDrivers(std::vector<Driver>& drivers, const std::unordered_map<int, std::vector<Rider>>& groupedRiders, std::vector<Rider>& unassigned) {
    for (const auto& group : groupedRiders) {
        const std::vector<Rider>& riders = group.second;
        int groupSize = riders.size();

        // Find the driver with enough capacity and the lowest fill ratio
        auto it = std::min_element(drivers.begin(), drivers.end(), [groupSize](const Driver& a, const Driver& b) {
            if (a.capacity < groupSize) return false;
            if (b.capacity < groupSize) return true;
            return calculateFillRatio(a) < calculateFillRatio(b);
        });

        if (it != drivers.end() && it->capacity >= groupSize) {
            for (const auto& rider : riders) {
                it->riders.push_back(rider.name);
            }
            it->capacity -= groupSize;
        } else {
            // If no driver has enough capacity, add all riders to the unassigned list
            unassigned.insert(unassigned.end(), riders.begin(), riders.end());
        }
    }
}

void outputResults(const std::vector<Driver>& drivers, const std::vector<Rider>& unassigned) {
    std::cout << "=== Assignment Results ===\n";

    int totalCapacityUsed = 0;
    for (const auto& driver : drivers) {
        std::cout << "Driver [" << driver.name << "]:\n";
        if (driver.riders.empty()) {
            std::cout << "  No Riders Assigned\n";
        } else {
            for (const auto& rider : driver.riders) {
                std::cout << "  Rider [" << rider << "]\n";
            }
            totalCapacityUsed += (driver.initialCapacity - driver.capacity);
        }
        std::cout << "\n";
    }

    if (!unassigned.empty()) {
        std::cout << "Driver [NULL]:\n";
        for (const auto& rider : unassigned) {
            std::cout << "  Rider [" << rider.name << "]\n";
        }
        std::cout << "\n";
    }

    std::cout << "=== Summary ===\n";
    std::cout << "Total Capacity Used: " << totalCapacityUsed << "\n";
    std::cout << "Total Riders Unassigned: " << unassigned.size() << "\n";
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <input_file>\n";
        return 1;
    }

    std::ifstream inputFile(argv[1]);

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
            if (ss >> name >> capacity) {
                drivers.push_back({name, capacity, capacity});
            } else {
                std::cerr << "Error: Invalid driver format in input file.\n";
                return 1;
            }
        } else {
            std::string name;
            int groupID = -1;
            ss >> name;
            if (ss >> groupID) {
                riders.push_back(Rider{name, groupID});
            } else if (!name.empty()) {
                riders.push_back(Rider{name});
            } else {
                std::cerr << "Error: Invalid rider format in input file.\n";
                return 1;
            }
        }
    }

    inputFile.close();

    // Group riders by their group ID
    auto groupedRiders = groupRidersByGroupID(riders);

    // Assign grouped riders to drivers
    assignGroupedRidersToDrivers(drivers, groupedRiders, unassigned);

    // Output results
    outputResults(drivers, unassigned);

    return 0;
}
