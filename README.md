## corbuilds

A header-only C++ library for computational geometry and TikZ visualization.

### Modules

---

### `cglobal.hpp` — Global Definitions

| Symbol | Description |
|---|---|
| `PI` | double, value `acos(-1.)` |
| `GLOBAL::eps` | default tolerance for float comparisons (`1e-8`) |
| `GLOBAL::cmp(x, y)` | returns `true` if \|x - y\| < eps |
| `GLOBAL::mpdir(dir)` | maps compass direction string (`"N"`, `"SE"` etc.) to index `0..7` |
| `GLOBAL::DelPx[8]` / `DelPy[8]` | unit offsets for 8 compass directions |

---

### `point.hpp` — Point

```cpp
struct Point { double x, y; };
```

| Function | Signature |
|---|---|
| Constructor | `Point()`, `Point(double x, double y)` |
| Equality | `p1 == p2`, `p1 != p2` |
| Vector arithmetic | `A + B`, `A - B`, `A * k`, `A / k` |
| Print | `print(p)` |

---

### `line.hpp` — Line

Represents line in general form: `Ax + By + C = 0`.

```cpp
struct Line { double A, B, C; };
```

| Function | Signature |
|---|---|
| Default | `Line()` |
| Coefficients | `Line(double A, double B, double C)` |
| From two points | `Line(Point P, Point Q)` |
| Equality | `l1 == l2`, `l1 != l2` |
| Print | `print(l)` |

---

### `basic_geometry.hpp` — Core Geometric Operations

| Function | Description |
|---|---|
| `dist(A, B)` | Euclidean distance between two points |
| `dist(P, l)` | Distance from point to line |
| `cross(A, B)` | 2D cross product `A.x*B.y - A.y*B.x` |
| `dot(A, B)` | Dot product `A.x*B.x + A.y*B.y` |
| `norm(A)` | Vector magnitude |
| `line_division(A, B, x, y)` | Point dividing segment AB at ratio `x:y` (default `1:1` = midpoint) |
| `foot_point(P, l)` | Perpendicular foot from point P to line l |
| `mirror_reflection(P, l)` | Mirror point P across line l |
| `mirror_reflection(l, l0)` | Mirror line l across line l0 |
| `intersection(l1, l2)` | Intersection point of two lines |
| `point_rotate(P, O, theta)` | Rotate point P around O by `theta` radians CCW |
| `point_translate(P, dx, dy)` | Translate point by (dx, dy) |
| `line_rotate(l, P, theta)` | Rotate line l around point P by `theta` radians CCW |
| `angle(A, B, C)` | Radian angle ∠ABC (signed, -π to π) |
| `angle(l1, l2)` | Acute radian angle between two lines (0 to π/2) |
| `perpendicular_line(l, P)` | Line perpendicular to l passing through P |
| `parallel_line(l, P)` | Line parallel to l passing through P |
| `point_on_line(P, l)` | Whether point P lies on line l |
| `point_on_segment(P, A, B)` | Whether point P lies on segment AB |
| `polar(r, theta)` | Convert polar coordinates `(r, θ)` to `Point` |
| `is_parallel(l1, l2)` | Whether two lines are parallel |
| `is_perpendicular(l1, l2)` | Whether two lines are perpendicular |
| `angle_bisector(A, B, C)` | Internal angle bisector at vertex B of ∠ABC (returns Line) |
| `angle_bisector(l1, l2)` | Both angle bisectors of two lines (returns `vector<Line>` of size 2) |
| `perpendicular_bisector(A, B)` | Perpendicular bisector of segment AB |
| `DX(l, x)`, `DY(l, y)`, `PX(l, x)`, `PY(l, y)` | Evaluate point on line by coordinate |
| `three_points_on_a_row(A, B, C)` | Whether A, B, C are on the same line |
| `three_lines_intersect(l1, l2, l3)` | Whether l1, l2, l3 intersect on the same point |

---

### `triangle.hpp` (namespace `tri`)

| Function | Description |
|---|---|
| `area(A, B, C)` | Signed area of triangle ABC |
| `G(A, B, C)` | Centroid |
| `I(A, B, C)` | Incenter |
| `O(A, B, C)` | Circumcenter |
| `H(A, B, C)` | Orthocenter |
| `N9(A, B, C)` | Nine-point circle center (midpoint of O and H) |
| `R(A, B, C)` | Circumradius |
| `r(A, B, C)` | Inradius |
| `N(A, B, C)` | Nagel point |
| `Ex(A, B, C)` | Three excenters (returns `vector<Point>` of size 3) |
| `K(A, B, C)` | Symmedian point (Lemoine) |
| `Ge(A, B, C)` | Gergonne point |
| `Sp(A, B, C)` | Spieker center |
| `Fe(A, B, C)` | Feuerbach point (incircle tangency with nine-point circle) |
| `barycentric(P, A, B, C)` | Barycentric coordinates `(α, β, γ)` of P w.r.t. triangle ABC |
| `isogonal_conjugate(P, A, B, C)` | Isogonal conjugate of point P w.r.t. triangle ABC |

