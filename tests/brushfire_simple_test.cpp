#include <catch2/catch_all.hpp>

#include "clearance_field.hpp"
#include "helper.hpp"

using clearance_field::BrushfireClearanceField;
using clearance_field::ObstacleChecker;

TEST_CASE("simple - BrushfireClearanceField") {
  int w = 10, h = 10;

  int G[10][10] = {
      // clang-format off
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 1, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 1, 0, 0, 0, 0, 0, 0},
    {0, 0, 1, 0, 0, 0, 0, 0, 1, 0},
    {0, 0, 1, 1, 1, 0, 0, 1, 0, 0},
    {0, 0, 0, 1, 1, 0, 1, 0, 0, 1},
    {0, 0, 0, 0, 1, 0, 0, 0, 0, 1},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
      // clang-format on
  };

  int grid[10][10] = {0};

  ObstacleChecker isObstacle = [&grid](int x, int y) { return grid[y][x]; };
  BrushfireClearanceField field(w, h, 1024, 1, 1, isObstacle);
  field.Build();

  int expect1[10][10] = {
      // clang-format off
      {1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
      {1, 2, 2, 2, 2, 2, 2, 2, 2, 1},
      {1, 2, 3, 3, 3, 3, 3, 3, 2, 1},
      {1, 2, 3, 4, 4, 4, 4, 3, 2, 1},
      {1, 2, 3, 4, 5, 5, 4, 3, 2, 1},
      {1, 2, 3, 4, 5, 5, 4, 3, 2, 1},
      {1, 2, 3, 4, 4, 4, 4, 3, 2, 1},
      {1, 2, 3, 3, 3, 3, 3, 3, 2, 1},
      {1, 2, 2, 2, 2, 2, 2, 2, 2, 1},
      {1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
      // clang-format on
  };
  FieldAssertEq<10, 10>(&field, expect1);

  for (int y = 0; y < h; ++y) {
    for (int x = 0; x < w; ++x) {
      if (G[y][x]) {
        grid[y][x] = 1;
        field.Update(x, y);
      }
    }
  }
  field.Compute();

  int expect2[10][10] = {
      // clang-format off
      {1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
      {1, 2, 2, 2, 2, 2, 2, 1, 0, 1},
      {1, 2, 1, 1, 1, 2, 2, 1, 1, 1},
      {1, 1, 1, 0, 1, 2, 2, 1, 1, 1},
      {1, 1, 0, 1, 1, 1, 1, 1, 0, 1},
      {1, 1, 0, 0, 0, 1, 1, 0, 1, 1},
      {1, 1, 1, 0, 0, 1, 0, 1, 1, 0},
      {1, 2, 1, 1, 0, 1, 1, 1, 1, 0},
      {1, 2, 2, 1, 1, 1, 2, 2, 1, 1},
      {1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
      // clang-format on
  };
  FieldAssertEq<10, 10>(&field, expect2);
}
