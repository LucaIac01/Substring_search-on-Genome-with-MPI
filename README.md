# Substring Search on Genome using MPI

This project implements a parallel substring search algorithm using MPI (Message Passing Interface) to identify patterns in genomic sequences. It also includes a serial implementation for performance comparison.

## 📁 Project Structure

```
.
├── MPI/
│   └── Advanced Computer Architecture/
│       ├── parallel2.c               # Main parallel implementation
│       ├── Libraries/
│           ├── rabinkarp.c/h         # Rabin-Karp string search algorithm
│           └── utilities.c/h         # Utility functions
├── Serial/
│   └── Advanced Computer Architecture/
│       └── serial.c                  # Serial implementation
├── report.pdf                        # Final project report
└── README.md
```

## 🚀 How to Run

### Requirements

- MPI implementation (e.g., [OpenMPI](https://www.open-mpi.org/))
- GCC or compatible C compiler

### Compilation

#### Parallel version

```bash
mpicc -o parallel_search parallel2.c Libraries/rabinkarp.c Libraries/utilities.c
```

#### Serial version

```bash
gcc -o serial_search serial.c ../MPI/Advanced\ Computer\ Architecture/Libraries/rabinkarp.c ../MPI/Advanced\ Computer\ Architecture/Libraries/utilities.c
```

### Execution

#### Run parallel version

```bash
mpirun -np 4 ./parallel_search pattern.txt TestGenome.txt
```

#### Run serial version

```bash
./serial_search pattern.txt text_genome.txt
```

## 📊 Features

- Uses the **Rabin-Karp** algorithm for efficient string matching.
- Designed for **performance benchmarking** between serial and parallel processing.
- Handles large genomic datasets.

## 📄 Report

For a detailed explanation of the algorithm, performance metrics, and architecture design, see [report.pdf](./report.pdf).

## 🧠 Authors

- *Luca Iacopino - Calogero Bugia*

## 📜 License

This project is licensed under the MIT License. See `LICENSE` for details.
