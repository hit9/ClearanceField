#ifndef TRUE_CLEARANCE_FIELD_TEST_HELPER_HPP
#define TRUE_CLEARANCE_FIELD_TEST_HELPER_HPP

#include "true_clearance_field.hpp"

// Should field equals to expect.
template <int w, int h>
void FieldAssertEq(int expects[w][h], true_clearance_field::TrueClearanceField& field) {
  for (int x = 0; x < h; ++x) {
    for (int y = 0; y < w; ++y) {
      REQUIRE(expects[x][y] == field.Get(x, y));
    }
  }
}

#endif
