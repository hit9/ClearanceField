// A simple dynamical minimum obstacle distance (to the right and bottom) field library on
// equal-weighted 2D grid map.
// Source code: https://github.com/hit9/true-clearance-field
// Author: hit9[At]icloud.com, License: BSD
//
// Notes:
// 1. The true-clearance-distance concept can be found at:
//    https://web.archive.org/web/20190411040123/http://aigamedev.com/open/article/clearance-based-pathfinding/
// 2. The incremental update mechanism is similar to LPAStar algorithm.
//    ref: https://en.wikipedia.org/wiki/Lifelong_Planning_A*
// 3. The initial build could be optimized by a quadtree.
//    ref: https://github.com/hit9/quadtree-hpp

#ifndef TRUE_CLEARANCE_FIELD_HPP
#define TRUE_CLEARANCE_FIELD_HPP

#include <functional>  // for std::function
#include <map>
#include <utility>  // for std::pair

namespace true_clearance_field {

static const int inf = 0x3f3f3f3f;

// ObstacleChecker is the type of the function that returns true if the given
// cell (x,y) is an obstacle.
using ObstacleChecker = std::function<bool(int x, int y)>;

// CellVisitor is a function to visit a cell (x,y).
using CellVisitor = std::function<void(int x, int y)>;

// ObstaclesQueryFunction is the type of the function that queries all obstacles's positions inside
// a given rectangle region. Where the (x1,y1) and (x2,y2) are the top-left and bottom-right
// corners of the query region. And it's ensured that we won't pass in cross-boundry coordinates.
// It's highly recommended to use a quadtree (or something similar)
// for this purpose, that's much faster than the naive way.
// A quadtree library's reference: https://github.com/hit9/quadtree-hpp
using ObstaclesQueryFunction =
    std::function<void(int x1, int y1, int x2, int y2, CellVisitor& visitor)>;

class TrueClearanceField {
  // Paramaters:
  // * w and h are the width and height of the grid map
  // * d is the upper bound of obstacle distance to maintain, usually setting this to (or larger
  //   than) the size of moving unit.
  // * costUnit is the unit cost moving from one cell to neighbour cells on horizontal and vertical
  //   directions. diagonalCostUnit is the unit cost on diagonal directions, generally it can be
  //   set to 1.414 times of costUnit. We stick to use integers instead of floating point numbers
  //   for calculations, for better performance, accuracy and convenience. So it's recommended to
  //   set costUnit to a value larger than 10.
  // * isObstacle(x,y) returns true if the cell (x,y) is an obstacle.
  TrueClearanceField(int w, int h, int d, int costUnit, int diagonalCostUnit,
                     ObstacleChecker isObstacle);
  // Returns the pre-calculated minimum distance from cell (x,y) to the nearest obstacle locating
  // in the right-bottom directions.
  // Returns inf if the distance is larger than d.
  int Get(int x, int y) const;
  // Build initials the minimum obstacle distances for the whole map.
  // We will iterate each of exisiting obstacle cells, initials their values to zeros, and expand
  // to the neighbour cells on the left and top, until all cells are initialized or the value is
  // out of bound d.
  // It's required to call Build before using any further features.
  // The obstaclesQueryFunction is optional, but it's highly recommended to use a quadtree to find
  // all existing obstacles in the map.
  // A quadtree library's reference: https://github.com/hit9/quadtree-hpp
  void Build(ObstaclesQueryFunction obstaclesQueryFunction = nullptr);
  // AddObstacle should be called after an obstacle is added at cell (x,y).
  // The nearby cells on the left-top quadrant within a distance of d will be updated.
  void AddObstacle(int x, int y);
  // RemoveObstacle should be called after an obstacle is removed from cell (x,y).
  // The nearby cells on the left-top quadrant within a distance of d will be updated.
  void RemoveObstacle(int x, int y);

 private:
  const int w, h;
  const int d, costUnit, diagonalCostUnit;
  ObstacleChecker originalIsObstacle;

  using K = std::pair<int, int>;  // { cost, z }

  // g[z] stores the old value of cell z.
  // rhs[z] stores the new value of cell z, it will be updated to g.
  // local over-consistency: g[z] > rhs[z]
  // local under-consistency: g[z] < rhs[z]
  // local consistency: g[z] == rhs[z]
  std::vector<int> g, rhs;

  // the priority queue: k => z
  // use ordered map instead std::priority_queue because we need to support "update", "erase"
  // operations.
  std::map<K, int> q;

  // ~~~~~~~~~~~ Internals ~~~~~~~~~~~~~~~
  int packxy(int x, int y);
  std::pair<int, int> packxy(int z);
  bool isObstacle(int z);
  void update(int z);
  void compute();
};

}  // namespace true_clearance_field

#endif
