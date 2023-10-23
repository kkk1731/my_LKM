#!/bin/bash -eu
echo "input pid :"
read PID
echo "input vaddr_head :"
read VADDR_HEAD
echo "input vaddr_end :"
read VADDR_END
echo "input opcode"
echo " 1:v2p"
echo " 2:search_huge"
echo " 3:show_vma"

echo ":"
read OPCODE

make -s -C modules/ search_huge OPCODE=$OPCODE PID=$PID VADDR_HEAD=0x$VADDR_HEAD VADDR_END=0x$VADDR_END