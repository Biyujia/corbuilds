#include <bits/stdc++.h>
#include "../tikz.hpp"

using namespace std;

double f(double x) { return sin(x); }
const double pi = acos(-1.);

int main() {
    tikz_picture pic;
    pic.global_scale = 2.0;
    pic.node_size = 0.2;

    pic.open_file();
    
    pic.set_axis_grid(-1., 5., -1.5, 1.5, "NW");
    
    pic.draw_func(f, -0.5, +4.5, 100);

    int n = 10;

    const double h = pi / (1. * n);
    for (int i = 0; i < n; i++) {
        double x1 = i * h;
        pic.filldraw_rectangle(Point(x1, 0), Point(x1 + h, f(x1)), "blue", "blue", 0.5);
    }

    pic.formula(Point(4, 0.75), "y = \\sin(x)");

    pic.run();

    return 0;
}