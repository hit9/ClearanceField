// Dynamical clearance field (minimum obstacle distance) on 2D grid map.
// Source code: https://github.com/hit9/clearance-field
// Author: hit9[At]icloud.com, License: BSD

// Brushfire Clearence field
// ~~~~~~~~~~~~~~~~~~~~~~~~~
// 1. brushfire references:
//      https://web.ist.utl.pt/~margaridaacferreira/RoboticsFun/
//      https://www.societyofrobots.com/programming_wavefront.shtml
// 2. The incremental updating mechanism is inspired by LPAStar algorithm.
//    ref: https://en.wikipedia.org/wiki/Lifelong_Planning_A*

#include "clearance_field_interface.hpp"

namespace clearance_field {

class BrushfireClearanceField : public IClearanceField {
 public:
  // Paramaters:
  // * w and h are the width and height of the grid map
  // * u is the upper bound of obstacle distance to maintain, usually setting this to the max value
  //   of radius of all moving units.
  // * costUnit is the unit cost moving from one cell to neighbour cells on horizontal and vertical
  //   directions. diagonalCostUnit is the unit cost on diagonal directions, generally it can be
  //   set to 1.414 times of costUnit. We stick to use integers instead of floating point numbers
  //   for calculations, for better performance, accuracy and convenience. So it's recommended to
  //   set costUnit to a value larger than 10.
  // * isObstacle(x,y) returns true if the cell (x,y) is an obstacle.

  BrushfireClearanceField(int w, int h, int u, int costUnit, int diagonalCostUnit,
                          ObstacleChecker isObstacle);

  void SetUpdatedCellVisistor(UpdatedCellVisistor f) override { updatedCellVisitor = f; }

 private:
  const int w, h;
  const int u, costUnit, diagonalCostUnit;

  UpdatedCellVisistor updatedCellVisitor = nullptr;
};

}  // namespace clearance_field
