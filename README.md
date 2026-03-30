# procmon 

## What is it? 

procmon (hereinafter referred to as the utility) is a cross-platform console utility intended for process monitoring. This utility collects information about running processes and displays the main process metrics, such as CPU usage, memory usage, and other important metrics. It is also worth noting the provided ability to configure the displayed and calculated metrics for your own tasks and goals.
It is planned to continue developing the utility further by adding new functions to it, such as creating logs, that is, recording all previous metrics over a certain period of time.

## Development

In future versions of the utility, it is planned to add: an interface for Terminal/BASH; a module for managing the utility settings. 

## Documentation

[Architecture](docs/architecture.md) 

## Installation 
```
git clone https://github.com/lefleen/procmon.git
cd procmon
cmake -S . -B
build cmake --build build
```
## License

Please see the file called [LICENSE](LICENSE). 

## Contacts 

GitHub: [lefleen](https://github.com/lefleen)
