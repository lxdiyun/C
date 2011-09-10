smsh: smsh1.o execute.o splitline.o process.o controlflow.o
	gcc -o $@ $^ -Wall
smsh1.o: smsh1.c smsh.h
	gcc -c $< -Wall
execute.o: execute.c
	gcc -c $< -Wall
splitline.o: splitline.c smsh.h
	gcc -c $< -Wall
process.o: process.c smsh.h
	gcc -c $< -Wall
controlflow.o: controlflow.c smsh.h
	gcc -c $< -Wall
