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
    int groupID = -1; // Added group ID for drivers
    std::vector<std::string> riders;

    // Constructor
    Driver(const std::string& driverName, int driverCapacity, int driverInitialCapacity, int driverGroupID = -1)
        : name(driverName), capacity(driverCapacity), initialCapacity(driverInitialCapacity), groupID(driverGroupID) {}
};

struct Rider {
    std::string name;
    int groupID = -1;

    Rider(const std::string& riderName, int riderGroupID = -1)
        : name(riderName), groupID(riderGroupID) {}
};

// Function to calculate the fill ratio of a driver
double calculateFillRatio(const Driver& driver) {
    if (driver.initialCapacity == 0) return 1.0; // Prevent division by zero
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

// Function to assign grouped riders to drivers with a balanced distribution
void assignGroupedRidersToDrivers(std::vector<Driver>& drivers, const std::unordered_map<int, std::vector<Rider>>& groupedRiders, std::vector<Rider>& unassigned) {
    for (const auto& group : groupedRiders) {
        const std::vector<Rider>& riders = group.second;
        int groupSize = riders.size();

        // Find the driver with enough capacity and balanced rider count
        auto it = std::min_element(drivers.begin(), drivers.end(), [groupSize](const Driver& a, const Driver& b) {
            if (a.capacity < groupSize) return false; // 'a' cannot accommodate
            if (b.capacity < groupSize) return true;  // 'b' cannot accommodate

            // Prefer the car with fewer total riders assigned (balance rider count)
            if (a.riders.size() != b.riders.size()) {
                return a.riders.size() < b.riders.size();
            }

            // If rider count is equal, compare fill ratio
            double fillRatioA = calculateFillRatio(a);
            double fillRatioB = calculateFillRatio(b);
            return fillRatioA < fillRatioB;
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

// Assign ungrouped riders with a balanced distribution
void assignUngroupedRiders(std::vector<Driver>& drivers, std::vector<Rider>& unassigned) {
    std::vector<Rider> remainingRiders;

    for (const auto& rider : unassigned) {
        auto it = std::min_element(drivers.begin(), drivers.end(), [](const Driver& a, const Driver& b) {
            if (a.capacity == 0) return false; // 'a' cannot accommodate
            if (b.capacity == 0) return true;  // 'b' cannot accommodate

            // Prefer the car with fewer total riders assigned
            return a.riders.size() < b.riders.size();
        });

        if (it != drivers.end() && it->capacity > 0) {
            it->riders.push_back(rider.name);
            it->capacity--;
        } else {
            remainingRiders.push_back(rider);
        }
    }

    unassigned = remainingRiders; // Update unassigned list
}

void outputResults(const std::vector<Driver>& drivers, const std::vector<Rider>& unassigned) {
    std::cout << "=== Assignment Results ===\n";

    int totalCapacityUsed = 0;
    int totalEmptySeats = 0;

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
        totalEmptySeats += driver.capacity;
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
    std::cout << "Total Empty Seats Remaining: " << totalEmptySeats << "\n";
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
            int capacity, groupID = -1;
            if (ss >> name >> capacity) {
                if (ss >> groupID) {
                    drivers.emplace_back(name, capacity, capacity, groupID);
                } else {
                    drivers.emplace_back(name, capacity, capacity);
                }
            } else {
                std::cerr << "Error: Invalid driver format in input file.\n";
                return 1;
            }
        } else {
            std::string name;
            int groupID = -1;
            ss >> name;
            if (ss >> groupID) {
                riders.emplace_back(name, groupID);
            } else if (!name.empty()) {
                riders.emplace_back(name);
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

    // Assign ungrouped riders
    assignUngroupedRiders(drivers, unassigned);

    // Output results
    outputResults(drivers, unassigned);

    return 0;
}
