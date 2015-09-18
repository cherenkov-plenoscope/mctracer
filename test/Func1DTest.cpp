#include "gtest/gtest.h"
#include <math.h> 
#include "Func1D.h"
#include "Tools/AsciiIo.h"

class Func1DTest : public ::testing::Test {

protected:
  double** tim;
  uint tim_size = 1337;

  double **with_duplicate;
  uint with_duplicate_size = 1337;

  std::vector<std::vector<double>> tim_matrix;

  Func1DTest() {
    init_tim_array();
    init_duplicate_array();
    init_tim_matrix();
  }

  ~Func1DTest() {
    delete_2D_array(tim, tim_size);
    delete_2D_array(with_duplicate, with_duplicate_size);
  }

  void init_tim_array() {

    tim = new double* [tim_size];

    double phase = 0.25;

    for(uint i=0; i<tim_size; i++) {

      double x = double(i)/double(tim_size);
      double y = sin(x*2.0*M_PI + phase);

      tim[i] = new double[2];
      tim[i][0] = x;
      tim[i][1] = y;
    }
  }

  void init_duplicate_array() {

    with_duplicate = new double* [with_duplicate_size];

    for(uint i=0; i<with_duplicate_size; i++) {
      with_duplicate[i] = new double[2];
      with_duplicate[i][0] = 4.2;
      with_duplicate[i][1] = 1.337;
    }
  }

  void delete_2D_array(double** arr, const uint size) {
    for(uint i=0; i<size; i++)
      delete arr[i];
    delete arr;
  }

  void init_tim_matrix() {

    for(uint i=0; i<tim_size; i++) {
      std::vector<double> row = {tim[i][0], tim[i][1]};
      tim_matrix.push_back(row);
    }
  }
};
//----------------------------------------------------------------------
TEST_F(Func1DTest, construct_using_matrix_of_vectors) {

  Func1D f(tim_matrix);
  for(uint i=0; i<tim_matrix.size(); i++) {
    double argument = tim_matrix.at(i).at(0);
    double value = tim_matrix.at(i).at(1);
    EXPECT_NEAR(value, f.interploate_value_at_argument(argument), 1e-9);
  }
}
//----------------------------------------------------------------------
TEST_F(Func1DTest, access_inside_range_in_mode_STRICT) {

  Func1D f(tim_matrix);

  for(uint i=0; i<tim_size; i++) {
    double argument = tim[i][0];
    double value = tim[i][1];
    EXPECT_NEAR(value, f.interploate_value_at_argument(argument), 1e-9);
  }
}
//----------------------------------------------------------------------
TEST_F(Func1DTest, access_below_lowest_argument_in_mode_STRICT) {

  Func1D f(tim_matrix);
  double arg_below_definition = tim[0][0] - 1e-9;

  EXPECT_THROW( f.interploate_value_at_argument(arg_below_definition), TracerException);
}
//----------------------------------------------------------------------
TEST_F(Func1DTest, access_above_highest_argument_in_mode_STRICT) {

  Func1D f(tim_matrix);
  double arg_above_definition = tim[tim_size-1][0] + 1e-9;

  EXPECT_THROW( f.interploate_value_at_argument(arg_above_definition), TracerException);
}
//----------------------------------------------------------------------
TEST_F(Func1DTest, access_exactly_on_lowest_argument_in_mode_STRICT) {

  Func1D f(tim_matrix);
  double arg = tim[0][0];

  EXPECT_NEAR(tim[0][1], f.interploate_value_at_argument(arg), 1e-9);
}
//----------------------------------------------------------------------
TEST_F(Func1DTest, access_exactly_on_highest_argument_in_mode_STRICT) {

  Func1D f(tim_matrix);
  double arg = tim[tim_size-1][0];

  EXPECT_NEAR(tim[tim_size-1][1], f.interploate_value_at_argument(arg), 1e-9);
}
//----------------------------------------------------------------------
TEST_F(Func1DTest, generate_from_from_file) {

  std::string path = "numeric_table_IO/tim_sinus.csv";
  AsciiIo::write_table_to_file(tim_matrix ,path);

  Func1D f(AsciiIo::gen_table_from_file(path));

  // precision loss in ascii files, cant access boundarys sharp,
  // start in row 1 and stop one row before end.
  for(uint i=1; i<tim_matrix.size()-1.; i++) {
    double argument = tim_matrix[i][0];
    double value = tim_matrix[i][1];
    EXPECT_NEAR(value, f.interploate_value_at_argument(argument), 1e-9);
  }
}
//----------------------------------------------------------------------
TEST_F(Func1DTest, empty_function) {

  std::vector<std::vector<double>> empty;
  EXPECT_THROW(Func1D fu(empty);, TracerException);
}