---

### `circle.hpp`

#### Circle struct

```cpp
struct Circle { Point c; double r; };
```

| Constructor | Description |
|---|---|
| `Circle()` | Unitialized |
| `Circle(Point c, double r)` | From center and radius |
| `Circle(A, B, C)` | Circumcircle of triangle ABC |

#### Functions

| Function | Description |
|---|---|
| `point_on_circle(C, P)` | Whether point P lies on the circle |
| `point_in_circle(C, P)` | Whether point P is strictly inside |
| `point_on_or_in_circle(C, P)` | Whether point P is on or inside |
| `dist(P, C)` | Distance from point to circle |
| `dist(C1, C2)` | Shortest distance between two circles |
| `center_dist(C1, C2)` | Distance between circle centers |
| `power_of_point(P, C)` | Power of point w.r.t. circle |
| `chord_length(C, l)` | Length of chord formed by line l crossing circle C |
| `radical_axis(C1, C2)` | Radical axis of two circles |
| `circle_line_intersection(C, l)` | Intersection points (0, 1, or 2); returns `vector<Point>` |
| `circle_circle_intersection(C1, C2)` | Intersection points; returns `vector<Point>` |
| `tangent_points_from_point(C, P)` | Tangent points from external point P to circle C |
| `tangent_lines_from_point(C, P)` | Tangent lines from external point P to circle C |
| `common_tangents(C1, C2)` | All common tangent lines of two circles |
| `circle_circle_position(C1, C2)` | Returns int: 0=coincident, 1=separate, 2=externally tangent, 3=intersecting, 4=internally tangent, 5=contained, 6=concentric |
| `circle_from_diameter(A, B)` | Circle with AB as diameter |
| `circle_from_center_and_point(center, P)` | Circle with given center passing through P |
| `circle_from_two_points_radius(A, B, r)` | Circle(s) through A, B with given radius (0, 1, or 2); returns `vector<Circle>` |
| `circle_inversion(P, C)` | Inverse of point P w.r.t. circle C |
| `circle_inversion(l, C)` | Inverse of line l w.r.t. circle C (returns Circle unless line passes through center) |
| `circle_inversion(C1, C)` | Inverse of circle C1 w.r.t. circle C |
| `apollonius_circle(A, B, k)` | Apollonius circle: set of points P where `PA/PB = k` |
| `incircle(A, B, C)` | Incircle of triangle ABC |
| `circumcircle(A, B, C)` | Circumcircle of triangle ABC (wrapper for `Circle(A,B,C)`) |
| `print(C)` | Print circle info |

#### Return value conventions

- `circle_circle_position` returns: `0` coincident, `1` separate, `2` externally tangent, `3` intersecting, `4` internally tangent, `5` contained, `6` concentric.
- `circle_line_intersection` and `circle_circle_intersection` return empty vector if no intersection, 1 element for tangency, 2 elements for proper intersection.

---

### `function.hpp`

| Function | Description |
|---|---|
| `bezier_decomposition(f, a, b, n)` | Approximate function `f` over `[a, b]` with `n-1` cubic Bezier segments; returns `vector<bezier3>` (each element is `array<Point, 4>`) |
| `derivative(f, x, h)` | The derivative of `f` at point `x` with measuring step `h` | 
| `derivative5(f, x, h)` | A more precise derivative of `f` at point `x`, using 5-point sample method with measuring step `h` |
| `integral_simpson(f, a, b, n)` | The integral of `f` in the interval `[a, b]` using the Simpson method with `n` steps |
| `integral_riemann(f, a, b, n)` | The integral of `f` in the interval `[a, b]` using the definition of Riemann integral with `n` steps |
| `tangent_line(f, x0)` | The tangent line of function `f` at point `(x0,f(x0))` |
| `normal_line(f, x0)` | The normal line of function `f` at point `(x0,f(x0))` |
| `find_roots(f, a, b, tol)` | The roots of function `f` in the interval `[a, b]` with a tolerance value `tol` | 

---

### `extra.hpp`

| Function | Description |
|---|---|
| `Nod(P, dir, Delt)` | Offset point P in compass direction `dir` by distance `Delt` (default `0.4`) |
| `print(args...)` | Variadic printf-style output (overloads for `Point`, `Line`, `Circle`, `double`, `string`) |

---

### `tikz.hpp` — TikZ Rendering

Class `tikz_picture` (LaTeX output via TikZ). Automatically loads `amsmath, amssymb` and sets up a background layer so fills/shades never obscure lines or labels.

| Field | Default | Description |
|---|---|---|
| `global_scale` | `1.0` | Scale entire picture (set before `open_file()`) |
| `node_size` | `0.4` | Label offset from points |
| `stress_size` | `0.05` | Dot radius |

