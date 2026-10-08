#ifndef triangle
#define triangle

#include "point.hpp"
#include "line.hpp"
#include "basic_geometry.hpp"

namespace tri {

inline double area(const Point &A, const Point &B, const Point &C) {
	return 0.5 * fabs(cross(B - A, C - A));
}

inline double r(const Point &A, const Point &B, const Point &C) {
	double a = dist(B, C), b = dist(A, C), c = dist(A, B);
	double s = (a + b + c) / 2.;
	return area(A, B, C) / s;
}

inline Point G(const Point &A, const Point &B, const Point &C) {
	return Point((A.x + B.x + C.x) / 3., (A.y + B.y + C.y) / 3.);
}

inline Point I(const Point &A, const Point &B, const Point &C) {
	double a = dist(B, C), b = dist(A, C), c = dist(A, B);
	return Point((a * A.x + b * B.x + c * C.x) / (a + b + c), 
				 (a * A.y + b * B.y + c * C.y) / (a + b + c));
}

inline Point O(const Point &A, const Point &B, const Point &C) {
	double dA = A.x * A.x + A.y * A.y, dB = B.x * B.x + B.y * B.y, dC = C.x * C.x + C.y * C.y;
	double ABx = A.x - B.x, BCx = B.x - C.x, CAx = C.x - A.x;
	double ABy = A.y - B.y, BCy = B.y - C.y, CAy = C.y - A.y;
	return Point(0.5 * (dA * BCy + dB * CAy + dC * ABy) / (A.x * BCy + B.x * CAy + C.x * ABy),
				 0.5 * (dA * BCx + dB * CAx + dC * ABx) / (A.y * BCx + B.y * CAx + C.y * ABx));
}

inline Point H(const Point &A, const Point &B, const Point &C) {
	double dA = A.x * A.x + A.y * A.y, dB = B.x * B.x + B.y * B.y, dC = C.x * C.x + C.y * C.y;
	double ABx = A.x - B.x, BCx = B.x - C.x, CAx = C.x - A.x;
	double ABy = A.y - B.y, BCy = B.y - C.y, CAy = C.y - A.y;
	return Point(A.x + B.x + C.x - (dA * BCy + dB * CAy + dC * ABy) / (A.x * BCy + B.x * CAy + C.x * ABy),
				 A.y + B.y + C.y - (dA * BCx + dB * CAx + dC * ABx) / (A.y * BCx + B.y * CAx + C.y * ABx));
}

inline Point N9(const Point &A, const Point &B, const Point &C) {
	Point o = O(A, B, C), h = H(A, B, C);
	return line_division(o, h);
}

inline double R(const Point &A, const Point &B, const Point &C) {
	return dist(O(A, B, C), A);
}

inline Point N(const Point &A, const Point &B, const Point &C) {
	double a = dist(B, C), b = dist(A, C), c = dist(A, B);
	double s = (a + b + c) / 2.;
	double sa = s - a, sb = s - b, sc = s - c;
	return Point((sa * A.x + sb * B.x + sc * C.x) / s,
				 (sa * A.y + sb * B.y + sc * C.y) / s);
}

inline std::vector<Point> Ex(const Point &A, const Point &B, const Point &C) {
	double a = dist(B, C), b = dist(A, C), c = dist(A, B);
	double denomA = -a + b + c, denomB = a - b + c, denomC = a + b - c;
	Point ExA((-a * A.x + b * B.x + c * C.x) / denomA,
			  (-a * A.y + b * B.y + c * C.y) / denomA);
	Point ExB(( a * A.x - b * B.x + c * C.x) / denomB,
			  ( a * A.y - b * B.y + c * C.y) / denomB);
	Point ExC(( a * A.x + b * B.x - c * C.x) / denomC,
			  ( a * A.y + b * B.y - c * C.y) / denomC);
	return {ExA, ExB, ExC};
}

inline Point K(const Point &A, const Point &B, const Point &C) {
	double a = dist(B, C), b = dist(A, C), c = dist(A, B);
	double a2 = a * a, b2 = b * b, c2 = c * c;
	double sum = a2 + b2 + c2;
	return Point((a2 * A.x + b2 * B.x + c2 * C.x) / sum,
				 (a2 * A.y + b2 * B.y + c2 * C.y) / sum);
}

inline Point Ge(const Point &A, const Point &B, const Point &C) {
	double a = dist(B, C), b = dist(A, C), c = dist(A, B);
	double s = (a + b + c) / 2.;
	double wa = 1. / (s - a), wb = 1. / (s - b), wc = 1. / (s - c);
	double sum = wa + wb + wc;
	return Point((wa * A.x + wb * B.x + wc * C.x) / sum,
				 (wa * A.y + wb * B.y + wc * C.y) / sum);
}

inline Point Sp(const Point &A, const Point &B, const Point &C) {
	double a = dist(B, C), b = dist(A, C), c = dist(A, B);
	double wa = b + c, wb = c + a, wc = a + b;
	double sum = wa + wb + wc;
	return Point((wa * A.x + wb * B.x + wc * C.x) / sum,
				 (wa * A.y + wb * B.y + wc * C.y) / sum);
}

inline std::vector<double> barycentric(const Point &P, const Point &A, const Point &B, const Point &C) {
	double T = cross(A - B, C - B);
	double alpha = cross(P - B, C - B) / T;
	double beta = -cross(A - C, P - C) / T;
	double gamma = 1. - alpha - beta;
	return {alpha, beta, gamma};
}

inline Point Fe(const Point &A, const Point &B, const Point &C) {
	double a = dist(B, C), b = dist(A, C), c = dist(A, B);
	double s = (a + b + c) / 2.;
	double sa = s - a, sb = s - b, sc = s - c;
	double wa = sa * (b - c) * (b - c);
	double wb = sb * (c - a) * (c - a);
	double wc = sc * (a - b) * (a - b);
	double sum = wa + wb + wc;
	return Point((wa * A.x + wb * B.x + wc * C.x) / sum,
				 (wa * A.y + wb * B.y + wc * C.y) / sum);
}

inline Point isogonal_conjugate(const Point &P, const Point &A, const Point &B, const Point &C) {
	double a = dist(B, C), b = dist(A, C), c = dist(A, B);
	double a2 = a * a, b2 = b * b, c2 = c * c;
	std::vector<double> bc = barycentric(P, A, B, C);
	double wa = a2 / bc[0], wb = b2 / bc[1], wc = c2 / bc[2];
	double sum = wa + wb + wc;
	return Point((wa * A.x + wb * B.x + wc * C.x) / sum,
				 (wa * A.y + wb * B.y + wc * C.y) / sum);
}

};

#endif
