#include "inc/main.hpp"

/*
 I attempted to do a kaiser window here, but I think I'm doing something
 horribly wrong, maybe I'll read about it later, but it's a little beyond me at
 the moment. For now the hamming_window is easier and more straightforward.
*/

double *Synth::kaiser_window(double beta, int filter_length) {
  double *window = new double[filter_length];
  const double alpha = (filter_length - 1) / 2.0;
  for (int n = 0; n < filter_length; ++n) {
    window[n] = std::cyl_bessel_i(
                    0, beta * std::sqrt(1 - std::pow((n - alpha) / alpha, 2))) /
                std::cyl_bessel_i(0, beta);
  }
  return window;
}

double *Synth::generate_kcoefficients(int filter_length, double beta) {
  double *coeffs = new double[filter_length];
  double *kwincoeffs = kaiser_window(beta, filter_length);
  double cutoff = 0.85;

  for (int i = 0; i < filter_length; ++i) {
    if (i == (filter_length - 1) / 2) {
      coeffs[i] = 2 * cutoff;
    } else {
      double t = i - (static_cast<double>(filter_length) - 1) / 2;
      coeffs[i] = sin(2 * M_PI * cutoff * t) / (M_PI * t);
    }
    coeffs[i] *= kwincoeffs[i];
  }

  delete[] kwincoeffs;
  return coeffs;
}

/*
 hamming window sinc for generating the appropriate coefficients for the FIR
 filter. 0.54 and 0.46 are constants typically used in this.
*/

double *Synth::hamming_window(int filter_length) {
  double *window = new double[filter_length];
  for (int n = 0; n < filter_length; ++n) {
    window[n] = 0.54 - 0.46 * cos(2 * M_PI * n / (filter_length - 1));
  }
  return window;
}

double *Synth::generate_hcoefficients(int filter_length) {
  double *hcoeffs = new double[filter_length];
  double *hwincoeffs = hamming_window(filter_length);

  /*
   Here the nyquist frequency is used to generate a cutoff frequency in Hz, so
   that it can be used to generate the coefficients for the fir filter. I am
   going with 0.2 of the nyquist which is around 4000 hz, I can modify this to
   be a bit more consequential later, but this is fine for the moment.
  */

  double nyquist = static_cast<double>(SR) / 2.0;
  double cutoff = nyquist * 0.2;
  printf("SET CUTOFF : %6.3f\n", cutoff);
  int m = (filter_length - 1) / 2;
  for (int n = 0; n < filter_length; ++n) {
    if (n == m) {
      hcoeffs[n] = 2 * cutoff / SR;
    } else {
      hcoeffs[n] = sin(2 * M_PI * cutoff * (n - m) / SR) / (M_PI * (n - m));
    }
    hcoeffs[n] *= hwincoeffs[n];
  }
  delete[] hwincoeffs;
  return hcoeffs;
}
