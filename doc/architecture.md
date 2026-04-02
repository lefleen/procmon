# procmon

This document is intended to provide an overview of the utility’s structure and logic. It is aimed at people who want to study the utility, participate in its modification, improve it, or otherwise work with it.

All functional parts listed below are referred to as **modules**. When the term **"module"** is used, it refers to the functional part described in the corresponding subsection.

## ManageProgramm

*A module implemented as a namespace.*

It is the core of the utility. It is responsible for organizing the interaction between all other modules. It contains the following functional parts:
1. thread management;
2. cleanup of residual resources of processes that have already terminated;
3. calling the procedure that updates the data of each process;
4. calling the CPU usage calculation function for a **DataProcess** class object.

## DataProcess

*A module implemented as a class.*

It serves as a container for process metrics. It contains the main metrics of a process: PID, memory usage, CPU usage, and others, as well as a function intended to update the metrics of the current class object.

## LinuxLogic / WindowsLogic

*Modules implemented as two separate namespaces, one for each OS.*

They contain concrete implementations for Windows and Linux systems. These modules implement the logic for obtaining process metrics, except for the logic related to **UsingCpu**.

## macro

*A module implemented as a separate file; it does not contain classes or namespaces.*

It contains macros and global program variables used as flags for various calculations and metric retrieval operations.

## ProcessDescriptorRAII

*A module implemented as a class.*

An RAII wrapper for process descriptors in Windows and file descriptors in Linux. It is used to prevent resource leaks when working with these entities.

## SharedSpaceLinux

*A module implemented as a namespace.*

It contains functional blocks for Linux systems that are used by other Linux-related modules throughout the program.

## types

*A module implemented as a separate file; it does not contain classes or namespaces and includes only one structure.*

It contains type definitions for different platforms. It helps maintain cross-platform compatibility.

## UsingCpu

*A module implemented as a class.*

It organizes the calculation of CPU usage metrics both for a specific time interval and for the entire lifetime. It stores state between calls based on time-related data.

> The first call is used for initialization.