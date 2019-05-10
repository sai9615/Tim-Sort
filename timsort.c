#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <limits.h>
#include <omp.h>

#define size 10000000 //size of the array to store elements.
int  run = 512;
#define sizes 10000000
int arr[size];
int temp[sizes];

int min(int a, int b)
{
  if(a < b)
  {
    return a;
  }
  else
    return b;
}

int insertion_sort(int array[], int low, int high)
{
  int t,d; 
  for (int c = 1 + low; c <= high; c++) 
  {
    d = c;
    while ( d > low && array[d-1] > array[d]) {
      t = array[d];
      array[d]   = array[d-1];
      array[d-1] = t;
      d--;
    }
}
 // printf(" \n Sorted list in ascending order:\n");
 
 // for (int c = 0; c <= high - 1; c++) {
 //   printf("%d \t", array[c]);
 // } 
//  printf("\n"); 
  return 0;
}


void merge(int array[], int beg, int mid, int end)
{
  int i=beg, j=mid+1, index=beg, k;
  while((i<=mid) && (j<=end))
  {
    if(array[i] < array[j])
    {
      temp[index] = array[i];
      i++;
    } else {
      temp[index] = array[j];
      j++;
    }
    index++;
  }
  if(i>mid)
  {
    while(j<=end)
    {
      temp[index] = array[j];
      j++;
      index++;
    }
  }
  else{
    while(i<=mid)
    {
      temp[index] = array[i];
      i++;
      index++;
    }
  }
  for(k=beg; k<=end; k++)
  {
    array[k]= temp[k];
  }
}

void Timsort(int arr[], int len)
{
  for(int i=0; i<len; i= i+run)
  {
    insertion_sort(arr, i, min((i+31),(len-1)));
  }
  for(int j = run; j<len; j= 2*j)
  {
    for(int beg =0; beg <len; beg = beg + 2*j)
    {
      int mid = min((beg + j - 1),(len-1));
    //  printf(" value of mid %d\n", mid);
      int end = min((beg + 2*j - 1), (len-1));
    //  printf("end is %d\n", end);
      merge(arr,beg,mid,end);
    }
  }
}


void Parallel_Timsort(int arr[], int len)
{
  #pragma omp Parallel
  {
  #pragma omp for
  for(int i=0; i<len; i= i+run)
  {
    insertion_sort(arr, i, min((i+31),(len-1)));
  }
  for(int j = run; j<len; j= 2*j)
  {
    for(int beg =0; beg <len; beg = beg + 2*j)
    {
      int mid = min((beg + j - 1),(len-1));
    //  printf(" value of mid %d\n", mid);
      int end = min((beg + 2*j - 1), (len-1));
    //  printf("end is %d\n", end);
      merge(arr,beg,mid,end);
    }
  }
}
}

void pruntime(int runs, double time)
{
  FILE *fptr;
  fptr = fopen("Parallel_time.txt","a");
  if(fptr == NULL) {
    printf(" couldn't write to the file\n");
  }
  fprintf(fptr, "%d,%f\n",runs, time);
  fclose(fptr);
}

void sruntime(int runs, double time)
{
  FILE *fptr;
  fptr = fopen("Serial_time.txt","a");
  if(fptr == NULL) {
    printf(" couldn't write to the file\n");
  }
  fprintf(fptr, "%d,%f\n",runs, time);
  fclose(fptr);
}


int main()
{
  int n, choice;
  int a;
  clock_t clk;
  double avt;
  char alph;
  srand(time(0));
  printf("1.Test Cutoff for Timsort 2. Parallel TimSort");
  scanf("%d", &choice);
  switch(choice)
  {
      case 1: printf("Generating array with random elements\n");
      for(int k=0; k< size; k++)
      {
      arr[k] = rand() % 1000;
      }
      clk = clock();
      int narray = sizeof(arr) / sizeof(arr[0]);
      printf("%d\n", narray);
      Timsort(arr, size);
      clk = clock() - clk; 
 //     for(int q=0; q<size ; q++)
 //   {
   //     printf("%d \t", arr[q]);
  //  } 
      double clockss = ((double)clk) / CLOCKS_PER_SEC;
      sruntime(size, clockss);
      printf("\n \n Run time for Serial Timsort = %f \n", clockss);
      break;

      case 2: printf("Generating array with random elements\n");
      for(int k=0; k< size; k++)
      {
      arr[k] = rand() % 1000;
      }
      double clkst = omp_get_wtime();
      Parallel_Timsort(arr, size);
      double clke = omp_get_wtime();
  //    for(int q=0; q<size ; q++)
  //    {
   //     printf("%d \t", arr[q]);
   //   } 
      double tot_time = clke - clkst;
      pruntime(size, tot_time);
      printf("\n \n Run time for Parallel Timsort = %f \n", tot_time);
      break;

      default: printf("Enter valid choice 1 or 2\n");
  }
return 0;
}