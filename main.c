#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>

typedef struct{
  int* array;
  int size;
} number_array_t;

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

void free_number_array(number_array_t* num_arr){
  free(num_arr->array);
  free(num_arr);
}

void print_number_array(number_array_t* num_arr){
  for(int i = 0; i < num_arr->size; i++){
    printf("%d\n", num_arr->array[i]);
  }
}

int main(int argc, char** argv)
{
  number_array_t* num_arr = create_random_list(100);
  print_number_array(num_arr);

  free_number_array(num_arr);
  return 0;
}
