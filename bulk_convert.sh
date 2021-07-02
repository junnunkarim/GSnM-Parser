#!/bin/sh

script_dir="script/"
output_dir="output/"
exe_name="gsnm-parser.out"
source_name="gsnm-parser.c"

if [ -f "$output_dir" ]
then
	echo "Removing previous outputs from $output_dir..."
	rm -rf "$output_dir"

	if [ !"$output_dir" ]
	then
		echo "Done"
	fi
fi

echo "Creating new GSnM-Parser executable..."
gcc "$source_name" -o "$exe_name"
echo "Done"


for file in "$script_dir"*.ks
do
	./"$exe_name" "$file"
done

if [ -f "output/" ]
then
	mkdir "output"
fi

for file1 in g*.html
do 
	mv "$file1" "output/"
done
