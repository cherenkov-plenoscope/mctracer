#include "gtest/gtest.h"
#include "Scenery/Geometry/HexGridAnnulus.h"
#include "Scenery/Geometry/GridNeighborhoodTopoligy.h"

class GridNeighborhoodTopoligyTest : public ::testing::Test {};
//------------------------------------------------------------------------------
TEST_F(GridNeighborhoodTopoligyTest, init) {

    double outer_R = 1.0;
    double spacing = 0.2;

    HexGridAnnulus grid_gen(outer_R, spacing);

    std::vector<Vec3> grid = grid_gen.get_grid();

    GridNeighborhoodTopoligy topo(&grid, spacing*1.1);

    std::vector<std::vector<unsigned int>> neighbors = topo.get_neighbor_relations();

    /*unsigned int i=0;
    for(std::vector<unsigned int> ne: neighbors) {
        std::cout << i << ") ";
        for(unsigned int n: ne) {
            std::cout << n << ", ";
        }
        std::cout << "\n";
        i++;
    }

    for (int i = 0; i < grid.size(); ++i)
    {
       std::cout << grid[i].str() << "\n";
    }*/
}