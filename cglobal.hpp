#ifndef cglobal
#define cglobal

#include <string>
#include <iostream>
#include <algorithm>
#include <cmath>
#include <cstdio>
#include <vector>
#include <map>
#include <array>

using ll = long long;
const double PI = std::acos(-1.);

namespace GLOBAL {
double eps = 1e-8;
const double DelPx[8] = {1., 1., 1., 0., -1., -1., -1., 0.};
const double DelPy[8] = {1., 0., -1., -1., -1., 0., 1., 1.};

inline int mpdir(const std::string &dir) {
	return dir == "NE" ? 0 :
			dir == "E" ? 1 :
			dir == "SE" ? 2 :
			dir == "S" ? 3 :
			dir == "SW" ? 4 :
			dir == "W" ? 5 :
			dir == "NW" ? 6 :
			dir == "N" ? 7 : -1;
}

inline bool cmp(const double &x, const double &y) {
	return std::fabs(x - y) < eps;
}

}

#endif