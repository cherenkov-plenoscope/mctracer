// Copyright 2014 Sebastian A. Mueller
#ifndef SCENERY_GEOMETRY_GRIDNEIGHBORHOODTOPOLIGY_H_
#define SCENERY_GEOMETRY_GRIDNEIGHBORHOODTOPOLIGY_H_

#include <string>
#include <vector>
#include "Core/Vec3.h"
#include "HexagonalPrismZ.h"

class GridNeighborhoodTopoligy{
    const std::vector<Vec3> *grid;
    double neighborhood_radius;
    std::vector<unsigned int> idx_x;
    std::vector<std::vector<unsigned int>> neigbors;
 public:
    GridNeighborhoodTopoligy(
        const std::vector<Vec3> *grid,
        const double neighborhood_radius);
    std::vector<std::vector<unsigned int>> get_neighbor_relations()const;
 private:
    void init_idx();
    void sort_idx();
    unsigned int get_candidate_low_x(const double x);
    unsigned int get_candidate_hig_x(const double x);
};

#endif  // SCENERY_GEOMETRY_GRIDNEIGHBORHOODTOPOLIGY_H_
