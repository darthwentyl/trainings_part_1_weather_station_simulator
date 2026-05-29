# Exercise
Please add a new process to the implementation from exercise **tr1_2**. The process should open a socket to listening for external command and receive data which the user wants to take.

Supporting command:
* ```temperature``` - return last 10 temperature measurements
* ```pressure``` - return last 10 pressure measurements
* ```temperature <n>``` - return last n temperature measurements
* ```pressure <n>``` - return last n pressure measurements
* ```help``` - show available commands
* ```exit``` - disconnect


## Implementation
The application should support next feature
* Multi-process logging system
* Write message in format: temperature: _24.57[C], pressure: 996.57[hPa]_
* Measurement precision to two digits
* Save in to file last 10 measurements into file. Temperature and pressure in separate files.
* Own exceptions implementation
* Support for pipe stream communication
* Visualization by gnuplot
* External user command listener used by socket

## Requirements
* Modern C++
* Object-oriented programming - OOP
* Unit tests - use google test framework
* Coverage - use gcov
* Memory leak - use valgrind
* Shared memory segment controlled by V-system

# Building
Prepare build system using cmake.

## Release
Release version configured with by ser **RELEASE** flags
```
mkdir build
cd build
cmake .. -DRELEASE=1
make j8
./bin/weather
```

## Debug
Debug version configured by default
```
mkdir build
cd build
cmake ..
make -j8
./bin/weather
```

## Unit tests
Release version configured with by ser **UT** flags
```
mkdir build
cd build
cmake .. -DUT=1
make -j8
./test/weather_tests
```

## Test coverage
Prepare a bash script which prepares the test coverage report. Let’s launch the script by command below
```
./coverage.sh
```

# Links
* https://en.wikipedia.org/wiki/Reverse_Polish_notation
* https://en.wikipedia.org/wiki/Object-oriented_programming
* https://google.github.io/googletest/
* https://valgrind.org/docs/manual/index.html
* https://gcc.gnu.org/onlinedocs/gcc/Gcov.html
* https://en.wikipedia.org/wiki/UNIX_System_V
* http://www.gnuplot.info
* https://www.man7.org/linux//man-pages/man2/pipe.2.html

# Notes
Valgrind error still reachable https://github.com/google/googletest/pull/4919
