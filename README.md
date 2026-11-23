# MiniOS — Mini Operating System Project

## 1. Overview

MiniOS is a mini operating system project developed in C to demonstrate fundamental operating system concepts through practical implementations and experiments.

The project explores process management, synchronization, inter-process communication, and system calls.

## 2. Semaphores and Queue — Topic 7

This module focuses on synchronization mechanisms and concurrent programming using POSIX threads and semaphores.

### Implemented Features

- **Semaphore-based synchronization:** Uses POSIX named semaphores to coordinate access to shared resources.
- **Thread-safe circular queue:** Implements a bounded FIFO queue with synchronized producer and consumer operations.
- **Producer–consumer problem:** Demonstrates communication and synchronization between concurrent producer and consumer threads.
- **Race condition demonstration:** Illustrates data inconsistency caused by unsynchronized access to shared variables.
- **Priority inversion demonstration:** Illustrates how a lower-priority task holding a shared resource can delay a higher-priority task.

### Build Instructions

Requirements:
- GCC
- Make
- POSIX threads and semaphores
- Linux environment (or a compatible environment)

Build all demonstrations:

```bash
make clean
make
```

### Running the Demonstrations

Run each executable separately:

```bash
./queue_test
./semaphore_demo
./producer_consumer
./race_demo
./priority_inversion_demo
```

### Test Results

| Demonstration | Observed Result |
|---|---|
| Queue test | Successfully produced and consumed 10 jobs in FIFO order |
| Semaphore demo | Final counter matched the expected value of 500,000 |
| Producer–consumer | All 10 jobs were successfully processed |
| Race condition | Demonstrated lost updates; final counter was lower than 500,000 |
| Priority inversion | Successfully demonstrated the conceptual priority inversion sequence |

## 3. System Call Tracing — Topic 8 Practical Extension

Linux `strace` was used as an additional practical exercise to observe system calls made by the queue and multithreaded programs.

### Commands Used

Display a summary of system calls:

```bash
strace -c ./queue_test
```

Save the complete system-call trace:

```bash
strace -o queue_trace.txt ./queue_test
```

Trace thread creation and synchronization-related calls:

```bash
strace -f -e trace=clone,clone3,futex ./queue_test
```

Display a summary of the selected calls:

```bash
strace -f -c -e trace=clone,clone3,futex ./queue_test
```

### Observations

- `clone` and `clone3` were observed during thread creation.
- `futex` calls were observed during thread synchronization.
- System-call tracing provided insight into how the application interacts with the Linux kernel.

**Note:** This Linux `strace` exercise is a practical extension and is separate from the xv6 system-call implementation.

## 4. Technologies Used

- C Programming Language
- POSIX Threads (pthreads)
- POSIX Named Semaphores
- Linux
- GCC
- GNU Make
- Docker
- `strace`

## 5. Project Structure

```text
MiniOS/
├── examples/
│   ├── queue_test.c
│   ├── semaphore_demo.c
│   ├── producer_consumer.c
│   ├── race_demo.c
│   └── priority_inversion_demo.c
├── include/
│   └── minios.h
├── src/
│   ├── main.c
│   ├── monitor.c
│   └── sync.c
├── Makefile
└── README.md
```

*The project structure above highlights the main files associated with the demonstrations. Other project files may also be present.*

## 6. Contributors

MiniOS is a collaborative academic project. Individual modules are developed and integrated by team members.
