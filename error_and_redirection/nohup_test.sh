#!/bin/bash

nohup "$@" > nohup.log 2>&1 &

# nohup command 2>file1.txt 1>file2.tex &
# 
# 
# or
# 
# nohup command 2>/dev/null 1>/dev/null &
