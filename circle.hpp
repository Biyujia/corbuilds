#ifndef circle
#define circle

#include "point.hpp"
#include "line.hpp"
#include "basic_geometry.hpp"

struct Circle {
	Point c;
	double r;

	Circle () {
		c = Point(), r = 0.;
	}

	Circle (const Point &c_, double r_) {
		c = c_, r = r_;
	}

	Circle (const Point &A, const Point &B, const Point &C) {
		double dA = A.x * A.x + A.y * A.y, dB = B.x * B.x + B.y * B.y, dC = C.x * C.x + C.y * C.y;
		double ABx = A.x - B.x, BCx = B.x - C.x, CAx = C.x - A.x;
		double ABy = A.y - B.y, BCy = B.y - C.y, CAy = C.y - A.y;
		c = Point(0.5 * (dA * BCy + dB * CAy + dC * ABy) / (A.x * BCy + B.x * CAy + C.x * ABy),
				  0.5 * (dA * BCx + dB * CAx + dC * ABx) / (A.y * BCx + B.y * CAx + C.y * ABx));
		r = dist(c, A);
	}

	bool operator == (const Circle &o) const {
		return c == o.c && GLOBAL::cmp(r, o.r);
	}
};

inline bool point_on_circle(const Circle &C, const Point &P) {
	return GLOBAL::cmp(dist(C.c, P), C.r);
}

inline bool point_in_circle(const Circle &C, const Point &P) {
	return dist(C.c, P) < C.r - GLOBAL::eps;
}

inline bool point_on_or_in_circle(const Circle &C, const Point &P) {
	return dist(C.c, P) <= C.r + GLOBAL::eps;
}

inline double dist(const Point &P, const Circle &C) {
	return fabs(dist(P, C.c) - C.r);
}

inline double dist(const Circle &C1, const Circle &C2) {
	return dist(C1.c, C2.c) - C1.r - C2.r;
}

inline double center_dist(const Circle &C1, const Circle &C2) {
	return dist(C1.c, C2.c);
}

inline double power_of_point(const Point &P, const Circle &C) {
	double d = dist(P, C.c);
	return d * d - C.r * C.r;
}

inline double chord_length(const Circle &C, const Line &l) {
	double d = dist(C.c, l);
	if (d >= C.r - GLOBAL::eps) return 0.;
	return 2. * sqrt(C.r * C.r - d * d);
}

inline std::vector<Point> circle_line_intersection(const Circle &C, const Line &l) {
	Point fp = foot_point(C.c, l);
	double d = dist(C.c, fp);
	if (d > C.r + GLOBAL::eps) return {};
	if (GLOBAL::cmp(d, C.r)) return {fp};
	double t = sqrt(C.r * C.r - d * d);
	double dx = -l.B / sqrt(l.A * l.A + l.B * l.B);
	double dy = l.A / sqrt(l.A * l.A + l.B * l.B);
	return {Point(fp.x + dx * t, fp.y + dy * t), Point(fp.x - dx * t, fp.y - dy * t)};
}

inline std::vector<Point> circle_circle_intersection(const Circle &C1, const Circle &C2) {
	double d = dist(C1.c, C2.c);
	if (d < GLOBAL::eps && GLOBAL::cmp(C1.r, C2.r)) return {};
	if (d > C1.r + C2.r + GLOBAL::eps) return {};
	if (d < fabs(C1.r - C2.r) - GLOBAL::eps) return {};
	if (GLOBAL::cmp(d, C1.r + C2.r) || GLOBAL::cmp(d, fabs(C1.r - C2.r))) {
		Point p = line_division(C1.c, C2.c, C2.r, C1.r);
		return {p};
	}
	double a = (C1.r * C1.r - C2.r * C2.r + d * d) / (2. * d);
	double h = sqrt(C1.r * C1.r - a * a);
	Point mid = line_division(C1.c, C2.c, d - a, a);
	double dx = (C2.c.y - C1.c.y) * h / d;
	double dy = (C2.c.x - C1.c.x) * h / d;
	return {Point(mid.x - dx, mid.y + dy), Point(mid.x + dx, mid.y - dy)};
}

