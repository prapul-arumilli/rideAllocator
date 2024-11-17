# Driver-Rider Assignment Tool

## Overview
This tool helps assign riders to drivers based on the available capacity of the drivers. Riders are grouped by their group ID (if any), and the tool tries to balance the distribution of riders across drivers to ensure maximum efficiency. The unassigned riders are also handled and shown at the end of the results.

## Features
- **Assign riders to drivers** based on available capacity.
- **Group riders by their group ID** to ensure that riders in the same group are assigned to drivers together.
- **Balance rider distribution** to prevent overloading any single driver.
- **Output a summary** of driver assignments, including unassigned riders and available seats.

## Requirements
- C++ Compiler (e.g., `g++`)
- Standard C++ Library

## Usage

1. **Input File Format**:
   The input should be in a text file, structured as follows:
   
   - **Driver Section**:
     Each driver is defined by their name, capacity, and optional group ID. This section should be followed by a blank line.
     - Example:
       ```
       Driver1 4
       Driver2 3 1
       Driver3 5
       ```
     In this example:
     - `Driver1` has a capacity of 4 and no group ID.
     - `Driver2` has a capacity of 3 and belongs to group ID `1`.
     - `Driver3` has a capacity of 5 and no group ID.

   - **Rider Section**:
     Each rider is defined by their name and optional group ID. Riders without a group ID are treated as ungrouped.
     - Example:
       ```
       Rider1 1
       Rider2
       Rider3 2
       Rider4 1
       ```
     In this example:
     - `Rider1` belongs to group `1`.
     - `Rider2` is ungrouped.
     - `Rider3` belongs to group `2`.
     - `Rider4` belongs to group `1`.

2. **Running the Program**:
   Compile the program using a C++ compiler. For example:
   ```bash
   g++ driver_rider_assignment.cpp -o driver_rider_assignment
   ```

   Run the program with the input file as a command-line argument:
   ```bash
   ./driver_rider_assignment input.txt
   ```

   Replace `input.txt` with the path to your input file.

3. **Output**:
   The program will output the assignment results to the console, showing:
   - The drivers with their assigned riders.
   - Unassigned riders (if any).
   - A summary showing the total capacity used, the empty seats remaining, and the number of unassigned riders.

   Example output:
   ```
   === Assignment Results ===
   Driver [Driver1]:
     Rider [Rider1]
     Rider [Rider2]

   Driver [Driver2]:
     Rider [Rider3]

   Driver [Driver3]:
     Rider [Rider4]

   Driver [NULL]:
     Rider [Rider5]

   === Summary ===
   Total Capacity Used: 8
   Total Empty Seats Remaining: 7
   Total Riders Unassigned: 1
   ```

   In this example:
   - `Driver1` is assigned `Rider1` and `Rider2`.
   - `Driver2` is assigned `Rider3`.
   - `Driver3` is assigned `Rider4`.
   - `Rider5` is unassigned and appears under the `NULL` driver.

## Customization
You can modify the program to:
- Handle additional constraints or specific logic for assigning riders to drivers.
- Customize the output format if you need to integrate this tool with other software.

## Troubleshooting
- Ensure the input file is formatted correctly (with drivers and riders separated by a blank line).
- The program handles both grouped and ungrouped riders. If riders are unassigned, they will be listed under the `NULL` driver.
- If the file format is incorrect or missing data, the program will output an error message.

## License
This project is open-source and free to use. Feel free to modify it according to your needs.
