# WebserverParser

## Compilation Instructions

### Machine Requirements

- Linux System (Ubuntu 24.04 for ex.)
- Basic build dependencies:
```
sudo apt install build-essential cmake libgtest-dev
```

### Build Instructions

```
$ mkdir build
$ cd build
$ cmake ..
$ make
```

Note: if you observe errors related to GTEST_LIBRARY / GTEST_INCLUDE_DIR / GTEST_MAIN_LIBRARY

You may need to compile GoogleTest locally: https://stackoverflow.com/questions/24295876/cmake-cannot-find-googletest-required-library-in-ubuntu

## Usage Instructions

To run the main webserver parser binary:
```
$ ./logParser

Required arguments:
  --action=webserver OR --action=resource
    =webserver: provides number of accesses to webserver per host
    =resource: provides number of successful resource accesses by URI
  --filepath=<path_to_webserver_logs>

Optional arguments:
 --minimum_date=[DD:HH:MM:SS]
 --maximum_date=[DD:HH:MM:SS]
   NOTE: minimum_date and maximum_date are used in conjunction
   to filter logs between the two date times

Sample usage:
./logParser --action=webserver --file=../tests/hostAccessTest_small.txt
  (Request number of accesses to webserver per host, using provided log file
./logParser --action=resource --file=../tests/resourceAccessTest_small.txt --minimum_date=[29:23:53:27] --maximum_date=[29:23:54:18]
  (Request number of successful resource accesses by URI, ranging between provided date times, using provided log file
```

To run GoogleTest automated tests, execute the test_webServerAnalyser binary:
```
$ ./test_webServerAnalyser
[==========] Running 6 tests from 2 test suites.
[----------] Global test environment set-up.
[----------] 3 tests from hostAccessTest
[ RUN      ] hostAccessTest.smallFile_noDateTime_test
[       OK ] hostAccessTest.smallFile_noDateTime_test (1 ms)
[ RUN      ] hostAccessTest.smallFile_invalidDateTime_test
[       OK ] hostAccessTest.smallFile_invalidDateTime_test (1 ms)
[ RUN      ] hostAccessTest.smallFile_dateTimeRange_test
[       OK ] hostAccessTest.smallFile_dateTimeRange_test (0 ms)
[----------] 3 tests from hostAccessTest (3 ms total)

[----------] 3 tests from resourceAccessTest
[ RUN      ] resourceAccessTest.smallFile_noDateTime_test
[       OK ] resourceAccessTest.smallFile_noDateTime_test (0 ms)
[ RUN      ] resourceAccessTest.smallFile_invalidDateTime_test
[       OK ] resourceAccessTest.smallFile_invalidDateTime_test (1 ms)
[ RUN      ] resourceAccessTest.smallFile_dateTimeRange_test
[       OK ] resourceAccessTest.smallFile_dateTimeRange_test (1 ms)
[----------] 3 tests from resourceAccessTest (3 ms total)

[----------] Global test environment tear-down
[==========] 6 tests from 2 test suites ran. (6 ms total)
[  PASSED  ] 6 tests.
```
