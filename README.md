Clearance Field
================

Warning: It may be unstable before version 1.0.0.

A simple incremental minimum obstacle distance field library on equal-weighted 2D grid map.

Contents:

* `TrueClearanceField`: In this case, the upper-left corner should be the position of the moving agent,
    and use the side length to compare with the clearance value.

* `BrushfireClearanceField`: In this case, the center should be the position of the moving agent,
    and use the radius length to compare with the clearance value.

Reference:

* https://web.archive.org/web/20190411040123/http://aigamedev.com/open/article/clearance-based-pathfinding
* The incremental updating mechanism is inspired by LPAStar algorithm: [Lifelong_Planning_A*](https://en.wikipedia.org/wiki/Lifelong_Planning_A*)

### The origin

To solve the pathfinding problem of agents of different sizes in a 2D grid, for an example of the `TrueClearanceField`,
the 2x2 agent in the figure below always takes its upper left corner cell as its position, and then we consider
the area that this position can pass through. Each cell has a minimum obstacle distance value,
which is the distance from the current cell to the nearest obstacle in the lower right quadrant.
A cell is able to walk through when its value is not smaller than the agent's size.

![](./misc/true-clearance-field-demo1.png)

### Features in brief

* Updating is dynamical (incremental).
* Supports a distance bound, thus updates are only propagated within a limited neighborhood.

### Demos

| <!-- -->                                                      | <!-- -->                                                    |
| ------------------------------------------------------------- | ----------------------------------------------------------- |
| `TrueClearanceField` without bound:  ![](misc/true-clearance-field-demo1.gif)      | `TrueClearanceField` with bound `3`: ![](misc/true-clearance-field-demo2.gif)  |
| `TrueClearanceField` larger map with bound `4`: ![](misc/true-clearance-field-demo3.gif) | `BrushfireClearanceField` with bound `3`: ![](misc/brushfire-clearance-field-demo1.jpg) |


### Code example

How to use:

Copy away the the files inside folder `Source`.

Simple code example:

```cpp
ClearanceField::ObstacleChecker isObstacle = [](int x, int y) { return grid[y][x]; };
ClearanceField::TrueClearanceField field(w, h, 1e5, 1, 1, isObstacle);
// OR ClearanceField::BrushfireClearanceField field(w, h, 1e5, 1, 1, isObstacle);

field.Build();

// Add obstacle at (x,y).
grid[y][x] = 1;
field.Update(x, y);

// Compute after any Update(s).
field.Compute();

// Get value at (x,y)
field.Get(x, y);
```

#### Visualizer

```bash
make -C visualizer
cd visualizer
./Build/ClearanceFieldVisualizer -w 20 -h 20 -impl 1 -u 3
```

### License

BSD
