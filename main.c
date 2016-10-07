/*
Project 4
CPSC 346, Sec 02
Kyle McCrohan (kmccrohan@zagmail.gonzaga.edu)
Ethan Mahintorabi (emahintorabi@zagmail.gonzaga.edu)

Description: 

*/
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>

// Constants
#define DEFAULT_COUNT 100

// Structs
typedef struct 
{
 int count;
 char* fileNameUnsorted;
 char* fileNameSorted;
}Parameters;

// Member Variables
Parameters params;

// ------------------------- Functions ---------------------- //

// Prints the params
void printParams() {
  printf("N: %d\n", params.count);
  printf("file1: %s\n", params.fileNameUnsorted);
  printf("file2: %s\n", params.fileNameSorted);
}

// Parses arguments and puts them in global params
// If arguments are not valid, let user know.
void parseArgs(int argc, char** argv) 
{
  if (argc < 3 )
  {
    printf("Invalid arguement count\n");
    exit(1);
  }
  else 
  {
    params.fileNameUnsorted = argv[1];
    params.fileNameSorted = argv[2];
     
    // See if there is an arg for N
    if (argc > 3) 
    {
      params.count = atoi(argv[3]);
    }
    else
    {
      params.count = DEFAULT_COUNT;
    }
  }
}

// Main execution of program.
int main(int argc, char** argv)
{
  parseArgs(argc, argv);
  printParams();
}


