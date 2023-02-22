# What is this folder?

This folder containes all the files related to testing and comparing the performance speed of arithmetic operations.

# How to work with this

All the testing code is located in main.cpp file. To build the executables, use build.sh script. It will generate a number of binary files inside the bin folder. If you don't have one, you need to create it. But don't worry, bash will yell at you if you forget.

To perform the test, run test.sh. You may provide an optional argument — the name of the subdirectory of data folder where the output files should be stored. If you don't provide said argument, the files will be placed inside the data/temp directory, and all the information previously stored the will be erased. Bash might yell at you once again if you don't create the respective folder.

After performing the tests, you might want to proccess the data you just collected. To gather it in one file, use compose.sh. You must provide it with one argument: the name of the subdirectory of the data folder, whose data you want to "compose" into a single file. For example, './compose.sh temp' will collect all the files in data/temp and copy their content into data/temp.txt .

Regardless of composing, you can use data_parser.py script to parse the data (he-he). It requires one argument: the name of the file containing the data to be processed. The output dipsplayed by the parser is in the Latex format.

The Report.pdf file is there for history; it's a brief explanation of the work I've don, and doesn't do anything.