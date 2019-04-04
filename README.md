# Travelling-Salesman-Problem
Implementation of a genetic algorithm to solve the tsp.  
(This project was developed on my first semester and thus the code is not really well-structured.)

DESCRIPTION  
	-c		: Reads one more path and checks if its accurate. If yes, its length is printed.  
	-p [VALUE]	: Sets Population = VALUE  
	-g [VALUE]	: Sets Generations = VALUE  
	-pc [VALUE]	: Sets Pc = VALUE  
	-pm [VALUE]	: Sets Pm = VALUE  
	-i [FILE]	: Reads data from FILE
  
Some sample data can be found in the following link:
http://comopt.ifi.uni-heidelberg.de/software/TSPLIB95/tsp/

By default:  
Population=100, Generations=10000, Pc=0.5, Pm=0.1

Example:
```
./tspga < test.txt
```

Example:
```
./tspga -i test2.txt -g 500
```
Example:
```
./tspga -i swiss42.tsp -p 100 -g 1000 -pc 0.85 -pm 0.6
```
