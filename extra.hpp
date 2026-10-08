#ifndef extra
#define extra

#include "cglobal.hpp"
#include "point.hpp"
#include "line.hpp"

// The position function 
inline Point Nod(const Point &P, const std::string &dir, double Delt = 0.4) {
	return Point(P.x + GLOBAL::DelPx[GLOBAL::mpdir(dir)] * Delt, P.y + GLOBAL::DelPy[GLOBAL::mpdir(dir)] * Delt);
}

// print
void print() { printf("\n"); }
void print(const double& x) { printf("[Double] %.6lf\n", x); }
void print(const std::string& s) { printf("[String] %s\n", s.c_str()); }
template <typename T, typename... Ts>
void print(const T& arg, const Ts&... args) {
	print(arg); print(args...);
}


#endif