#include <catch2/catch_all.hpp>

#include "ClearanceField.h"
#include "helper.hpp"

using ClearanceField::ObstacleChecker;
using ClearanceField::TrueClearanceField;

// Reference:
// https://web.archive.org/web/20190411040123/http://aigamedev.com/open/article/clearance-based-pathfinding/
TEST_CASE("simple with bound - TrueClearanceField")
{
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

	int grid[10][10] = { 0 };

	ObstacleChecker	   isObstacle = [&grid](int x, int y) { return grid[y][x]; };
	TrueClearanceField field(w, h, 4, 1, 1, isObstacle);
	field.Build();

	int expect1[10][10] = {
		// clang-format off
      {4, 4, 4, 4, 4, 4, 4, 3, 2, 1},
      {4, 4, 4, 4, 4, 4, 4, 3, 2, 1},
      {4, 4, 4, 4, 4, 4, 4, 3, 2, 1},
      {4, 4, 4, 4, 4, 4, 4, 3, 2, 1},
      {4, 4, 4, 4, 4, 4, 4, 3, 2, 1},
      {4, 4, 4, 4, 4, 4, 4, 3, 2, 1},
      {4, 4, 4, 4, 4, 4, 4, 3, 2, 1},
      {3, 3, 3, 3, 3, 3, 3, 3, 2, 1},
      {2, 2, 2, 2, 2, 2, 2, 2, 2, 1},
      {1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
		// clang-format on
	};

	FieldAssertEqOrGt<10, 10>(&field, expect1);
	Validate<10, 10, 1>(&field, isObstacle);

	for (int y = 0; y < h; ++y)
	{
		for (int x = 0; x < w; ++x)
		{
			if (G[y][x])
			{
				grid[y][x] = 1;
				field.Update(x, y);
			}
		}
	}
	field.Compute();

	int expect2[10][10] = {
		// clang-format off
    {3, 3, 3, 3, 4, 3, 2, 1, 1, 1},
    {3, 2, 2, 2, 4, 3, 2, 1, 0, 1},
    {2, 2, 1, 1, 3, 3, 2, 2, 2, 1},
    {2, 1, 1, 0, 2, 2, 2, 1, 1, 1},
    {2, 1, 0, 1, 1, 2, 1, 1, 0, 1},
    {2, 1, 0, 0, 0, 1, 1, 0, 1, 1},
    {3, 2, 1, 0, 0, 1, 0, 2, 1, 0},
    {3, 3, 2, 1, 0, 3, 3, 2, 1, 0},
    {2, 2, 2, 2, 2, 2, 2, 2, 2, 1},
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
		// clang-format on
	};
	FieldAssertEq<10, 10>(&field, expect2);
	Validate<10, 10, 1>(&field, isObstacle);
}
