#include "gtest/gtest.h"
#include "Tools/TableTools.h"

class TableToolsTest : public ::testing::Test {
};
//----------------------------------------------------------------------
TEST_F(TableToolsTest, read_non_existing_file) {

  EXPECT_THROW(
    std::valarray<std::valarray<double>> table = 
      TableTools::read_file("non_existing_path/non_existing_file.csv"),
    TracerException
  );
}
//----------------------------------------------------------------------
TEST_F(TableToolsTest, read_empty_file) {

  EXPECT_THROW(
    std::valarray<std::valarray<double>> table = 
      TableTools::read_file("numeric_table_IO/empty.csv"),
    TracerException
  );
}
//----------------------------------------------------------------------
TEST_F(TableToolsTest, read_valid_file_table_only) {

  std::valarray<std::valarray<double>> table = 
    TableTools::read_file("numeric_table_IO/matrix_4col_3row.csv");

  //wavelength, mean_free_path, absorbtion_coeff, wuff
  //123.4, 100.0, 0.99, 133.7
  //234.5, 101.0, 0.04, 13.37
  //666.6, 88.8, 0.056, 1.337

  ASSERT_EQ(4, table.size());
  ASSERT_EQ(3, table[0].size());
  ASSERT_EQ(3, table[1].size());
  ASSERT_EQ(3, table[2].size());
  ASSERT_EQ(3, table[3].size());

  // 1st row
  EXPECT_EQ(123.4, table[0][0]);
  EXPECT_EQ(100.0, table[1][0]);
  EXPECT_EQ(0.99,  table[2][0]);
  EXPECT_EQ(133.7, table[3][0]);

  // 2nd row
  EXPECT_EQ(234.5, table[0][1]);
  EXPECT_EQ(101.0, table[1][1]);
  EXPECT_EQ( 0.04, table[2][1]);
  EXPECT_EQ(13.37, table[3][1]);
  
  // 3rd row
  EXPECT_EQ(666.6, table[0][2]);
  EXPECT_EQ(88.8,  table[1][2]);
  EXPECT_EQ(0.056, table[2][2]);
  EXPECT_EQ(1.337, table[3][2]);
}
//----------------------------------------------------------------------
TEST_F(TableToolsTest, read_valid_file) {

  TableTools::Factory tab_fab("numeric_table_IO/matrix_4col_3row.csv");

  std::valarray<std::valarray<double>> table = tab_fab.get_table();
  std::vector<std::string> column_names = tab_fab.get_column_names();

  //wavelength, mean_free_path, absorbtion_coeff, wuff
  //123.4, 100.0, 0.99, 133.7
  //234.5, 101.0, 0.04, 13.37
  //666.6, 88.8, 0.056, 1.337

  ASSERT_EQ(4, column_names.size());
  EXPECT_TRUE(StringTools::is_equal(column_names[0], "wavelength"));
  EXPECT_TRUE(StringTools::is_equal(column_names[1], "mean_free_path"));
  EXPECT_TRUE(StringTools::is_equal(column_names[2], "absorbtion_coeff"));
  EXPECT_TRUE(StringTools::is_equal(column_names[3], "wuff"));

  // 4 columns and 3 rows
  ASSERT_EQ(4, table.size());
  ASSERT_EQ(3, table[0].size());
  ASSERT_EQ(3, table[1].size());
  ASSERT_EQ(3, table[2].size());
  ASSERT_EQ(3, table[3].size());

  // 1st row
  EXPECT_EQ(123.4, table[0][0]);
  EXPECT_EQ(100.0, table[1][0]);
  EXPECT_EQ(0.99,  table[2][0]);
  EXPECT_EQ(133.7, table[3][0]);

  // 2nd row
  EXPECT_EQ(234.5, table[0][1]);
  EXPECT_EQ(101.0, table[1][1]);
  EXPECT_EQ( 0.04, table[2][1]);
  EXPECT_EQ(13.37, table[3][1]);
  
  // 3rd row
  EXPECT_EQ(666.6, table[0][2]);
  EXPECT_EQ(88.8,  table[1][2]);
  EXPECT_EQ(0.056, table[2][2]);
  EXPECT_EQ(1.337, table[3][2]);
}
//----------------------------------------------------------------------
TEST_F(TableToolsTest, write_table) {

  const uint rows = 100;
  const uint cols = 10;
  std::string path = "numeric_table_IO/written_table.csv";

  std::valarray<std::valarray<double>> table(cols);

  double value;
  for(uint col=0; col<cols; col++) {
    std::valarray<double> column(rows);
    for(uint row=0; row<rows; row++) {
      value = double(col)*double(row)/(double(rows)*double(cols));
      column[row] = value;
    }
    table[col] = column;
  }

  std::vector<std::string> column_names(cols);
  for(uint col=0; col<cols; col++) {
    std::stringstream name;
    name << "name of col: " << col;
    column_names[col] = name.str();
  }

  TableTools::write_table_and_column_names_to_file(
    table,
    column_names,
    path
  );

  //----------------------------------------------------------------------------
  // read in again
  //----------------------------------------------------------------------------
  
  TableTools::Factory tab_fab(path);
  std::valarray<std::valarray<double>> table_in = tab_fab.get_table();
  std::vector<std::string> column_names_in = tab_fab.get_column_names();

  // check column names
  ASSERT_EQ(cols, column_names_in.size());

  for(uint col=0; col<cols; col++)
    EXPECT_TRUE(StringTools::is_equal(column_names_in[col], column_names[col]));


  ASSERT_EQ(cols, table_in.size());
  for(uint col=0; col<cols; col++)
    ASSERT_EQ(rows, table_in[col].size());

  for(uint col=0; col<cols; col++) {
    for(uint row=0; row<rows; row++) {
      EXPECT_EQ(table[col][row], table_in[col][row]);
    }
  }
}