#include "inc/operations.hpp"
#include <cstdio>

void swap(int *a, int *b) {
  int t = *a;
  *a = *b;
  *b = t;
}

int partition(int *map, int low, int high) {
  int pivot = map[high];
  int i = (low - 1);
  for (int j = low; j <= high - 1; j++) {
    if (map[j] < pivot) {
      i++;
      swap(&map[i], &map[j]);
    }
  }
  swap(&map[i + 1], &map[high]);
  return (i + 1);
}

void quicksort(int *map, int low, int high) {
  if (low < high) {
    int pivot = partition(map, low, high);
    quicksort(map, low, pivot - 1);
    quicksort(map, pivot + 1, high);
  }
}

void swapf(double *a, double *b) {
  int t = *a;
  *a = *b;
  *b = t;
}

int partitionf(double *map, int low, int high) {
  int pivot = map[high];
  int i = (low - 1);
  for (int j = low; j <= high - 1; j++) {
    if (map[j] < pivot) {
      i++;
      swapf(&map[i], &map[j]);
    }
  }
  swapf(&map[i + 1], &map[high]);
  return (i + 1);
}

void quicksortf(double *map, int low, int high) {
  if (low < high) {
    int pivot = partitionf(map, low, high);
    quicksortf(map, low, pivot - 1);
    quicksortf(map, pivot + 1, high);
  }
}

int find_key_pressed(int *enumlist, int target) {
  int left = 0;
  int right = 23;
  while (left <= right) {
    int middle = left + (right - left) / 2;
    if (enumlist[middle] == target) {
      return middle;
    } else if (enumlist[middle] < target) {
      left = middle + 1;
    } else {
      right = middle - 1;
    }
  }
  return -1;
}