inline std::vector<Point> tangent_points_from_point(const Circle &C, const Point &P) {
	double d = dist(P, C.c);
	if (d < C.r - GLOBAL::eps) return {};
	if (GLOBAL::cmp(d, C.r)) return {P};
	double a = C.r * C.r / d;
	double h = C.r * sqrt(d * d - C.r * C.r) / d;
	Point mid = line_division(C.c, P, d - a, a);
	double dx = (P.y - C.c.y) * h / d;
	double dy = (P.x - C.c.x) * h / d;
	return {Point(mid.x - dx, mid.y + dy), Point(mid.x + dx, mid.y - dy)};
}

inline std::vector<Line> tangent_lines_from_point(const Circle &C, const Point &P) {
	std::vector<Point> pts = tangent_points_from_point(C, P);
	std::vector<Line> res;
	for (const Point &tp : pts)
		res.push_back(Line(P, tp));
	return res;
}

inline std::vector<Line> common_tangents(const Circle &C1, const Circle &C2) {
	std::vector<Line> res;
	double d = dist(C1.c, C2.c);
	if (d < GLOBAL::eps) return res;
	if (!GLOBAL::cmp(C1.r, C2.r)) {
		double dx = C2.c.y - C1.c.y;
		double dy = C1.c.x - C2.c.x;
		double len = sqrt(dx * dx + dy * dy);
		double nx = dx / len, ny = dy / len;
		res.push_back(Line(nx, ny, -(nx * (C1.c.x + nx * C1.r) + ny * (C1.c.y + ny * C1.r))));
		res.push_back(Line(nx, ny, -(nx * (C1.c.x - nx * C1.r) + ny * (C1.c.y - ny * C1.r))));
	}
	for (int sign = -1; sign <= 1; sign += 2) {
		double r_sum = C1.r + sign * C2.r;
		if (fabs(r_sum) >= d - GLOBAL::eps) continue;
		double a = C1.r / r_sum * d;
		double h = sqrt(d * d - r_sum * r_sum) / d * C1.r / r_sum * d;
		Point e = line_division(C1.c, C2.c, d - a, a);
		double dx = (C2.c.y - C1.c.y) * h / d;
		double dy = (C2.c.x - C1.c.x) * h / d;
		Point tp1(e.x - dx, e.y + dy), tp2(e.x + dx, e.y - dy);
		double sf = (double)sign;
		Point base2 = line_division(C1.c, C2.c, d - sf * C2.r, sf * C2.r);
		Line l1(tp1, base2), l2(tp2, base2);
		if (fabs(dist(C1.c, l1) - C1.r) < GLOBAL::eps && fabs(dist(C2.c, l1) - C2.r) < GLOBAL::eps)
			res.push_back(l1);
		if (fabs(dist(C1.c, l2) - C1.r) < GLOBAL::eps && fabs(dist(C2.c, l2) - C2.r) < GLOBAL::eps)
			res.push_back(l2);
	}
	return res;
}

inline Line radical_axis(const Circle &C1, const Circle &C2) {
	return Line(2. * (C1.c.x - C2.c.x),
				2. * (C1.c.y - C2.c.y),
				(C2.c.x * C2.c.x + C2.c.y * C2.c.y - C2.r * C2.r) -
				(C1.c.x * C1.c.x + C1.c.y * C1.c.y - C1.r * C1.r));
}

