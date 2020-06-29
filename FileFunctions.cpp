/*
Authors: Juan Pina-Sanz, Mikhail Mineev
Users: cssc2147, cssc2160
Class: CS 570, Summer 2020
Assignment 2
Filename: FileFunctions.cpp
/*

#include "main.h"

// Given the number of arguments (argc) in an array of arguments (argv), this
// will go through those arguments and, if necessary, bifurcate the arguments
// into arrays cmd1 and cmd2.  It will return a PipeRedirect enum representing
// whether there was a pipe in the command, a redirect to a file, or neither.
// cmd1 and cmd2 will only be populated if there was a pipe or a redirect.
PipeStatus parse_command(int argc, char** argv, char** cmd1, char** cmd2) {
  // Assume no pipe or redirect will be found.
  PipeStatus result = NOPIPE;

  // Will hold the index of argv where the pipe or redirect is found.
  int split = -1;

  // Go through the array of arguments...
  for (int i=0; i<argc; i++) {
    // Pipe found!
    if (strcmp(argv[i], "|") == 0) {
      result = PIPE;
      split = i;
  }

  // If a pipe was found...
  if (result != NOPIPE) {
    // Go through the array of arguments up to the point where the
    // pipe was found and store each of those arguments in cmd1.
    for (int i=0; i<split; i++)
      cmd1[i] = argv[i];

    // Go through the array of arguments from the point where the pipe
    // was found through the end of the array of arguments and store each
    // argument in cmd2.
    int count = 0;
    for (int i=split+1; i<argc; i++) {
      cmd2[count] = argv[i];
      count++;
    }

    // Terminate cmd1 and cmd2 with NULL, so that execvp likes them.
    cmd1[split] = NULL;
    cmd2[count] = NULL;
  }

  // Return an enum showing whether a pipe was found or not
    return result;
    }
}

// This pipes the output of cmd1 into cmd2.
void pipe_cmd(char** cmd1, char** cmd2){
  int fds[2]; // file descriptors
  pipe(fds);
  pid_t pid;

  // child process #1
  if (fork() == 0) {
    // Reassign stdin to fds[0] end of pipe.
    dup2(fds[0], 0);

    // Not going to write in this child process, so we can close this end
    // of the pipe.
    close(fds[1]);

    // Execute the second command.
    execvp(cmd2[0], cmd2);
    perror("execvp failed");

  // child process #2
  } else if ((pid = fork()) == 0) {
    // Reassign stdout to fds[1] end of pipe.
    dup2(fds[1], 1);

    // Not going to read in this child process, so we can close this end
    // of the pipe.
    close(fds[0]);

    // Execute the first command.
    execvp(cmd1[0], cmd1);
    perror("execvp failed");

  // parent process
  } else
    waitpid(pid, NULL, 0);
}

// This will get input from the user, split the input into arguments, insert
// those arguments into the given array, and return the number of arguments as
// an integer.
int read_args(char **argv){
  char *cstr;
  string arg;
  int argc = 0;

  // Read in arguments till the user hits enter
  while (cin >> arg) {
    // Let the user exit out if their input suggests they want to.
    if (want_to_quit(arg)) {
      cout << "Goodbye!\n";
      exit(0);
    }

    // Convert that std::string into a C string.
    cstr = new char[arg.size()+1];
    strcpy(cstr, arg.c_str());
    argv[argc] = cstr;

    // Increment our counter of where we're at in the array of arguments.
    argc++;

    // If the user hit enter, stop reading input.
    if (cin.get() == '\n')
      break;
  }

  // Have to have the last argument be NULL so that execvp works.
  argv[argc] = NULL;

  // Return the number of arguments we got.
  return argc;
}

void redirect_cmd(char** cmd, char** file){
  int fds[2]; // file descriptors
  int count;  // used for reading from stdout
  int fd;     // single file descriptor
  char c;     // used for writing and reading a character at a time
  pid_t pid;  // will hold process ID; used with fork()

  pipe(fds);

  // child process #1
  if (fork() == 0) {

    fd = open(file[0], O_RDWR | O_CREAT, 0666);

    // open() returns a -1 if an error occurred
    if (fd < 0) {
      printf("Error: %s\n", strerror(errno));
      return;
    }

    dup2(fds[0], 0);

    // Don't need stdout end of pipe.
    close(fds[1]);

    // Read from stdout...
    while ((count = read(0, &c, 1)) > 0)
      write(fd, &c, 1); // Write to file.

    execlp("echo", "echo", NULL);

  // child process #2
  } else if ((pid = fork()) == 0) {
    dup2(fds[1], 1);

    // Don't need stdin end of pipe.
    close(fds[0]);

    // Output contents of the given file to stdout.
    execvp(cmd[0], cmd);
    perror("execvp failed");

  // parent process
  } else {
    waitpid(pid, NULL, 0);
    close(fds[0]);
    close(fds[1]);
  }
}

// Given the number of arguments (argc) and an array of arguments (argv),
// this will fork a new process and run those arguments.
void run_cmd(int argc, char** argv) {
  pid_t pid;

  // Fork our process
  pid = fork();

  // error
  if (pid < 0)
    perror("Error (pid < 0)");

  // child process
  else if (pid == 0) {
    execvp(argv[0], argv);
    perror("execvp error");
  // parent process
  }
}

// Given a string of user input, this determines whether or not the user
// wants to exit the shell.
bool want_to_quit(string choice) {
    // Lowercase the user input
    for (unsigned int i=0; i<choice.length(); i++){
       choice[i] = tolower(choice[i]);
     }
     return (choice == "quit" || choice == "exit");
}
