#!/bin/bash

D='/usr/src'
output_file='/home/oscreader/repos/OS-Labs/lab1.5/linux_source_structure.txt'

for d in $D; do
	if [ -d "$d" ]; then
		echo 'Directory: '$d >> $output_file
		echo '.c files: ' >> $output_file
		find $d -name '*.c' >> $output_file
		echo '.h files: ' >> $output_file
		find $d -name '*.h' >> $output_file
	fi
done
