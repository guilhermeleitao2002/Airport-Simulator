# Flight Management and Reservation System

## Overview

This project extends the functionalities developed in Project 1. In addition to managing airports and flights, this version introduces dynamic reservation management and deletion operations. The system is implemented in C and interacts with the user through a command-driven interface.

## Features

- **Airport Management:**  
  - Add new airports with a 3-letter uppercase identifier.  
  - List all airports or a subset specified by the user.

- **Flight Management:**  
  - Add flights with details such as flight code, departure/arrival airports, date, time, duration, and capacity (with no upper limit on passengers per flight anymore).  
  - List all flights, or filter flights by departure (command `p`) or arrival (command `c`).

- **Date Management:**  
  - The system simulates time by maintaining a current date (initially set at 01-01-2022).  
  - The user can advance the date using the corresponding command.

- **Reservation Management:**  
  - **Add/List Reservations (command `r`):**  
    - Add a new reservation for a given flight, or list all reservations for a flight.  
    - Reservations include a unique reservation code and the number of passengers.
  - **Deletion (command `e`):**  
    - Remove flights or reservations.  
    - When deleting a flight, all associated reservations are also removed.

## Command Summary

The system reads commands from the standard input. All commands are provided as a line starting with a command keyword followed by the necessary parameters. The commands include:

| Command | Action                                                                                           | Format (Input/Output)                                                                                   |
|---------|--------------------------------------------------------------------------------------------------|---------------------------------------------------------------------------------------------------------|
| `q`     | Terminates the program.                                                                          | **Input:** `q` <br> **Output:** None                                                                   |
| `a`     | Adds a new airport.                                                                              | **Input:** `a <IDAirport> <country> <city>` <br> **Output:** `airport <IDAirport>`                      |
| `l`     | Lists airports.                                                                                  | **Input:** `l [<IDAirport> ...]` <br> **Output:** `<IDAirport> <city> <country> #flights` per line       |
| `v`     | Adds a new flight or lists all flights.                                                          | **Input:** `v [<flight_code> <departure> <arrival> <date> <time> <duration> <capacity>]`                |
| `p`     | Lists flights departing from a specific airport.                                               | **Input:** `p <IDAirport>` <br> **Output:** `<flight_code> <arrival> <date> <time>` per line             |
| `c`     | Lists flights arriving at a specific airport.                                                  | **Input:** `c <IDAirport>` <br> **Output:** `<flight_code> <departure> <dateArrival> <timeArrival>`       |
| `t`     | Advances the current date in the system.                                                       | **Input:** `t <date>` <br> **Output:** `<date>` (the new system date)                                   |
| `r`     | Adds a new reservation or lists reservations for a flight.                                     | **Input:** `r <flight_code> <date> [<reservation_code> <passenger_number>]` <br> **Output:** `<reservation_code> <passenger_number>` per reservation, in lexicographic order by reservation code |
| `e`     | Deletes a flight or a reservation.                                                             | **Input:** `e <code>` <br> **Output:** Removes the flight (and associated reservations) or the specified reservation |

## Constraints and Format Details

- **Input/Output Formats:**  
  - Dates are represented as `DD-MM-YYYY` (e.g., `01-01-2022`).  
  - Times use the `HH:MM` format (e.g., `09:05`).
- **Identifiers:**  
  - Airports: Exactly three uppercase letters.  
  - Flights: Two uppercase letters followed by 1 to 4 digits (leading zeros are not allowed in the numeric part).  
  - Reservations: Must be composed solely of uppercase letters and digits and have at least 10 characters.
- **Limits:**  
  - There is no limit on the number of reservations or on the size of reservation codes.  
  - The memory for reservations should be allocated dynamically, using only the memory strictly required.
- **Error Handling:**  
  - The system must report the first error encountered per command (e.g., invalid flight code, non-existent airport, duplicate reservation code, etc.).  
  - If the system runs out of memory, it should print `No memory.` before terminating gracefully after releasing all allocated memory.

## Compilation and Execution

- **Compilation:**  
  Compile using the following command:
  ```sh
  gcc -Wall -Wextra -Werror -ansi -pedantic -o proj2 *.c
  ```
  The compiler must not emit any warnings or produce output during compilation.

- **Execution:**  
  Run the program using:
  ```sh
  ./proj2 < test.in > test.myout
  ```
  You can verify your output by comparing it to the expected output with:
  ```sh
  diff test.out test.myout
  ```

## Submission Guidelines

- **Submission Package:**  
  Submit a `.zip` file containing all your source files (`.c` and `.h`).  
  - If your project consists of a single source file, the zip should contain that file only.  
  - For multi-file projects, ensure that all necessary files are included.
- **Upload:**  
  Upload the zip file via the project submission system.  
  **Note:** Submissions from the same student must be at least 10 minutes apart.

## Evaluation Criteria

The project is evaluated based on:

1. **Functionality (0-16 points):**  
   - Correct implementation of the commands and error handling.
2. **Code Quality (-4 to +4 points):**  
   - Clarity, proper commenting, indentation, modularity, dynamic memory management, and code organization.
3. **Automated Testing:**  
   - The code will be tested on a GNU/Linux system with limited memory and execution time.  
   - Passing the provided tests does not guarantee full correctness, as additional hidden tests may be applied.
4. **Tools:**  
   - Tools such as lizard, valgrind, and fsanitize may be used to check for code complexity and memory leaks.

## Additional Notes

- Only the standard libraries `stdio.h`, `stdlib.h`, and `string.h` are allowed.
- Ensure that your program strictly follows the input and output formats, including spacing and line breaks.
- Incremental development and thorough testing are recommended to avoid common pitfalls and ensure correct behavior before final submission.
