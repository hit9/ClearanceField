#include <iomanip>
#include <iostream>

#include "clearance_field.hpp"

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
  clearance_field::ObstacleChecker isObstacle = [](int x, int y) { return grid[y][x]; };
  clearance_field::TrueClearanceField field(w, h, 10000, 1, 1, isObstacle);

  auto inspect = [&field]() {
    for (int y = 0; y < h; ++y) {
      for (int x = 0; x < w; ++x) {
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

  for (int y = 0; y < h; ++y) {
    for (int x = 0; x < w; ++x) {
      if (G[y][x]) {
        grid[y][x] = 1;
        field.Update(x, y);
      }
    }
  }

  field.Compute();

  inspect();

  return 0;
}
