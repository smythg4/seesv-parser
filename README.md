# seesv-parser
A .csv file parser in C

# Parses multiple csv files in strict format and puts them into a red-black tree structure

Steps and goals:
- ~~accept a command line argument to define filename~~
- ~~open and read a file (include proper error handling)~~
- ~~split lines up on '\n's and send lines off to a line-processing subroutine~~
- ~~write single line processing subroutine that takes a line and return a Person struct~~
- ~~generate a linked-list of Person structs~~
- ~~write a sorting routine that's provided a list of persons and a property to sort by~~
- ~~write a subroutine to print a Person struct~~
- ~~write a subroutine to print a linked list of Person structs~~
- ~~replace linked list of Persons with black-red tree~~
- ~~allow program to accept multiple filenames~~
- ~~read and parse multiple files~~
- add functionality to remove a rb tree node
- remove debug statements
- allow program to accept an output argument
- take the RB tree structure and generate consolidated output file