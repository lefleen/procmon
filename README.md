# procmon 

## What is it? 

procmon is a cross-platform console utility intended for process monitoring. This utility collects information about running processes and displays the main process metrics, such as CPU usage, memory usage, and other important metrics.

## Documentation

[Architecture](docs/architecture.md) 

## Installation 

### Supported platforms

- Microsoft Windows
- Linux

### Requirements
- CMake
- C++ compiler

### Compiling
```
git clone https://github.com/lefleen/procmon.git
cd procmon
cmake -S . -B build
cmake --build build
```
**Add the directory containing the executable file to your `PATH`.**

## Usage
```bash
procmon help
procmon help get
procmon help set
procmon get <metric>
```
### Available metrics:
- all
- name
- time
- memory
- iCPU
- tCPU
- time_view
- memory_view

```bash
procmon set <metric> <setting>
```

### Available metrics:
- name
- time
- memory
- iCPU
- tCPU
- time_view
- memory_view

### Available boolean settings:
- on
- off

### Available view settings:
#### time_view:
seconds
minutes
hours
days
#### memory_view:
- B
- KiB
- MiB
- GiB
- TiB

Examples:
```bash
procmon get name

procmon set name off
procmon set time_view seconds
```

## License

Please see the file called [LICENSE](LICENSE). 

## Contacts 

GitHub: [lefleen](https://github.com/lefleen)
