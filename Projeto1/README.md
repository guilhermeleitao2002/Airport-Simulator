# Flight Management System

## Project Overview
This project involves developing a **Flight Management System** that allows users to manage airports and flights through a set of predefined commands. The system operates through a text-based interface where users can add, list, and query flights and airports.

## Features
- **Airport Management:**
  - Add new airports with unique identifiers.
  - List all airports in the system.
- **Flight Management:**
  - Add new flights with details such as departure/arrival airports, date, time, and passenger capacity.
  - List all flights or filter by departure/arrival airport.
- **System Date Management:**
  - The system keeps track of a simulation date.
  - Users can manually advance the date.

## Commands
The program accepts the following commands via standard input:

| Command | Description |
|---------|-------------|
| `q` | Terminates the program. |
| `a <ID> <country> <city>` | Adds a new airport. |
| `l [<ID> ...]` | Lists all airports or specified ones. |
| `v [<flight_code> <departure> <arrival> <date> <time> <duration> <capacity>]` | Adds a new flight or lists all flights. |
| `p <airport_ID>` | Lists flights departing from a given airport. |
| `c <airport_ID>` | Lists flights arriving at a given airport. |
| `t <date>` | Advances the system date. |

## Constraints
- Airports have unique 3-letter uppercase IDs and are limited to **40 airports**.
- Flights have unique codes (two uppercase letters followed by 1-4 digits) and are limited to **30,000 flights**.
- The system operates within the date range **01-01-2022 to 31-12-2023**.
- Flight duration cannot exceed **12 hours**.
- Passenger capacity ranges from **10 to 100**.

## Input/Output Format
- **Dates:** `DD-MM-YYYY`
- **Times:** `HH:MM`
- **Output Formatting:** Must strictly follow the provided format.

## Compilation & Execution
- Compile using:
  ```sh
  gcc -Wall -Wextra -Werror -ansi -pedantic -o proj1 *.c
  ```
- Run the program:
  ```sh
  ./proj1 < test.in > test.myout
  ```
- Compare output with expected results:
  ```sh
  diff test.out test.myout
  ```

## Submission Instructions
- Submit a `.zip` file containing all `.c` and `.h` files.
- The final submission will be used for grading.
- Automated testing will be performed, ensuring strict adherence to the expected input/output format.

## Development Guidelines
- Develop incrementally and test each feature separately.
- Ensure proper formatting, no trailing spaces, and strict adherence to input/output rules.
- Avoid using the `qsort` function.
- The only allowed libraries are:
  - `stdio.h`
  - `stdlib.h`
  - `ctype.h`
  - `string.h`

## Evaluation Criteria
- **Functionality (0-16 points):** Based on automated testing.
- **Code Quality (-4 to +4 points):** Based on structure, readability, comments, and modularity.
- **Strict Formatting Compliance:** Incorrect formatting can result in a score of zero.

## Notes
- The provided test cases are **not exhaustive**. Additional tests may be used during evaluation.
- The system will not provide feedback on hidden test cases.

For more details, refer to the project documentation.

