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
 char* file_name_unsorted;
 char* file_name_sorted;
}Parameters;

typedef struct{
  int* array;
  int size;
} number_array_t;

// Member Variables
Parameters params;

// ------------------------- Functions ---------------------- //

// Prints the params
void printParams() {
  printf("N: %d\n", params.count);
  printf("file1: %s\n", params.file_name_unsorted);
  printf("file2: %s\n", params.file_name_sorted);
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
    params.file_name_unsorted = argv[1];
    params.file_name_sorted = argv[2];

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


/**
* This function will return a number_array_t
* struct that will contain a int[] of random integers
* the list size will be set by the calle
*
* @param size - the size of the array
*/
number_array_t* create_random_list(int size)
{
  time_t t;
  srand((unsigned) time(&t));

  int* raw_array = (int*) calloc(size, sizeof(int));
  for(int i = 0; i < size; i++){
    raw_array[i] = rand() % 10000;
  }

  number_array_t* result = (number_array_t*) calloc(1, sizeof(number_array_t));
  result->array = raw_array;
  result->size = size;
  return result;
}

// Frees a number array.
void free_number_array(number_array_t* num_arr){
  free(num_arr->array);
  free(num_arr);
}

// Prints out a number array. for testing purposes.
void print_number_array(number_array_t* num_arr){
  for(int i = 0; i < num_arr->size; i++){
    printf("%d\n", num_arr->array[i]);
  }
}

void write_number_array_to_file(char* file_name, number_array_t* num_arr)
{
  FILE* unsorted_array;
  unsorted_array = fopen(file_name, "w+");
  for(int i = 0; i < num_arr->size; i++){
    fprintf(unsorted_array, "%d\n", num_arr->array[i]);
  }
}

// Main execution of program.
int main(int argc, char** argv)
{
  parseArgs(argc, argv);
  number_array_t* num_arr = create_random_list(params.count);
  write_number_array_to_file(params.file_name_unsorted, num_arr);
  print_number_array(num_arr);
  free_number_array(num_arr);
  return 0;
}
