import sys

class Driver:
    def __init__(self, name, capacity, initial_capacity, group_id=None):
        self.name = name
        self.capacity = capacity
        self.initial_capacity = initial_capacity
        self.group_id = group_id  # If None, driver does not have a specific group to pick up
        self.riders = []

    def calculate_fill_ratio(self):
        if self.initial_capacity == 0:
            return 1.0  # Prevent division by zero
        return 1.0 - (self.capacity / self.initial_capacity)

class Rider:
    def __init__(self, name, group_id=None):
        self.name = name
        self.group_id = group_id

def group_riders_by_group_id(riders):
    grouped_riders = {}
    for rider in riders:
        if rider.group_id not in grouped_riders:
            grouped_riders[rider.group_id] = []
        grouped_riders[rider.group_id].append(rider)
    return grouped_riders

def assign_grouped_riders_to_drivers(drivers, grouped_riders, unassigned):
    # First assign riders with specific group IDs to drivers with the same group ID
    for group_id, riders in grouped_riders.items():
        group_size = len(riders)

        # Handle ungrouped riders later in the `assign_ungrouped_riders` function
        if group_id is None:
            unassigned.extend(riders)
            continue

        # Try to find a driver with the same group ID and enough capacity
        driver = None
        for d in drivers:
            if d.group_id == group_id and d.capacity >= group_size:
                driver = d
                break

        # If no driver with matching group ID, find the first available driver with enough capacity
        if not driver:
            for d in drivers:
                if d.capacity >= group_size:
                    driver = d
                    break

        if driver:
            # Assign riders to this driver
            for rider in riders:
                driver.riders.append(rider.name)
            driver.capacity -= group_size
        else:
            # If no driver has enough capacity, add riders to unassigned
            unassigned.extend(riders)

def assign_ungrouped_riders(drivers, unassigned):
    remaining_riders = []
    for rider in unassigned:
        # Try to assign ungrouped riders to any available driver
        driver = min(
            (d for d in drivers if d.capacity > 0),
            key=lambda d: len(d.riders) if d.capacity > 0 else float('inf'),
            default=None
        )

        if driver:
            driver.riders.append(rider.name)
            driver.capacity -= 1
        else:
            remaining_riders.append(rider)

    return remaining_riders

def output_results(drivers, unassigned):
    total_capacity_used = 0
    total_empty_seats = 0
    print("\n")
    for driver in drivers:
        print(f"{driver.name}:")
        if not driver.riders:
            print("  No Riders Assigned")
        else:
            for rider in driver.riders:
                print(f"  {rider}")
            total_capacity_used += (driver.initial_capacity - driver.capacity)
        total_empty_seats += driver.capacity
        print()

    if unassigned:
        print("NULL:")
        for rider in unassigned:
            print(f"  {rider.name}")
        print()

    print("Summary:")
    print(f"  Total Capacity Used: {total_capacity_used}")
    print(f"  Total Empty Seats Remaining: {total_empty_seats}")
    print(f"  Total Riders Unassigned: {len(unassigned)}\n")

def main():
    if len(sys.argv) != 2:
        print(f"Usage: {sys.argv[0]} <input_file>")
        sys.exit(1)

    try:
        with open(sys.argv[1], 'r') as input_file:
            drivers = []
            riders = []
            unassigned = []
            reading_drivers = True

            for line in input_file:
                line = line.strip()

                if not line:
                    reading_drivers = False
                    continue

                parts = line.split()
                if reading_drivers:
                    name = parts[0]
                    capacity = int(parts[1])
                    group_id = int(parts[2]) if len(parts) > 2 else None
                    drivers.append(Driver(name, capacity, capacity, group_id))
                else:
                    name = parts[0]
                    group_id = int(parts[1]) if len(parts) > 1 else None
                    riders.append(Rider(name, group_id))

            # Group riders by their group ID
            grouped_riders = group_riders_by_group_id(riders)

            # Assign grouped riders to drivers based on matching group ID
            assign_grouped_riders_to_drivers(drivers, grouped_riders, unassigned)

            # Assign ungrouped riders (those with None group ID)
            unassigned = assign_ungrouped_riders(drivers, unassigned)

            # Output results
            output_results(drivers, unassigned)

    except FileNotFoundError:
        print(f"Error: Unable to open input file: {sys.argv[1]}")
        sys.exit(1)

if __name__ == "__main__":
    main()
