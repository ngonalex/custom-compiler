#!/bin/bash
gcc -c test.s && ld test.o && ./a.out
exit 0