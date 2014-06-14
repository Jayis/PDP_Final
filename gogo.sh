#!/bin/bash
make
mpiexec -n 16 a.out >result
python showpoint.py
