README.md
Authors: Juan Pina-Sanz, Mikhail Mineev

Users: cssc2147, cssc2160

Class: CS 570, Summer 2020

Assignment 2

Filename: README

File Manifest: Makefile main.cpp main.h README.md FileHandler.cpp

Run on Class Account cssc2147 

Compile Instructions: Compile using make

Operation Instructions: After compiling run the msh file, it will open the microshell and allow th euser to input their own commands.

Design Description: Our idea for this Microshell was to divide the user input into arguments that could be passed through to execvp and execlp based on the type of command or path entered. We check the input arguments and verify if there is a pipe or not and call the appropriate function to handle the execution of the command.

Bugs: The MicroShell will output a prompt initially, and despite there not being another one after the first command is entered, it will still accept commands afterwards.

For the program Mikhail Mineev handled the user data,Juan Pina-Sanz handled the execution of the user data, both worked on coments and README together. 
                 
In the process of making this program we tried out a large number of solutions we believed would work. Unfortunately we wasted a large ammount of time on a solution which did not end up working. Overall all seemed well, however we essentially boxed ourselves in with the way we decided to handle arrays. We did not anticipate using the exec commands, and did not setup our program accordingly. We later realized that this was one of the requirements and instead of attempting to adopt or rather convoluted program we decided to start off from scratch and base our entire program around the exec functions. The main problem we faced with this new program wasthe ability to run multiple pipes in a single command. Our main problem was the ability to traverse from one pipe command to the other. Our initial idea was to initialize to two buffers which would change depending on how many pipes were executed. In essence we wanted to have 3 cases: case 1 was inputed file only( no pipes), case 2 was only 1 pipe, and case 3 would have handled 2+ pipes.
We ended up using 2 flags, pipe and nopipe in order to deferentiate between file entrees and piped entrees.  As well as lhBuff and rhBuff in order to separate the array using the pipe.
