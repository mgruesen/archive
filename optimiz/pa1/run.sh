#!/bin/bash
if [ "$#" -ne 1 ] 
  then
  echo "Invalid number of arguments"
  exit 1
fi
# Single File Test
if [ -f "$1" ] 
  then
  file=${1%.*}
  file=${file##*/} 
  echo "Parsing $file.txt to oneil_source/$file.oneil..."
  if [ -d oneil_source ]
    then
    rm -rf oneil_source
  fi
  mkdir oneil_source
  build/lex $1 > "oneil_source/$file.oneil"
  if [ "$?" -eq 0 ] 
    then
    echo "PASSED"
  else
    echo "FAILED"
    exit 1
  fi
  echo "Parsing oneil_source/$file.oneil to c_source/$file.c..."
  if [ -d c_source ]
    then
    rm -rf c_source
  fi
  mkdir c_source
  build/compile "oneil_source/$file.oneil" > "c_source/$file.c"
  if [ "$?" -eq 0 ] 
    then
    echo "PASSED"
  else
    echo "FAILED"
    exit 1
  fi
  if [ -d c_bin ]
    then
    rm -rf c_bin
  fi
  mkdir c_bin
  echo "Compiling c_source/$file.c to c_bin/$file..."
  gcc "c_source/$file.c" -o "c_bin/$file"
  if [ "$?" -eq 0 ] 
    then
    echo "PASSED"
    echo "Running $file..."
    c_bin/$file
    if [ "$?" -eq 0 ] 
      then
      echo "PASSED"
    else
      echo "FAILED"
      exit 1
    fi    
  else
    echo "FAILED"
    exit 1
  fi
# Directory of Test Files
elif [ -d "$1" ] 
  then
  if [ -d oneil_source ]
    then
    rm -rf oneil_source
  fi
  mkdir oneil_source
  if [ -d c_source ]
    then
    rm -rf c_source
  fi
  mkdir c_source
  if [ -d c_bin ]
    then
    rm -rf c_bin
  fi
  mkdir c_bin
  for fname in `ls -d -1 $1*`
  do
    echo $fname
    file=${fname%.*}
    file=${file##*/}
    echo "Parsing $file.txt to oneil_source/$file.oneil..."
    build/lex $fname > "oneil_source/$file.oneil"
    if [ "$?" -eq 0 ] 
      then
      echo "PASSED"
    else
      echo "FAILED"
      exit 1
    fi
    echo "Parsing oneil_source/$file.oneil to c_source/$file.c..."
    build/compile "oneil_source/$file.oneil" > "c_source/$file.c"
    if [ "$?" -eq 0 ] 
      then
      echo "PASSED"
    else
      echo "FAILED"
      exit 1
    fi
    echo "Compiling c_source/$file.c to c_bin/$file"
    gcc "c_source/$file.c" -o "c_bin/$file"
    if [ "$?" -eq 0 ] 
      then
      echo "PASSED"
      echo "Running $file..."
      c_bin/$file
      if [ "$?" -eq 0 ] 
        then
        echo "PASSED"
      else
        echo "FAILED"
        exit 1
      fi
    else
      echo "FAILED"
      exit 1
    fi
  done
fi
