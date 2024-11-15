# roman-numeral-calculator

This repository implements the
[Roman Numeral Calculator](http://codingdojo.org/cgi-bin/index.pl?KataRomanCalculator) kata,
with the added feature of also implementing subtraction.

It is intended to be built on a Linux platform, such as Ubuntu 24.04.

From a base of desktop Ubuntu, the following command will install the necessary dependencies:

```
sudo apt-get install check
```

After installing dependencies, you can build and execute tests by typing the following command
in the project directory:

```
make
```

To perform static analysis on the code, you can additionally install clang, which includes the
clang static analyzer tool:

```
sudo apt-get install clang clang-tools
scan-build make
```

I've left an example bug in the code that the static analyzer will find.  When analysis is complete,
you will be given a command line by which you can examine the results, such as:

```
scan-view /tmp/scan-build-2016-09-10-210356-21567-1
```

Just enter that command, and it will open a browser window for you navigate.  If there are no errors,
you will not be given such a command, as there is no report to examine.

Also, to measure test coverage, you will need to enter the following commands:

```
sudo apt-get install lcov
make coverage_report
```

This will measure test coverage of the tests, and then open a web browser with a report of the
measured test coverage.


And, you can do further checks with clang-tidy:

```
sudo apt-get install clang-tidy
make tidy
```

This will do buffer overflow security checks, for example.
