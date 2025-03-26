# Flight Management and Reservation System

This package contains two related projects developed in C.

## Project 1 – Flight Management System
- **Purpose:**  
  Develop a system to manage airports and flights.
- **Key Features:**  
  - Add and list airports (each with a 3-letter uppercase ID, country, and city).
  - Add and list flights (identified by a flight code with two uppercase letters and 1–4 digits, with specified departure/arrival times and dates).
  - Simulate system date advancement.
- **Constraints:**  
  - Maximum of 40 airports and 30,000 flights.
  - Strict input/output formatting and error handling.
- **Compilation:**  
  ```sh
  gcc -Wall -Wextra -Werror -ansi -pedantic -o proj1 *.c
  ```

## Project 2 – Extended Flight Management with Reservations
- **Purpose:**  
  Extend Project 1 by adding reservation management and deletion functionalities.
- **Key Features:**  
  - All functionalities from Project 1.
  - Add and list flight reservations (with unique reservation codes and passenger numbers).
  - Delete flights (along with their reservations) or individual reservations.
- **Constraints:**  
  - No limits on the number of reservations.
  - Dynamic memory allocation for reservations, with controlled memory usage.
  - Adherence to strict formatting and error reporting.
- **Compilation:**  
  ```sh
  gcc -Wall -Wextra -Werror -ansi -pedantic -o proj2 *.c
  ```

## General Notes
- **Input/Output:**  
  Both projects interact via a command-driven interface using standard input/output.  
- **Submission:**  
  Source files should be packaged in a `.zip` file and submitted as per the course guidelines.  
- **Evaluation:**  
  Grading is based on functionality, code quality, and adherence to the provided specifications.
