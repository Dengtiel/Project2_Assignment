# CProject2_Assignment — C Programming Projects

**Student Email:** d.akol@alustudent.com
**Language:** C
**Compiler:** GCC

---

## Project Overview

| Project | Title | Custom Feature |
|---------|-------|----------------|
| 1 | Dynamic Student Record Engine | Grade Stability Score |
| 2 | Adaptive Text Intelligence Tool | Average Word Length |
| 3 | Callback-Based Device Monitoring Simulator | Fault Detector Callback |

---

## Project 1 — Dynamic Student Record Engine

### Description
A dynamic student record system that stores, processes, and analyzes academic records. The program uses structures, dynamic memory allocation, pointer traversal, and function pointers to manage a dataset of 15 students.

### Concepts Demonstrated
- `struct` with id, name, and grade fields
- Dynamic memory using `malloc`, `realloc`, and `free`
- Function pointers via a menu dispatch table
- Pointer traversal of the student array
- String operations using `strcpy` and `strcmp`

### Custom Analysis Function
**Grade Stability Score** — calculates the standard deviation of all grades and converts it into a 0–100 stability score. A high score means grades are tightly clustered; a low score means high variation across the class.

### How to Compile and Run
```bash
cd project1
gcc -Wall -o student_records project1_student_records.c
./student_records
```

### Menu Options
```
1. Add Student
2. Display Students
3. Sort Students
4. Calculate Average
5. Find Top Student
6. Grade Stability Score
0. Exit
```

---

## Project 2 — Adaptive Text Intelligence Tool

### Description
A text analysis tool that dynamically processes a paragraph entered by the user or pre-loaded from a source text. The program splits text into words using pointer traversal and computes various statistics.

### Source Text
Opening paragraph from *The Alchemist* by Paulo Coelho.

### Concepts Demonstrated
- Dynamic memory allocation for text and word arrays
- Pointer-based tokeniser (no `strtok`)
- `struct wordStat` for tracking word frequency
- `strlen`, `strcmp`, `strcpy`, `strncpy` all used explicitly
- Memory addresses of key data structures printed to output
- Function pointers for all analysis functions

### Custom Analysis Function
**Average Word Length** — sums the length of every word in the text and divides by the total word count to give the mean number of characters per word.

### How to Compile and Run
```bash
cd project2
gcc -Wall -o text_tool project2_text_tool.c
./text_tool
```

### Menu Options
```
1. Word Count
2. Longest Word
3. Most Frequent Word
4. Average Word Length
5. Enter custom text
0. Exit
```

---

## Project 3 — Callback-Based Device Monitoring Simulator

### Description
A device monitoring simulator where different devices send readings that are processed using callback functions passed as function pointers. The program simulates 10 random device readings on startup.

### Device Types
| Type | Reading Unit | Alert Thresholds |
|------|-------------|-----------------|
| Temperature Sensor | Degrees Celsius | > 80°C overheat, < -10°C freeze |
| Pressure Sensor | Kilopascals (kPa) | > 200 kPa high, < 50 kPa low |
| Voltage Monitor | Volts (V) | > 13.5V over, < 10.5V low battery |

### Concepts Demonstrated
- `struct Device` containing a `union` for different reading types
- Dynamic memory allocation for the device array
- Callback functions via `void (*callback)(struct Device *)`
- `process_device()` dispatcher accepting a function pointer
- Pointer traversal for device initialisation and sweeps

### Custom Callback Function
**fault_detector** — evaluates each device reading and assigns a severity level: OK, WARNING, or CRITICAL. Prints a unified fault report for all devices regardless of type.

### How to Compile and Run
```bash
cd project3
gcc -Wall -o device_monitor project3_device_monitor.c
./device_monitor
```

### Menu Options
```
1. Run another 10 random readings
2. Fault Detector sweep
3. Show all device names
0. Exit
```

---

## Compiling All Projects at Once

From the root `CSC_Assignment` folder:

```bash
gcc -Wall -o project1/student_records project1/project1_student_records.c && echo "Project 1 OK"
gcc -Wall -o project2/text_tool        project2/project2_text_tool.c        && echo "Project 2 OK"
gcc -Wall -o project3/device_monitor   project3/project3_device_monitor.c   && echo "Project 3 OK"
```

Expected output:
```
Project 1 OK
Project 2 OK
Project 3 OK
```

---

## Project Structure

```
CSC_Assignment/
├── README.md
├── project1/
│   └── project1_student_records.c
├── project2/
│   └── project2_text_tool.c
└── project3/
    └── project3_device_monitor.c
```

---

## Requirements

### Ubuntu / Linux
- GCC compiler — install with `sudo apt install gcc`
- Git — install with `sudo apt install git`
- VS Code — install with `sudo snap install code --classic`

### Windows
- Install **MinGW-w64** (provides GCC for Windows) from https://winlibs.com/
  - Download the zip, extract to `C:\mingw64`
  - Add `C:\mingw64\bin` to your system PATH: Search "Environment Variables" → Edit System Environment Variables → Path → New → `C:\mingw64\bin`
- Install **Git for Windows** from https://git-scm.com/downloads
- Install **VS Code** from https://code.visualstudio.com/
- Compile and run using the same `gcc` commands in the VS Code terminal or Git Bash:
  ```bash
  gcc -Wall -o student_records project1_student_records.c
  student_records.exe
  ```

### All Platforms
- No external libraries required — standard C only (`stdio.h`, `stdlib.h`, `string.h`, `time.h`, `ctype.h`)
