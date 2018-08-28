#!/bin/bash

D='/usr/src'
output_file='/home/oscreader/repos/OS-Labs/lab1.5/linux_source_structure.txt'

for d in $( find /usr/src -type d ); do
	echo 'Directory: '$d >> $output_file
	echo '.c files: ' >> $output_file
	ls $d/*.c >> $output_file
	echo '.h files: ' >> $output_file
	ls $d/*.h >> $output_file
done
