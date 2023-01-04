#!/bin/bash -eu
echo "input pid :"
read PID
echo "input vaddr_head :"
read VADDR_HEAD
echo "input vaddr_end :"
read VADDR_END

make -s -C modules/ search_huge OPCODE=2 PID=$PID VADDR_HEAD=0x$VADDR_HEAD VADDR_END=0x$VADDR_END