#include "gtest/gtest.h"
#include <math.h> 
#include "Func1D.h"
#include "Tools/AsciiIo.h"

class Func1DSpeedTest : public ::testing::Test {

protected:
  std::vector<std::vector<double>> fact_pulse_matrix;
  const uint tim_size = 300;
  const uint Nsamples = 10000;

  Func1D *fact_pulse_func;

  Func1DSpeedTest() {
    init_tim_matrix();

    fact_pulse_func = new Func1D(fact_pulse_matrix);
  }

  void init_tim_matrix() {

    for(uint i=0; i<tim_size; i++) {
      double x = double(i);
      std::vector<double> row = {x, fact_gapd_pulse(x)};
      fact_pulse_matrix.push_back(row);
    }
  }

  double Polynom3Function(const double x, const double c3, const double c2, const double c1, const double c0)const {
    const double xx = x*x;
    return x*xx*c3 + xx*c2 + x*c1 + c0;
  }

  double fact_gapd_pulse(const double x)const {
    return (1.626*(1-exp(-0.3803*0.5*x))*exp(-0.0649*0.5*x));
  }
};
//----------------------------------------------------------------------
TEST_F(Func1DSpeedTest, Func1D_fact_pulse) {
  uint i;
  double sumy = 0.0;
  for(i=0; i<Nsamples*tim_size-Nsamples; i++ ) {
    double x = double(i)/Nsamples;
    sumy += fact_pulse_func->interploate_value_at_argument(x);
  }
  std::cout << "sampels " << i << ", sum" <<  sumy << "\n";
}
//----------------------------------------------------------------------
TEST_F(Func1DSpeedTest, pure_fact_pulse) {

  uint i;
  double sumy = 0.0;
  for(i=0; i<Nsamples*tim_size-Nsamples; i++ ) {
    double x = double(i)/Nsamples;
    sumy += fact_gapd_pulse(x);
  }
  std::cout << "sampels " << i << ", sum" <<  sumy << "\n";
}
//----------------------------------------------------------------------
TEST_F(Func1DSpeedTest, pure_Polynom3) {
  uint i;
  double sumy = 0.0;
  for(i=0; i<Nsamples*tim_size-Nsamples; i++ ) {
    double x = double(i)/Nsamples;
    sumy += Polynom3Function(x,1.0,1.0,1.0,1.0);
  }
  std::cout << "sampels " << i << ", sum" <<  sumy << "\n";
}