#include <bits/stdc++.h>
#include "../tikz.hpp"

using namespace std;

int main() {
    tikz_picture pic;
    pic.set_file_name("imo_triangle.tex");
    pic.node_size = 0.3;
    pic.default_angle_radius = 0.3;
    pic.axis_draw_O = 0;

    pic.open_file();
    // set up the axis and the grid
    pic.set_axis_grid(-1, 11, -1, 10, "SW", 0.2);

    Point A(0, 0), B(8, 0), C(2, 6);

    pic.draw_polygon({A, B, C}, true, "thick");

    Point G = tri::G(A, B, C);
    Point I = tri::I(A, B, C);
    Point O = tri::O(A, B, C);
    Point H = tri::H(A, B, C);
    Point N9 = tri::N9(A, B, C);

    Circle inC = incircle(A, B, C);
    Circle circC = circumcircle(A, B, C);
    pic.draw_circle(inC, "blue");
    pic.draw_circle(circC, "red, thin");
    pic.draw_dashed_circle(Circle(N9, tri::R(A, B, C)/2));

    // Euler line
    pic.draw_segment(O, H, "thick, green!60!black");

    pic.draw_point(G, "orange");
    pic.draw_point(I, "blue");
    pic.draw_point(O, "red");
    pic.draw_point(H, "purple");
    pic.draw_point(N9, "teal");

    // labels
    pic.label_math({A, B, C, G, I, O, H, N9},
        {"A", "B", "C", "G", "I", "O", "H", "N_9"},
        {"SW","SE","N", "N", "N", "N", "N", "S"});

    // angles
    pic.draw_angle_arc(C, A, B, 0.25, "blue, thin");
    pic.draw_angle_arc(A, B, C, 0.35, "blue, thin");

    pic.run();

    return 0;
}