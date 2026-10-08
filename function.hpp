#ifndef function
#define function

#include <cmath>
#include <limits>
#include <stdexcept>
#include <utility>
#include <vector>

#include "point.hpp"
#include "line.hpp"
#include "basic_geometry.hpp"

using bezier3 = std::array<Point, 4>;

namespace fn {
    inline bool finite(const double &x) {
        return fn::finite(x);
    }  

    inline double nanv() { 
        return std::numeric_limits<double>::quiet_NaN(); 
    }
};

template <typename F>
inline std::vector<bezier3> bezier_decomposition(F f, const double &a, const double &b, const int &n) {
    if (n < 2) throw std::invalid_argument("n must be at least 2");
    const double h = (b - a) / (1.0 * (n - 1));
    std::vector<double> f_sample(n);
    for (int i = 0; i < n; i++)
        f_sample[i] = f(a + i * h);
    std::vector<double> f_slope(n);
    f_slope[0] = (f_sample[1] - f_sample[0]) / h;
    f_slope[n - 1] = (f_sample[n - 1] - f_sample[n - 2]) / h;
    for (int i = 1; i < n - 1; i++) 
        f_slope[i] = (f_sample[i + 1] - f_sample[i - 1]) / (2.0 * h);
    std::vector<bezier3> ret;
    ret.reserve(n - 1);
    for (int i = 0; i < n - 1; i++) {
        ret.push_back({
            Point(a + i * h, f_sample[i]),
            Point(a + (1. * i + 1. / 3.) * h, f_sample[i] + f_slope[i] * h / 3),
            Point(a + (1. * i + 2. / 3.) * h, f_sample[i + 1] - f_slope[i + 1] * h / 3.),
            Point(a + (i + 1) * h, f_sample[i + 1])
        });
    }
    return ret;
}

template <typename F>
inline double derivative(F f, double x, double h = 1e-6) {
    if (h == 0.0) return fn::nanv();
    const double y1 = f(x + h), y2 = f(x - h);
    if (!fn::finite(y1) || !fn::finite(y2)) return fn::nanv();
    return (y1 - y2) / (2.0 * h);
}

template <typename F>
inline double derivative5(F f, double x, double h = 1e-4) {
    if (h == 0.0) return fn::nanv();
    const double a = f(x + 2.0 * h), b = f(x + h);
    const double c = f(x - h), d = f(x - 2.0 * h);
    if (!fn::finite(a) || !fn::finite(b) || !fn::finite(c) || !fn::finite(d))
        return fn::nanv();
    return (-a + 8.0 * b - 8.0 * c + d) / (12.0 * h);
}

// integrating a function using Simpson's method
template <typename F>
inline double integral_simpson(F f, double a, double b, int n = 1000) {
    if (n < 2) throw std::invalid_argument("integral_simpson: n >= 2");
    if (a == b) return 0.0;
    if (a > b) return -integral_simpson(f, b, a, n);
    if (n % 2 == 1) ++n;
    const double h = (b - a) / n;
    double s = f(a) + f(b);
    for (int i = 1; i < n; ++i) {
        const double v = f(a + i * h);
        if (!fn::finite(v)) return fn::nanv();
        s += (i % 2 == 0) ? 2.0 * v : 4.0 * v;
    }
    return s * h / 3.0;
}

// integrating a function using Riemann integral's definition
template <typename F>
inline double integral_riemann(F f, double a, double b, int n = 1000, int rule = 0) {
    if (n < 1) throw std::invalid_argument("integral_riemann: n >= 1");
    if (a == b) return 0.0;
    if (a > b) return -integral_riemann(f, b, a, n, rule);
    const double h = (b - a) / n;
    double s = 0.0;
    for (int i = 0; i < n; ++i) {
        double x;
        if (rule < 0) x = a + i * h; // left
        else if (rule > 0) x = a + (i + 1) * h;  // right
        else x = a + (i + 0.5) * h; // midpoint
        const double v = f(x);
        if (!fn::finite(v)) return fn::nanv();
        s += v;
    }
    return s * h;
}

template <typename F>
inline Line tangent_line(F f, double x0) {
    const double y0 = f(x0);
    const double m = derivative(f, x0);
    if (!fn::finite(y0) || !fn::finite(m))
        return Line();
    return Line(m, -1.0, y0 - m * x0);
}

template <typename F>
inline Line normal_line(F f, double x0) {
    const double y0 = f(x0);
    const double m = derivative(f, x0);
    if (!fn::finite(y0) || !fn::finite(m))
        return Line();
    return Line(1.0, m, -(x0 + m * y0));
}

// find roots by using the bisection method on small intervals
template <typename F>
inline std::vector<double> find_roots(F f, double a, double b, int n, double tol = 1e-10) {
    std::vector<double> roots;
    if (n < 2) throw std::invalid_argument("find_roots: n >= 2 required");
    if (a > b) std::swap(a, b);
    if (a == b) return roots;
    if (tol <= 0.0) tol = 1e-12;

    // add a root unless one already exists within a distance
    auto add_root = [&](double x) {
        if (!fn::finite(x)) return;
        const double dist = std::max(tol, 1e-12);
        if (!roots.empty() && std::fabs(x - roots.back()) <= dist) return;
        roots.push_back(x);
    };

    const double h = (b - a) / (n - 1);
    double x0 = a, f0 = f(x0);
    for (int i = 1; i <= n; ++i) {
        const double x1 = (i == n) ? b : a + i * h;
        const double f1 = f(x1);
        if (fn::finite(f0) && fn::finite(f1)) {
            if (std::fabs(f0) <= tol) add_root(x0);
            if (f0 * f1 < 0.0) {
                double lo = x0, hi = x1, flo = f0, fhi = f1;
                for (int it = 0; it < 200; ++it) {
                    const double mid = 0.5 * (lo + hi);
                    const double fm = f(mid);
                    if (!fn::finite(fm)) break;
                    if (std::fabs(fm) <= tol || (hi - lo) <= 2.0 * tol * (1.0 + std::fabs(mid))) {
                        add_root(mid);
                        break;
                    }
                    if (flo * fm < 0.0) { hi = mid; fhi = fm; }
                    else { lo = mid; flo = fm; }
                }
            }
        }
        x0 = x1;
        f0 = f1;
    }
    if (fn::finite(f0) && std::fabs(f0) <= tol) add_root(b);
    return roots;
}



#endif
