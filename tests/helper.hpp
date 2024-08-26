#ifndef TRUE_CLEARANCE_FIELD_TEST_HELPER_HPP
#define TRUE_CLEARANCE_FIELD_TEST_HELPER_HPP

#include <catch2/catch_all.hpp>

#include "true_clearance_field.hpp"

// Should field equals to expect.
template <int w, int h>
void FieldAssertEq(true_clearance_field::TrueClearanceField& field, int expects[w][h]) {
  for (int x = 0; x < h; ++x) {
    for (int y = 0; y < w; ++y) {
      REQUIRE(expects[x][y] == field.Get(x, y));
    }
  }
}

// Should field not less than expect.
template <int w, int h>
void FieldAssertEqOrGt(true_clearance_field::TrueClearanceField& field, int expects[w][h]) {
  for (int x = 0; x < h; ++x) {
    for (int y = 0; y < w; ++y) {
      REQUIRE(expects[x][y] <= field.Get(x, y));
    }
  }
}

template <int w, int h, int costUnit>
void Validate(true_clearance_field::TrueClearanceField& field,
              true_clearance_field::ObstacleChecker isObstacle) {
  // Very naive helper function to test if a clearance field is valid.
  for (int x = 0; x < h; ++x) {
    for (int y = 0; y < w; ++y) {
      int dist = field.Get(x, y);
      // there should be no obstacles inside [(x,y), (x+dist-1, y+dist-1)]
      int x1, y1;
      for (x1 = x; x1 < std::min(h, x + dist / costUnit); ++x1) {
        for (y1 = y; y1 < std::min(w, y + dist / costUnit); ++y1) {
          REQUIRE(!isObstacle(x1, y1));
        }
      }
      // there should exist at least one obstacle or cross the bound on x1,y1 borders.
      bool foundObstaclesOrBound = (x1 == h) || (y1 == w);
      if (!foundObstaclesOrBound) {
        for (int x2 = x1; x2 >= 0; --x2) {
          if (isObstacle(x2, y1)) {
            foundObstaclesOrBound = true;
            break;
          }
        }
      }
      if (!foundObstaclesOrBound) {
        for (int y2 = y1; y2 >= 0; --y2) {
          if (isObstacle(x1, y2)) {
            foundObstaclesOrBound = true;
            break;
          }
        }
      }
      REQUIRE(foundObstaclesOrBound);
    }
  }
}

#endif
