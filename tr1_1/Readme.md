# Build
- release
    ```
    mkdir build && cd build && cmake .. -DRELEASE=1
- debug
    ```
    mkdir build && cd build && cmake ..
- unit tests
    ```
    mkdir build && cd build && cmake .. -DUT=1

valgrind error still reachable:
https://github.com/google/googletest/pull/4919
