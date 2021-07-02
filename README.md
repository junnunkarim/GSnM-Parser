# GSnM-Parser
A small program to parse "G-Senjou no Maou" scripts and create an ebook

## Introduction
This program parses scripts of "G-Senjou no Maou" and creates an ebook without compromising choices. While reading it as ebook, you will get choices and clicking them will take you to the corresponding ending. You can find a pre formatted epub in `ebook` directory.

## Instructions
1. You can compile the program and feed it a G-Senjou no Maou script file or you can use the `bulk_convert.sh`(outputs html files) to batch convert the scripts of the novel.
1. If using the `bulk_convert.sh`-
	1. First make sure you are using a linux based distro (if using windows, download and install cygwin with gcc and git, should work on android(with termux) and any unix based OS)
	1. Clone this repo to your local machine and `cd` to it
	1. Make a directory named `script`
	1. Extract and decrypt the data.xp3 file found in the steam release(use KrKrExtract)
	1. Copy the scripts from the `scenario` directory found from the extracted folder to the `script/` directory
	1. Then run `./bulk_convert.sh`
	1. It will produce many html files in a new directory named `output`. Now install calibre and open it.
	1. Create an empty ebook by using `Ctrl-Shift-E` and choose the format as epub.
	1. Right-Click on the newly created ebook and select `Edit Book`.
	1. Click on file > Import files into book and selct all the html files from the output folder.
	1. Then click on Tools > Table of Contents > Edit Table of Contents and click on `Generate ToC from major headings`
	1. Click done and save the file.
