#ifndef basic_geometry
#define basic_geometry

#include "point.hpp"
#include "line.hpp"

inline double dist(const Point &A, const Point &B) {
	return sqrt((A.x - B.x) * (A.x - B.x) + (A.y - B.y) * (A.y - B.y));
}

inline double cross(const Point &A, const Point &B) {
	return A.x * B.y - A.y * B.x;
}

inline double dot(const Point &A, const Point &B) {
	return A.x * B.x + A.y * B.y;
}

inline double norm(const Point &A) {
	return sqrt(A.x * A.x + A.y * A.y);
}

inline double dist(const Point &P, const Line &l) {
	return fabs(l.A * P.x + l.B * P.y + l.C) / sqrt(l.A * l.A + l.B * l.B);
}

inline Point line_division(const Point &A, const Point &B, double x = 1., double y = 1.) { 
	return Point((A.x * y + B.x * x) / (x + y), (A.y * y + B.y * x) / (x + y));
}

inline Point foot_point(const Point &P, const Line &l) {
	double A = l.A, B = l.B, C = l.C, x = P.x, y = P.y;
	return Point((B * B * x - A * B * y - A * C) / (A * A + B * B),
				 (A * A * y - A * B * x - B * C) / (A * A + B * B)
				);
}

inline Point mirror_reflection(const Point &P, const Line &l) {
	double A = l.A, B = l.B, C = l.C, x = P.x, y = P.y;
	return Point(((B * B - A * A) * x - 2. * A * B * y - 2. * A * C) / (A * A + B * B),
				 ((A * A - B * B) * y - 2. * A * B * x - 2. * B * C) / (A * A + B * B)				
				);
}

inline Point point_rotate(const Point &P, const Point &O, double theta) {
	double dx = P.x - O.x, dy = P.y - O.y;
	double c = cos(theta), s = sin(theta);
	return Point(O.x + dx * c - dy * s, O.y + dx * s + dy * c);
}

inline Point point_translate(const Point &P, double dx, double dy) {
	return Point(P.x + dx, P.y + dy);
}

inline Line line_rotate(const Line &l, const Point &P, double theta) {
	double c = cos(theta), s = sin(theta);
	double newA = l.A * c - l.B * s;
	double newB = l.A * s + l.B * c;
	double newC = l.C + (1. - c) * (l.A * P.x + l.B * P.y) + s * (l.B * P.x - l.A * P.y);
	return Line(newA, newB, newC);
}

inline double DX(const Line &l, const double &x) { return - (l.A * x + l.C) / l.B; }
inline double DY(const Line &l, const double &y) { return - (l.B * y + l.C) / l.A; }
inline Point PX(const Line &l, const double &x) { return Point(x, DX(l, x)); }
inline Point PY(const Line &l, const double &y) { return Point(DY(l, y), y); }

inline Line mirror_reflection(const Line &l, const Line &l0) {
	double L = l0.A, M = l0.B, N = l0.C, A = l.A, B = l.B, C = l.C;
	return Line(2 * A * B * M + L * (A * A - B * B),
				2 * A * B * L + M * (B * B - A * A),
				2 * A * C * L + 2 * B * C * M - N * (A * A + B * B)
			   );
}

inline Point intersection(const Line &A, const Line &B) {
	double a = A.A, b = A.B, c = B.A, d = B.B, e = -A.C, f = -B.C;
	double det = a * d - b * c;
	return Point((d * e - b * f) / det, (a * f - c * e) / det); 
}

inline double angle(const Point &A, const Point &B, const Point &C) {
	Point BA = A - B, BC = C - B;
	return atan2(cross(BA, BC), dot(BA, BC));
}

inline double angle(const Line &l1, const Line &l2) {
	double d1 = sqrt(l1.A * l1.A + l1.B * l1.B);
	double d2 = sqrt(l2.A * l2.A + l2.B * l2.B);
	double cosv = fabs(l1.A * l2.A + l1.B * l2.B) / (d1 * d2);
	if (cosv > 1.) cosv = 1.;
	return acos(cosv);
}

inline Line perpendicular_line(const Line &l, const Point &P) {
	return Line(-l.B, l.A, l.B * P.x - l.A * P.y);
}

inline Line parallel_line(const Line &l, const Point &P) {
	return Line(l.A, l.B, -(l.A * P.x + l.B * P.y));
}

inline bool point_on_line(const Point &P, const Line &l) {
	return GLOBAL::cmp(l.A * P.x + l.B * P.y + l.C, 0.);
}

inline bool point_on_segment(const Point &P, const Point &A, const Point &B) {
	if (!GLOBAL::cmp(cross(B - A, P - A), 0.)) return false;
	double t = dot(P - A, B - A) / dot(B - A, B - A);
	return t >= -GLOBAL::eps && t <= 1. + GLOBAL::eps;
}

inline Point polar(double r, double theta) {
	return Point(r * cos(theta), r * sin(theta));
}

inline bool is_parallel(const Line &l1, const Line &l2) {
	return GLOBAL::cmp(l1.A * l2.B, l1.B * l2.A);
}

inline bool is_perpendicular(const Line &l1, const Line &l2) {
	return GLOBAL::cmp(l1.A * l2.A + l1.B * l2.B, 0.);
}

inline Line angle_bisector(const Point &A, const Point &B, const Point &C) {
	Point BA = A - B, BC = C - B;
	double d1 = norm(BA), d2 = norm(BC);
	Point dir = BA / d1 + BC / d2;
	Line l(B, B + dir);
	return l;
}

inline std::vector<Line> angle_bisector(const Line &l1, const Line &l2) {
	double d1 = sqrt(l1.A * l1.A + l1.B * l1.B);
	double d2 = sqrt(l2.A * l2.A + l2.B * l2.B);
	double a1 = l1.A / d1, b1 = l1.B / d1, c1 = l1.C / d1;
	double a2 = l2.A / d2, b2 = l2.B / d2, c2 = l2.C / d2;
	std::vector<Line> res;
	res.push_back(Line(a1 - a2, b1 - b2, c1 - c2));
	res.push_back(Line(a1 + a2, b1 + b2, c1 + c2));
	return res;
}

inline Line perpendicular_bisector(const Point &A, const Point &B) {
	Point mid = line_division(A, B);
	return Line(B.x - A.x, B.y - A.y, -(B.x - A.x) * mid.x - (B.y - A.y) * mid.y);
}

inline bool three_points_on_a_row(const Point &A, const Point &B, const Point &C) {
	return GLOBAL::cmp((A.x - B.x) * (C.y - B.y), (A.y - B.y) * (C.x - B.x));
}

inline bool three_lines_intersect(const Line &l1, const Line &l2, const Line &l3) {
	Point A1 = intersection(l1, l2), A2 = intersection(l1, l3);
	return dist(A1, A2) < GLOBAL::eps;
}

#endif
