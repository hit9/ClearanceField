// Dynamical clearance field (minimum obstacle distance) on 2D grid map.
// Source code: https://github.com/hit9/clearance-field
// Author: hit9[At]icloud.com, License: BSD

// Version: 0.2.0

// IClearanceField
// ~~~~~~~~~~~~~~~
// This is the interface of all clearance fields.

#ifndef CLEARANCE_FIELD_INTERFACE_HPP
#define CLEARANCE_FIELD_INTERFACE_HPP

#include <functional>  // for std::function

namespace clearance_field {

static const int inf = 0x3f3f3f3f;

// UpdatedCellVisistor is a function that visits the cell of which the value
// is updated by function Compute.
using UpdatedCellVisistor = std::function<void(int x, int y)>;

// ObstacleChecker is the type of the function that returns true if the given
// cell (x,y) is an obstacle.
using ObstacleChecker = std::function<bool(int x, int y)>;

// Abstract interface class of clearance fields.
// Any implementers should guarantee that the clearance values are computed incrementally.
class IClearanceField {
 public:
  // SetUpdatedCellVisistor sets a function to listen cells of which the clearance value
  // are changed by a Compute() call.
  virtual void SetUpdatedCellVisistor(UpdatedCellVisistor f) = 0;

  // Build the clearance field on an **empty** 2D grid map.
  // This method should be called before any further feature is used.
  virtual void Build() = 0;

  // Returns the clearance value at position (x,y);
  // The implementation should guarantee the returned value is not smaller than the actual value.
  // That is the specific implementers may returns unreal distance values, but the values will not
  // smaller than the real value.
  virtual int Get(int x, int y) const = 0;

  // Update should be called after an obstacle is added or removed at cell (x,y).
  virtual void Update(int x, int y) = 0;

  // Compute should be called after any changes.
  // Returns the number of cells updated.

  virtual int Compute() = 0;
};

}  // namespace clearance_field

#endif