| Method | Description |
|---|---|
| `set_file_name(s)` | Set output `.tex` filename |
| `set_node_size(n)` | Set node label offset |
| `set_stress_size(s)` | Set dot size |
| `open_file()` | Write LaTeX preamble and begin `tikzpicture` |
| `close()` | End `tikzpicture` and document |
| `set_axis(xl,xr,yd,yu)` | Draw coordinate axes |
| `set_grid(xl,xr,yd,yu)` | Draw coordinate grid (step via `grid_step` field) |
| `set_axis_grid(...)` | Combined axes + grid |
| `match_point(pts, cycle)` | Draw polyline through points; `cycle=true` closes the path |
| `match_point(vpts, cycle)` | Batch draw multiple polylines |
| `node(P, dir, label)` | Place labeled node near point P (offset by `dir`) |
| `node(vP, vDir, vLabel)` | Batch node placement |
| `stress(P)` | Draw a filled dot at point P |
| `stress(vP)` | Batch stress points |
| `draw_line(l, l_l, l_r, ...)` | Draw segment of line l |
| `draw_func(f, a, b, n)` | Plot function using Bezier decomposition |
| `compile()` | Run `pdflatex` on the output file |
| `run()` | `close()` then `compile()` |

---

### Example 1: Triangle Centers and the Euler Line

This example computes all major triangle centers, draws the Euler line (through O, G, H, N9), and highlights the Nagel and Gergonne points on the incircle.

```cpp
#include "corbuilds.hpp"

int main() {
    Point A(0, 0), B(8, 0), C(2, 6);

    Point G  = tri::G(A, B, C);
    Point I  = tri::I(A, B, C);
    Point O  = tri::O(A, B, C);
    Point H  = tri::H(A, B, C);
    Point N9 = tri::N9(A, B, C);
    Point N  = tri::N(A, B, C);
    Point Ge = tri::Ge(A, B, C);
    Point K  = tri::K(A, B, C);
    Point Sp = tri::Sp(A, B, C);

    printf("Triangle: A(0,0) B(8,0) C(2,6)\n");
    print(G);  print(I);  print(O);  print(H);
    print(N9); print(N);  print(Ge); print(K); print(Sp);

    printf("Euler line: (%.6f,%.6f) -- (%.6f,%.6f)\n", O.x, O.y, H.x, H.y);
    printf("ON9 / N9H ratio = %.6f\n", dist(O, N9) / dist(N9, H));

    vector<Point> ex = tri::Ex(A, B, C);
    printf("Excenters: ");
    for (auto& e : ex) printf("(%.3f,%.3f) ", e.x, e.y);
    printf("\n");

    Point P_in(3, 2);
    Point P_iso = tri::isogonal_conjugate(P_in, A, B, C);
    printf("Isogonal conjugate of (3,2): (%.6f, %.6f)\n", P_iso.x, P_iso.y);

    return 0;
}
```

---

### Example 2: Inversive Geometry — Circle Inversion and Apollonius Circles

This example demonstrates circle inversion and the Apollonius circle, which are fundamental tools in inversive geometry for studying angle-preserving transformations and coaxal circle families.

```cpp
#include "corbuilds.hpp"

int main() {
    Point O(0, 0);
    Circle C_inv(O, 4);  // inversion circle

    printf("=== Circle Inversion (radius=4) ===\n");

    Point Q(8, 0);
    Point invQ = circle_inversion(Q, C_inv);
    printf("Q(8,0) -> (%g, %g)\n", invQ.x, invQ.y);
    printf("|OQ|*|invQ| = %g (expect 16)\n", dist(O, Q) * dist(O, invQ));

    Line l(1, 0, -1);
    Circle invLine = circle_inversion(l, C_inv);
    printf("Line x=1 inverted to circle: (%.6f,%.6f) r=%.6f\n",
           invLine.c.x, invLine.c.y, invLine.r);
    printf("Inverted circle passes through O: %d\n", point_on_circle(invLine, O));

    Circle C2(Point(7, 0), 2);
    Circle invC2 = circle_inversion(C2, C_inv);
    printf("Circle (7,0) r=2 inverted: (%.6f,%.6f) r=%.6f\n",
           invC2.c.x, invC2.c.y, invC2.r);

    printf("\n=== Apollonius Circle ===\n");

    Point P1(0, 0), P2(8, 0);
    for (double k : {0.5, 2.0}) {
        Circle ac = apollonius_circle(P1, P2, k);
        printf("k=%.1f: center=(%.3f,%.3f) radius=%.3f\n", k, ac.c.x, ac.c.y, ac.r);
        Point test_p = Point(ac.c.x + ac.r, 0);
        double r1 = dist(test_p, P1), r2 = dist(test_p, P2);
        printf("  verify: %.3f / %.3f = %.3f\n", r1, r2, r1 / r2);
    }

    return 0;
}
```

---

### Dependency Graph

```
cglobal.hpp
  |
point.hpp
  |
line.hpp
  |
basic_geometry.hpp
  |
  +-- triangle.hpp
  |
  +-- circle.hpp
  |
  +-- corbuilds_function.hpp
  |
  +-- corbuilds.hpp (master include)
        |
        +-- extra.hpp
              |
              +-- tikz.hpp
```