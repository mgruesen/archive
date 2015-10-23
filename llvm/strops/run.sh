#!/bin/bash

for bin in build/*
do
  echo $bin
  ./$bin
done