# LLVM
Some basic string operations written in LLVM assembly language.

## strops
Per course requirements, implement the following string operations using LLVM assembly.

### strlen
Return the length of a null terminated character string.

### strcmp
Compate two null terminated character strings for equality.

### strcpy
Copy a null terminated character string.

## Build
To build, run the `build.sh` script.

## Run
Binaries will be build into the `build/` folder. From there, each binary executed with appropriate arguments:

### strlen
Args - A string, wrapped in quotes. e.g. `strlen "some string to check length"`.

### strcmp
Args - Two strings, wrapped in quotes. e.g. `strcmp "string1" "string2"`.

### strcpy
Args - A string, wrapped in quotes. e.g. `strcpy "some string to copy"`.
