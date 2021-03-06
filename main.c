/*
Project 4
CPSC 346, Sec 02
Kyle McCrohan (kmccrohan@zagmail.gonzaga.edu)
Ethan Mahintorabi (emahintorabi@zagmail.gonzaga.edu)

Description:
- randomly generates an int array of length N (specified in 3rd argument, default 100)
- writes this array to a file (specified in 1st argument)
- reads this file to extract the array back into a global var
- spawns two threads to perform selection sort on the first and second halves of the array.
- afterwards, thread three joins the two halves of the array.
- thread three writes sorted array to file (specified in 2nd arg)
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
number_array_t* num_array;

// ------------------------- Functions ---------------------- //

// Merges the two halves of the array in order.
// Pass in pointer to num_array_t instance.
void merge(number_array_t* num_arr)
{

  // Extract temp variables.
  int* temp = num_arr->array;
  int size = num_arr->size;

  // malloc the new int array
  int* arr = (int*)malloc(size * sizeof(int));

  // create two indices, one for first half, one for second.
  int i1 = 0;
  int i2 = size / 2;

  // Loop through arrays, merging two parts together.
  for (int index = 0; index < size; index++)
  {
    // See if we have exceeded bounds of either parts of array.
    if (i1 >= size / 2)
    {
       arr[index] = temp[i2++];
    }
    else if (i2 >= size)
    {
      arr[index] = temp[i1++];
    }
    else
    {
      // Both sides are valid, so compare which is smaller
      if (temp[i1] < temp[i2])
      {
        arr[index] = temp[i1++];
      }
      else
      {
        arr[index] = temp[i2++];
      }
    }
  }

  // Reassign sorted array to struct.
  num_arr->array = arr;

  // free old int array.
  free(temp);
  temp = NULL;
}

/*
* Writes a number array to a file given the file name, and a number_array_t pointer
* @param file_name - The name of the file in the form of a char* parameter
* @param num_arr - The number array to be written to file.
*/
void write_number_array_to_file(char* file_name, number_array_t* num_arr)
{
  FILE* unsorted_array;
  unsorted_array = fopen(file_name, "w+");
  for(int i = 0; i < num_arr->size; i++)
  {
    fprintf(unsorted_array, "%d\n", num_arr->array[i]);
  }
  fclose(unsorted_array);
}

// Funciton to be run by thread 3. merges and writes to file.
// Pass in pointer to num_array_t instance.
void* thread3(void* param_in)
{
  // Cast pointer to a num_array pointer
  number_array_t* num_arr = (number_array_t*)param_in;

  merge(num_arr);
  write_number_array_to_file(params.file_name_sorted, num_arr);
  pthread_exit(NULL);
}

// Reads the file in and returns a pointer to the number array.
// count: length of int arr
// file: file name to be read
number_array_t* read_file(int count, char* file) 
{
  FILE * fp;
  char * line = NULL;
  size_t len = 0;
  ssize_t read;

  // malloc the int array
  int* arr = (int*)malloc(count * sizeof(int));
  int i = 0;

  fp = fopen(file, "r");

  // read each line
  while ((read = getline(&line, &len, fp)) != -1) 
  {
    arr[i++] = atoi(line);
  }

  // Close file
  fclose(fp);
  if (line)
    free(line);

  number_array_t* result = (number_array_t*) calloc(1, sizeof(number_array_t));
  result->array = arr;
  result->size = count;

  return result;
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
  for(int i = 0; i < size; i++)
  {
    raw_array[i] = rand() % 10000;
  }

  number_array_t* result = (number_array_t*) calloc(1, sizeof(number_array_t));
  result->array = raw_array;
  result->size = size;
  return result;
}

// Frees a number array.
void free_number_array(number_array_t* num_arr)
{
  free(num_arr->array);
  free(num_arr);
}

// Prints out a number array. for testing purposes.
void print_number_array(number_array_t* num_arr)
{
  for(int i = 0; i < num_arr->size; i++)
  {
    printf("%d\n", num_arr->array[i]);
  }
}


/*
* This function will sort the left half of a
* number array
* @param number_array - a struct of type number_array_t
*/
void* selection_sort_left(void* number_array)
{
  number_array_t* num_arr = (number_array_t*) number_array;

  int size = num_arr->size;
  int* array = num_arr->array;
  for(int i = 0; i < size/2; i++)
  {

    int smallest = array[i];
    int smallest_index = i;

    for(int j = i; j < size/2; j++)
    {
      if(array[j] < smallest)
      {
        smallest = array[j];
        smallest_index = j;
      }
    }

    array[smallest_index] = array[i];
    array[i] = smallest;
  }
  pthread_exit(NULL);
}

/*
* This function will sort the right half of a
* number array
* @param number_array - a struct of type number_array_t
*/
void* selection_sort_right(void* number_array)
{
  number_array_t* num_arr = (number_array_t*) number_array;

  int size = num_arr->size;
  int* array = num_arr->array;
  for(int i = size/2; i < size; i++)
  {

    int smallest = array[i];
    int smallest_index = i;

    for(int j = i; j < size; j++)
    {
      if(array[j] < smallest)
      {
        smallest = array[j];
        smallest_index = j;
      }
    }

    array[smallest_index] = array[i];
    array[i] = smallest;
  }
  pthread_exit(NULL);
}

// Main execution of program.
int main(int argc, char** argv)
{
  parseArgs(argc, argv);
  number_array_t* num_arr = create_random_list(params.count);

  write_number_array_to_file(params.file_name_unsorted, num_arr);
  free_number_array(num_arr);

  num_array = read_file(params.count, params.file_name_unsorted);

  // Spawn threads to do the sorting.
  pthread_t t1,t2,t3;
  int status;

  // Start first two threads to sort each half of array.
  status = pthread_create(&t1, NULL, selection_sort_left, (void*)num_array );
  if (status != 0)
  {
    printf("Error in thread 1:  %d\n",status);
    exit(-1);
  }

  status = pthread_create(&t2, NULL, selection_sort_right, (void*)num_array );
  if (status != 0)
  {
    printf("Error in thread 2:  %d\n",status);
    exit(-1);
  }

  // Join first two threads.
  pthread_join(t1,NULL);
  pthread_join(t2,NULL);

  // Now thread 3 merges the two halves together.
  status = pthread_create(&t3, NULL, thread3, (void*)num_array );
  if (status != 0)
  {
    printf("Error in thread 3:  %d\n",status);
    exit(-1);
  }
  pthread_join(t3,NULL);

  free_number_array(num_array);
  return 0;
}
