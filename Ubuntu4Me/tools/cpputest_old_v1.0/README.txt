1. Configurations
   In ./build/defs.mk file, you should modify CPPUTEST_HOME according to your own system environment. 
   e.g. In my personal PC, CPPUTEST_HOME = ~/majun/Dev/git/cpputest
        In qdbuild3, CPPUTEST_HOME = ~/sandbox/Main/TelicaRoot/components/UT/CppUTest
   
2. Directories
cpputest
├── bin 
│   ├── AllTest.c
│   ├── AllTest.h
│   └── makefile
├── build
│   ├── defs.mk
│   └── rules.mk
├── makefile
├── src 
│   ├── hello.c
│   ├── hello.h
│   └── makefile
└── Test 
    ├── kavon.h
	├── kavonTest.c: basic example
	├── kavonTestMock.c: mock example
	└── makefile
(1) bin: test binary location
(2) build: defines and rules
(3) src: Please add your source code here 
(4) Test: Please add your test case here

3. Access link:
https://github.com/kavonm/Ubuntu4Me/tree/master/tools/cpputest

4. How to use it?
--  make
--  make clean
