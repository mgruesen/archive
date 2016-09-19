#!/bin/bash

if [ -z "$LLC" ] 
then
  echo "Error. Shell variable LLC not set. Please set this variable to your LLC compiler"
  exit 1
elif [ -z "$CLANG" ]
then
  echo "Error. Shell variable CLANG not set. Please set this variable to your CLANG compiler"
  exit 1
fi

# Clear old build folder
rm -rf build/
mkdir build

cd src/
# Build all the files in the src dir
for source in *.ll
do  
  asm="${source%%.*}.s"
  binary="../build/${source%%.*}"
  echo "Compiling $source to $asm"
  `$LLC $source`
  echo "Compiling $asm to $binary"
  `$CLANG $asm -o $binary`
  echo "Exectuable $binary created"
done
cd ..