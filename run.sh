#!/bin/bash
make
./c+- test
java -jar jasmin.jar out.j
java out
rm -f out.class
