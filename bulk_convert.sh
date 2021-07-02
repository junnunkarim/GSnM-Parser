#!/bin/bash

script_dir="script/"
output_dir="output/"
exe_name="gsnm-parser.out"
source_name="gsnm-parser.c"

if [ -d "$output_dir" ]
then
	echo "Removing previous outputs from $output_dir..."
	rm -rf "$output_dir"

	if [ ! -d "$output_dir" ]
	then
		echo "Done"
	fi
fi

echo "Creating new GSnM-Parser executable..."
	gcc "$source_name" -o "$exe_name"
echo "Done"

if [ ! -d "$output_dir" ]
then
	mkdir "$output_dir"
fi

echo "Performing actions-"
for file in "$script_dir"*.ks
do
	./"$exe_name" "$file"
	echo "$file"
done

for file_move in g*.html
do 
	mv "$file_move" "$output_dir"
done

rm -f "$exe_name"