inline int circle_circle_position(const Circle &C1, const Circle &C2) {
	double d = dist(C1.c, C2.c);
	if (GLOBAL::cmp(d, 0.) && GLOBAL::cmp(C1.r, C2.r)) return 0;
	if (d > C1.r + C2.r + GLOBAL::eps) return 1;
	if (GLOBAL::cmp(d, C1.r + C2.r)) return 2;
	if (GLOBAL::cmp(d, fabs(C1.r - C2.r)) && d > GLOBAL::eps) return 3;
	if (GLOBAL::cmp(d, fabs(C1.r - C2.r))) return 4;
	if (d < fabs(C1.r - C2.r) - GLOBAL::eps) return 5;
	return 6;
}

inline Circle circle_from_diameter(const Point &A, const Point &B) {
	return Circle(line_division(A, B), dist(A, B) / 2.);
}

inline Circle circle_from_center_and_point(const Point &center, const Point &P) {
	return Circle(center, dist(center, P));
}

inline std::vector<Circle> circle_from_two_points_radius(const Point &A, const Point &B, double r) {
	double d = dist(A, B);
	if (d > 2. * r + GLOBAL::eps) return {};
	if (GLOBAL::cmp(d, 2. * r)) return {Circle(line_division(A, B), r)};
	Point mid = line_division(A, B);
	double h = sqrt(r * r - (d / 2.) * (d / 2.));
	double dx = -(B.y - A.y) * h / d;
	double dy = (B.x - A.x) * h / d;
	return {Circle(Point(mid.x + dx, mid.y + dy), r),
			Circle(Point(mid.x - dx, mid.y - dy), r)};
}

inline Point circle_inversion(const Point &P, const Circle &C) {
	double d2 = (P.x - C.c.x) * (P.x - C.c.x) + (P.y - C.c.y) * (P.y - C.c.y);
	double k = C.r * C.r / d2;
	return Point(C.c.x + k * (P.x - C.c.x), C.c.y + k * (P.y - C.c.y));
}

inline Circle circle_inversion(const Line &l, const Circle &C) {
	double v = l.A * C.c.x + l.B * C.c.y + l.C;
	double r2 = C.r * C.r;
	double A = l.A, B = l.B;
	double x0 = C.c.x, y0 = C.c.y;
	Point newC(x0 - r2 * A / (2. * v), y0 - r2 * B / (2. * v));
	double newR = r2 * sqrt(A * A + B * B) / (2. * fabs(v));
	return Circle(newC, newR);
}

inline Circle circle_inversion(const Circle &C1, const Circle &C) {
	double d2 = (C1.c.x - C.c.x) * (C1.c.x - C.c.x) + (C1.c.y - C.c.y) * (C1.c.y - C.c.y);
	double k = C.r * C.r / (d2 - C1.r * C1.r);
	Point newC(C.c.x + k * (C1.c.x - C.c.x), C.c.y + k * (C1.c.y - C.c.y));
	double newR = fabs(k) * C1.r;
	return Circle(newC, newR);
}

inline Circle apollonius_circle(const Point &A, const Point &B, double k) {
	if (GLOBAL::cmp(k, 1.)) return Circle();
	double k2 = k * k;
	double d = 1. - k2;
	Point newC((A.x - k2 * B.x) / d, (A.y - k2 * B.y) / d);
	double newR = k * dist(A, B) / fabs(d);
	return Circle(newC, newR);
}

inline Circle incircle(const Point &A, const Point &B, const Point &C) {
	double a = dist(B, C), b = dist(A, C), c = dist(A, B);
	double s = (a + b + c) / 2.;
	double r_val = sqrt((s - a) * (s - b) * (s - c) / s);
	Point incenter((a * A.x + b * B.x + c * C.x) / (a + b + c),
				   (a * A.y + b * B.y + c * C.y) / (a + b + c));
	return Circle(incenter, r_val);
}

inline Circle circumcircle(const Point &A, const Point &B, const Point &C) {
	return Circle(A, B, C);
}

inline void print(const Circle &x) {
	printf("[Circle] center=(%.6lf, %.6lf), r=%.6lf\n", x.c.x, x.c.y, x.r);
}

#endif
