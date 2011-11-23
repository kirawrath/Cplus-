#!/bin/bash
./c+- $1
java -jar jasmin.jar out.j
java out
rm -f out.class
