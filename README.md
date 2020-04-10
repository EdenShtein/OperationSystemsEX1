# OperationSystemsEX1
QA system to grade students work


EX 1 

Part 1 - create comp.c to compare between content of two files
	system calls: open(), read(), close() (Cant use strcmp(), strncmp(), strlen())
	comp.out a.txt b.txt
	comp.out return 1 if the two files are not identical and 2 if they are the same 
	To see the result you should use the command: echo $?

Part 2 - Check student homework 
	system calls: open(), access(), stat(), S_ISREG(), S_ISDIR(), read(), write(), close(), dup2(), opendir(), closedir(), readdir(), execl(), exit(-1), fork(), wait(),  
				 Do not use strcmp(), strncmp(), strlen() - The code for those functions should be written by yourself. No existing library functions, other than system call functions should be used.
	gcc EX1_2.c
	./a.out /home/rmisegal/Homework/M1/setup/config.txt
	
	for this program we are using configuration file: /home/rmisegal/Homework/M1/setup/config.txt
	This file contain defination:
		/home/rmisegal/Homework/M1/students - The sub folder list location of the student names.
		/home/rmisegal/Homework/M1/setup/test_input.qa - The keyboard input (use dup2 to read input from file instead from keboard).
		/home/rmisegal/Homework/M1/setup/expected_output.qa - The expected output that the student program should print to the screen
	
	Under the student folder there are the student names: David  Joe  Michael  Yael
	Each student folder should contain C code. and its compile version:
		David.c main.out
		Joe.c main.out
		Michael.c main.out
		Yael.c main.out
	The main.out program is an interactive software that prints on the screen requests that the user has to responce with keyboard numbers. 
	The software returns the sum of two numbers.
	main.out screen and keyboard interactions:
					--------------------------
					please enter first number:
					2
					please enter scond number:
					3
					the sum is:
					5
					--------------------------
	
	
	For each student, a.out will use the comp.out to compare between the main.out printout and the expected result that exist in expected_output.qa
	remark: main.out printout will be save into program_output.txt
	comp.out /home/rmisegal/Homework/M1/setup/expected_output.qa /home/rmisegal/Homework/M1/program_output.txt
	
	The score for all students will be save into one file results.csv 
	comp.out return 1 if the two files are not identical and 2 if they are the same
	The comp.out printout will be redirect from srcreen into results.csv file 
	
	
	Pseudo code algorithm:
		---------------------------------------------------------
		a.out <--- config.txt
		readdir(students)
		for each students:
			execl(gcc student_name.c -o main.out)
			open(test_input.qa, O_RDONLY) 
			open(program_output.txt, O_WRONLY | O_CREAT | O_TRUNC ) 
			dup2(test_input.qa , keyboard)
			dup2(program_output.txt , screen)
			exccl(main.out)
			open(results.csv, O_WRONLY | O_CREAT | O_APPEND) 
			dup2(results.csv , screen)
			execl(comp.out program_output.txt expected_output.qa)
		dup2(screen, results.csv)
		printout on screen results.csv
		---------------------------------------------------------
	
	
	List of files and folder in this exercise:
	
	Folders	
		/home/rmisegal/Homework/M1 - main code
		/home/rmisegal/Homework/M1/setup/ - Configuration files
		/home/rmisegal/Homework/M1/students  - sutdent root list name
		/home/rmisegal/Homework/M1/students/David
		/home/rmisegal/Homework/M1/students/Joe
		/home/rmisegal/Homework/M1/students/Michael
		/home/rmisegal/Homework/M1/students/Yael
		
	Files
		/home/rmisegal/Homework/M1/comp.c	
		/home/rmisegal/Homework/M1/comp.out
		/home/rmisegal/Homework/M1/setup/config.txt
		/home/rmisegal/Homework/M1/EX1_2.c
		/home/rmisegal/Homework/M1/a.out
		/home/rmisegal/Homework/M1/program_output.txt
		/home/rmisegal/Homework/M1/results.csv
		/home/rmisegal/Homework/M1/setup/config.txt
		/home/rmisegal/Homework/M1/setup/test_input.qa
		/home/rmisegal/Homework/M1/setup/expected_output.qa
		/home/rmisegal/Homework/M1/students/David/David.c
		/home/rmisegal/Homework/M1/students/David/main.out
		/home/rmisegal/Homework/M1/students/Joe/Joe.c
		/home/rmisegal/Homework/M1/students/Joe/main.out
		/home/rmisegal/Homework/M1/students/Michael/Michael.c
		/home/rmisegal/Homework/M1/students/Michael/main.out
		/home/rmisegal/Homework/M1/students/Yael/Yael.c
		/home/rmisegal/Homework/M1/students/Yael/main.out
		
	Files contents:
	
		config.txt
						--------------------------
						/home/rmisegal/Homework/M1/students
						/home/rmisegal/Homework/M1/setup/test_input.qa
						/home/rmisegal/Homework/M1/setup/expected_output.qa	
						--------------------------
		
		test_input.qa
						--------------------------
						2
						3	
						--------------------------	
		
		David.c, Joe.c, Michael.c, Yael.c 
						--------------------------	
						#include <stdio.h>
						int main(int argc, char* argv[]){
						int a,b;
						printf("please enter first number: \n");
						scanf("%d", &a);
						printf("please enter scond number: \n");
						scanf("%d", &b);	
						printf("the sum is: \n");
						printf("%d \n", a+b);
						return 0;
						}
						--------------------------	

		expected_output.qa	
						--------------------------
						please enter first number:
						2
						please enter scond number:
						3
						the sum is:
						5
						--------------------------
		
		results.csv
						--------------------------
						David,1
						Joe,2
						Michael,2
						Yael,1
						--------------------------
						

		
		
		
		
