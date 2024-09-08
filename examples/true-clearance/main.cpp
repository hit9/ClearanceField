#include <iomanip>
#include <iostream>

#include "clearance_field_interface.hpp"
#include "impls/true_clearance_field.hpp"

const int w = 10, h = 10;

int G[w][h] = {
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

int grid[w][h] = {0};

int main() {
  clearance_field::ObstacleChecker isObstacle = [](int x, int y) { return grid[x][y]; };
  clearance_field::TrueClearanceField field(w, h, 10000, 1, 1, isObstacle);

  auto inspect = [&field]() {
    for (int x = 0; x < h; ++x) {
      for (int y = 0; y < w; ++y) {
        int v = field.Get(x, y);
        std::cout << std::setw(4);
        if (v == clearance_field::inf)
          std::cout << "inf";
        else
          std::cout << v;
        std::cout << " ";
      }
      std::cout << std::endl;
    }
  };

  std::cout << "building .." << std::endl;

  field.Build();

  inspect();

  std::cout << "updating .." << std::endl;

  for (int x = 0; x < h; ++x) {
    for (int y = 0; y < w; ++y) {
      if (G[x][y]) {
        grid[x][y] = 1;
        field.Update(x, y);
      }
    }
  }

  field.Compute();

  inspect();

  return 0;
}
