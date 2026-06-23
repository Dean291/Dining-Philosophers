# 🍽️ Dining Philosophers Problem: IPC & Synchronization

## 📌 Abstract
This repository contains a comprehensive implementation and analysis of the classic **Dining Philosophers Problem**, originally formulated by Edsger W. Dijkstra. Developed as part of an Operating Systems coursework, this project demonstrates advanced concepts in Inter-Process Communication (IPC), thread synchronization, and resource allocation using POSIX threads (`pthreads`) in a Linux environment.

## 🎯 Core Objectives
- **Concurrency Management:** Practical application of `pthread_mutex` and `pthread_cond` to manage concurrent thread executions safely.
- **Deadlock Simulation:** Intentional induction of a system deadlock to illustrate the four Coffman conditions (Mutual Exclusion, Hold and Wait, No Preemption, Circular Wait).
- **Deadlock Resolution & Starvation Prevention:** Implementation of an Arbitrator (Monitor/State Array) pattern to dynamically govern resource allocation, ensuring system progression and strict fairness.
- **Performance Profiling:** Microsecond-level telemetry to track waiting times and empirically validate the absence of thread starvation.

## 📂 Repository Architecture

```text
.
├── dining_deadlock.c      # Naive implementation demonstrating circular wait & system deadlock
├── dining_waiter.c        # Optimized Arbitrator pattern ensuring fairness and deadlock-freedom
├── Report.pdf             # In-depth architectural and statistical analysis (based on Tanenbaum's Modern OS)
└── README.md              # Project documentation
