# **DriveSync** 🚗✨  
Efficient Ride-to-Driver Assignment System  

## **Overview**  
DriveSync is a smart logistics tool designed to optimize rider-to-driver assignments for carpooling and ride-sharing. It ensures maximum seat utilization and minimizes unassigned riders through intelligent group prioritization and flexible handling of both grouped and ungrouped riders.  

This repository contains:  
- A **Python implementation** for fast prototyping and testing.  
- A **C++ implementation** optimized for performance and scalability.  

## **Features**  
- 🚦 **Group Prioritization**: Matches grouped riders with drivers to maintain group integrity.  
- 🧠 **Dynamic Assignment**: Allocates ungrouped riders based on driver availability and capacity.  
- 📊 **Efficiency Metrics**: Outputs statistics on total capacity used, empty seats, and unassigned riders.  

## **How It Works**  
1. **Input**:  
   - A text file specifying drivers and riders, with optional group IDs.  
   - Example:  
     ```txt
     # Drivers
     Driver1 4 1
     Driver2 3
     
     # Riders
     RiderA 1
     RiderB
     RiderC 1
     RiderD
     ```  

2. **Process**:  
   - Assigns grouped riders to matching drivers based on capacity.  
   - Allocates ungrouped riders to minimize unassigned riders and maximize seat utilization.  

3. **Output**:  
   - Displays driver assignments, unassigned riders, and summary statistics.  

## **File Structure**  
- `DriveSync.py`  
  - Python implementation of the project.  
  - Easy to modify and extend for testing or educational purposes.  

- `DriveSync.cpp`  
  - High-performance C++ implementation for handling large-scale datasets.  
  - Optimized for speed and memory usage.  

## **How to Run**  

### **Python Version**  
1. Ensure Python 3 is installed.  
2. Run the script with an input file:  
   ```bash
   python3 DriveSync.py input.txt
   ```  

### **C++ Version**  
1. Compile the program using a C++ compiler (e.g., `g++`):  
   ```bash
   g++ -o DriveSync DriveSync.cpp
   ```  
2. Run the compiled executable with an input file:  
   ```bash
   ./DriveSync input.txt
   ```  

## **Example Output**  
```txt
Driver1:
  RiderA
  RiderC

Driver2:
  RiderB
  RiderD

Summary:
  Total Capacity Used: 4
  Total Empty Seats Remaining: 3
  Total Riders Unassigned: 0
```  

## **Contributing**  
Contributions are welcome! Feel free to open issues or submit pull requests to improve functionality or documentation.  

## **License**  
This project is licensed under the MIT License.
