#include <bits/stdc++.h>
#include "../corbuilds.hpp"

using namespace std;

double f(double x) {return 4. + sin(x); }

const double INF = 1e15;

// directed cross ratio function
double cross_ratio(Point A, Point B, Point C) {
    return !three_points_on_a_row(A, B, C) ? INF 
           : GLOBAL::cmp(A.x, B.x) ? (A.y - C.y) / (C.y - B.y)
           : (A.x - C.x) / (C.x - B.x);
}

int main() {
    Point A = Point(), B = Point(5., 0.);
    double a = -1., b = 6.;
    int n = 5;
    double h = (b - a) / (1. * n);
    Line l = Line(Point(-1, 0), Point(0, 7));
    // Menelaus theorem experiment
    printf("Menelaus theorem verification: \n");
    for (int i = 0; i < n; i++) {
        printf("\nExperiment #%d:\n", i + 1);
        double x = a + i * h;
        Point C = Point(x, f(x));
        Line AB = Line(A, B), BC = Line(B, C), AC = Line(A, C);
        Point pAB = intersection(AB, l), pBC = intersection(BC, l), pAC = intersection(AC, l);
        double AB_ratio = cross_ratio(A, B, pAB), CA_ratio = cross_ratio(C, A, pAC), BC_ratio = cross_ratio(B, C, pBC);
        print(A, B, C);
        printf("AB_ratio, BC_ratio, CA_ratio = %.6lf, %.6lf, %.6lf\n", AB_ratio, BC_ratio, CA_ratio);
        printf("Product = %.6lf\n", AB_ratio * BC_ratio * CA_ratio); 
    }

    return 0;
}