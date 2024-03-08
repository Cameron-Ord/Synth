#include "inc/main.hpp"

void DataOps::swap(int *a, int *b) {
  int t = *a;
  *a = *b;
  *b = t;
}

int DataOps::partition(int *map, int low, int high) {
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

void DataOps::quicksort(int *map, int low, int high) {
  if (low < high) {
    int pivot = partition(map, low, high);
    quicksort(map, low, pivot - 1);
    quicksort(map, pivot + 1, high);
  }
}

void DataOps::swapf(double *a, double *b) {
  int t = *a;
  *a = *b;
  *b = t;
}

int DataOps::partitionf(double *map, int low, int high) {
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

void DataOps::quicksortf(double *map, int low, int high) {
  if (low < high) {
    int pivot = partitionf(map, low, high);
    quicksortf(map, low, pivot - 1);
    quicksortf(map, pivot + 1, high);
  }
}

int DataOps::find_key_pressed(int *enumlist, int target) {
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

double *DataOps::kaiser_window(double beta) {
  double *window = new double[BUFFERSIZE];
  const double alpha = (BUFFERSIZE - 1) / 2.0;
  for (int n = 0; n < BUFFERSIZE; ++n) {
    window[n] = std::cyl_bessel_i(
                    0, beta * std::sqrt(1 - std::pow((n - alpha) / alpha, 2))) /
                std::cyl_bessel_i(0, beta);
  }
  return window;
}

double *DataOps::generate_coefficients(int filter_length, double beta) {
  double *coeffs = new double[filter_length];
  double *kwincoeffs = kaiser_window(beta);
  double nyquist = SAMPLERATE / 2.0;
  double fraction = 0.00005;
  double cutoff = fraction * nyquist;
  for (int i = 0; i < filter_length; ++i) {
    if (i == (filter_length - 1) / 2) {
      coeffs[i] = 2 * cutoff;
    } else {
      double t = i - ((double)filter_length - 1) / 2;
      coeffs[i] = sin(2 * M_PI * cutoff * t) / (M_PI * t);
    }
  }

  for (int j = 0; j < filter_length; ++j) {
    coeffs[j] *= kwincoeffs[j];
  }
  delete[] kwincoeffs;
  return coeffs;
}

double *DataOps::FIRfunc(double buffer[], double coeff[], int filter_length) {
  double *filtered_buffer = new double[BUFFERSIZE];
  for (int i = 0; i < BUFFERSIZE; ++i) {
    double OUT = 0.0;
    for (int j = 0; j < filter_length; ++j) {
      OUT += buffer[i + j] * coeff[j];
    }
    filtered_buffer[i] = OUT;
  }
  return filtered_buffer;
}
