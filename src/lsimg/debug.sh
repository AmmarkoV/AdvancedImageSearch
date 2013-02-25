#!/bin/bash
   
time valgrind --tool=memcheck --error-limit=no --leak-check=yes --show-reachable=yes --num-callers=20 --track-fds=yes  ./lsimg $@ 2>error.txt
cd ..

exit 0
