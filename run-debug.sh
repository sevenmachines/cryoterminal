#!/bin/sh
LD_LIBRARY_PATH=../cryomesh/Debug/:../cryomesh-common/Debug/ Debug/cryoterminal localhost 10666 "$1"
