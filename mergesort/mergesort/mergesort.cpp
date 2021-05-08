#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <iostream>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <thread>
#include <sys/time.h>
#include <iostream>
using namespace std;
#ifdef __cplusplus
extern "C"
{
#endif
  void generateMergeSortData(int *arr, size_t n);
  void checkMergeSortResult(int *arr, size_t n);
#ifdef __cplusplus
}
#endif

/* merge function */
void merge(int arr[], int left, int middle, int right)
{
  int i = 0;
  int j = 0;
  int k = 0;
  int left_length = middle - left + 1;
  int right_length = right - middle;
  int left_array[left_length];
  int right_array[right_length];

  /* copy values to left array */
  for (int i = 0; i < left_length; i++)
  {
    left_array[i] = arr[left + i];
  }

  /* copy values to right array */
  for (int j = 0; j < right_length; j++)
  {
    right_array[j] = arr[middle + 1 + j];
  }

  i = 0;
  j = 0;
  /** chose from right and left arrays and copy */
  while (i < left_length && j < right_length)
  {
    if (left_array[i] <= right_array[j])
    {
      arr[left + k] = left_array[i];
      i++;
    }
    else
    {
      arr[left + k] = right_array[j];
      j++;
    }
    k++;
  }

  /* copy the remaining values to the array */
  while (i < left_length)
  {
    arr[left + k] = left_array[i];
    k++;
    i++;
  }
  while (j < right_length)
  {
    arr[left + k] = right_array[j];
    k++;
    j++;
  }
}

void merge_sort(int arr[], int left, int right)
{
  if (left < right)
  {
    int middle = left + (right - left) / 2;
    merge_sort(arr, left, middle);
    merge_sort(arr, middle + 1, right);
    merge(arr, left, middle, right);
  }
}

void merge_sections_of_array(int arr[], int number, int aggregation, int NUMBERS_PER_THREAD, int n)
{
  for (int i = 0; i < number; i = i + 2)
  {
    int left = i * (NUMBERS_PER_THREAD * aggregation);
    int right = ((i + 2) * NUMBERS_PER_THREAD * aggregation) - 1;
    int middle = left + (NUMBERS_PER_THREAD * aggregation) - 1;
    if (right >= n)
    {
      right = n - 1;
    }
    merge(arr, left, middle, right);
  }
  if (number / 2 >= 1)
  {
    merge_sections_of_array(arr, number / 2, aggregation * 2, NUMBERS_PER_THREAD, n);
  }
}

int main(int argc, char *argv[])
{

  if (argc < 3)
  {
    std::cerr << "Usage: " << argv[0] << " <n> <nbthreads>" << std::endl;
    return -1;
  }

  int n = atoi(argv[1]);
  int nthreads = atoi(argv[2]);
  // get arr data
  int *arr = new int[n];
  const int NUMBERS_PER_THREAD = n / nthreads;
  const int OFFSET = n % nthreads;
  generateMergeSortData(arr, n);

  std::chrono::time_point<std::chrono::system_clock> start = std::chrono::system_clock::now();
  auto thread_merge_sort = [&](int thread_id) -> void {
    int left = thread_id * (NUMBERS_PER_THREAD);
    int right = (thread_id + 1) * (NUMBERS_PER_THREAD)-1;
    if (thread_id == nthreads - 1)
    {
      right += OFFSET;
    }
    int middle = left + (right - left) / 2;
    if (left < right)
    {
      merge_sort(arr, left, right);
      merge_sort(arr, left + 1, right);
      merge(arr, left, middle, right);
    }
    // return NULL;
  };

  thread threads[nthreads];
  for (long i = 0; i < nthreads; i++)
  {
    threads[i] = thread(thread_merge_sort, i);
  }

  for (long i = 0; i < nthreads; i++)
  {
    threads[i].join();
  }

  merge_sections_of_array(arr, nthreads, 1, NUMBERS_PER_THREAD, n);
  std::chrono::time_point<std::chrono::system_clock> end = std::chrono::system_clock::now();
  std::chrono::duration<double> elpased_seconds = end - start;
  std::cerr << elpased_seconds.count() << std::endl;
  checkMergeSortResult(arr, n);

  delete[] arr;

  return 0;
}
