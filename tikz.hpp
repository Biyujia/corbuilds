#ifndef tikz
#define tikz

#include "corbuilds.hpp"
#include "circle.hpp"
#include "extra.hpp"
#include <fstream>
#include <vector>
#include <cstdlib>

class tikz_picture {
private:
	std::string file_name = "document.tex";
	const std::string default_file_name = "document.tex";
	FILE* fp;
	std::vector<Point> _label_anchors;   // anchor positions of placed labels
	std::vector<Point> _obstacle_pts;    // points labels should avoid

public:
	double node_size = 0.4;
	double stress_size = 0.05;
	bool axis_draw_O = 1;
	double grid_step = 0.5;
	double default_angle_radius = 0.35;
	double default_right_angle_sz = 0.25;
	double default_opacity = 0.3;
	std::string default_fill_color = "gray";
	double global_scale = 1.0;
	double label_min_sep_dist = 3.0;

	// ── Smart label placement ──────────────────────────────────
	// Set before open_file() if you want labels pushed outward
	// from a specific centre (defaults to (0,0)).
	double picture_center_x = 0.0;
	double picture_center_y = 0.0;

	// Call once per picture to clear the internal label / obstacle
	// registries before re‑using the same tikz_picture object.
	inline void reset_labels() {
		_label_anchors.clear();
		_obstacle_pts.clear();
	}

	// Register a point that labels should avoid (e.g. geometry
	// vertices, circle centres, formula positions).
	inline void register_obstacle(const Point &P) {
		_obstacle_pts.push_back(P);
	}
	inline void register_obstacle(const std::vector<Point> &vP) {
		for (auto &p : vP) register_obstacle(p);
	}

	// Return the best 1-2 char compass direction for a label at P.
	// Tries outer directions first (relative to picture_center),
	// avoids previously placed labels and registered obstacles.
	std::string smart_dir(const Point &P) {
		// Build priority list based on quadrant
		std::vector<std::string> dirs;
		double dx = P.x - picture_center_x;
		double dy = P.y - picture_center_y;
		if (dy >= 0 && dx >= 0)       dirs = {"NE","N","E","NW","SE","SW","S","W"};
		else if (dy >= 0 && dx < 0)   dirs = {"NW","N","W","NE","SW","SE","S","E"};
		else if (dy < 0 && dx >= 0)   dirs = {"SE","S","E","SW","NE","NW","N","W"};
		else                          dirs = {"SW","S","W","SE","NW","NE","N","E"};

		double min_sep = node_size * label_min_sep_dist;  // minimum anchor separation
		for (const std::string &d : dirs) {
			Point anchor = Nod(P, d, node_size);
			bool ok = true;
			for (auto &a : _label_anchors)
				if (dist(anchor, a) < min_sep) { ok = false; break; }
			if (!ok) continue;
			for (auto &o : _obstacle_pts)
				if (dist(anchor, o) < node_size * 1.8) { ok = false; break; }
			if (ok) {
				_label_anchors.push_back(anchor);
				return d;
			}
		}
		// fallback – use the highest-priority direction anyway
		_label_anchors.push_back(Nod(P, dirs[0], node_size));
		return dirs[0];
	}

	// Convenience: call smart_dir for each point in parallel,
	// returns std::vector of direction std::strings.
	std::vector<std::string> smart_dirs(const std::vector<Point> &vP) {
		std::vector<std::string> dirs(vP.size());
		for (size_t i = 0; i < vP.size(); i++)
			dirs[i] = smart_dir(vP[i]);
		return dirs;
	}

	// ── File management ────────────────────────────────────────

	inline void set_file_name(std::string s) { file_name = s; }

	inline void open_file() {
		fp = fopen(file_name.c_str(), "w+");
		fprintf(fp, "\\documentclass{article}\n");
		fprintf(fp, "\\usepackage[margin=1cm]{geometry}\n");
		fprintf(fp, "\\usepackage{tikz}\n");
		fprintf(fp, "\\usepackage{amsmath, amssymb}\n");
		fprintf(fp, "\\usetikzlibrary{patterns}\n");
		fprintf(fp, "\\usetikzlibrary{arrows.meta}\n");
		fprintf(fp, "\\usetikzlibrary{calc}\n");
		fprintf(fp, "\\usetikzlibrary{intersections}\n");
		fprintf(fp, "\\usetikzlibrary{angles}\n");
		fprintf(fp, "\\usetikzlibrary{quotes}\n");
		fprintf(fp, "\\begin{document}\n");
		fprintf(fp, "\\begin{tikzpicture}\n");
		fprintf(fp, "\\pgfdeclarelayer{background}\n");
		fprintf(fp, "\\pgfsetlayers{background,main}\n");
		if (!GLOBAL::cmp(global_scale, 1.))
			fprintf(fp, "\\begin{scope}[scale=%lf]\n", global_scale);
	}

	inline void close() {
		if (!GLOBAL::cmp(global_scale, 1.))
			fprintf(fp, "\\end{scope}\n");
		fprintf(fp, "\\end{tikzpicture}\n");
		fprintf(fp, "\\end{document}\n");
		fclose(fp);
	}

	inline void compile() {
		std::string command = "pdflatex.exe -synctex=1 -interaction=nonstopmode " + file_name;
		system(command.c_str());
	}

	inline void run() { close(); compile(); }

	inline void comment(const std::string &text) {
		fprintf(fp, "%% %s\n", text.c_str());
	}

	// ── Coordinate system ──────────────────────────────────────

	inline void coord(const Point &P, const std::string &name) {
		fprintf(fp, "\\coordinate (%s) at (%lf, %lf);\n", name.c_str(), P.x, P.y);
	}

	inline void set_axis(double xl, double xr, double yd, double yu, std::string dir = "SW") {
		fprintf(fp, "\\draw [->] (0, %lf) -- (0, %lf);\n", yd, yu);
		fprintf(fp, "\\draw [->] (%lf, 0) -- (%lf, 0);\n", xl, xr);
		fprintf(fp, "\\node at (%lf, 0) {$x$};\n", xr + node_size);
		fprintf(fp, "\\node at (0, %lf) {$y$};\n", yu + node_size);
		
		if (axis_draw_O) {
			Point O_pos = Nod(Point(0, 0), dir, node_size);
			fprintf(fp, "\\node at (%lf, %lf) {$O$};\n", O_pos.x, O_pos.y);
		}
		
	}

	inline void set_grid(double xl, double xr, double yd, double yu) {
		fprintf(fp, "\\draw [step=%lf, gray, very thin] (%lf, %lf) grid (%lf, %lf);\n",
			grid_step, xl, yd, xr, yu);
	}

	inline void set_axis_grid(double xl, double xr, double yd, double yu, std::string dir = "SW", double edge_d = 0.1) {
		set_axis(xl, xr, yd, yu, dir);
		set_grid(xl + edge_d, xr - edge_d, yd + edge_d, yu - edge_d);
	}

	// ── Scopes & clipping ──────────────────────────────────────

	inline void begin_scope(const std::string &opts = "") {
		if (opts.empty()) fprintf(fp, "\\begin{scope}\n");
		else fprintf(fp, "\\begin{scope}[%s]\n", opts.c_str());
	}

	inline void end_scope() {
		fprintf(fp, "\\end{scope}\n");
	}

	inline void clip_polygon(const std::vector<Point> &pts) {
		fprintf(fp, "\\clip (%lf, %lf) ", pts[0].x, pts[0].y);
		for (size_t i = 1; i < pts.size(); i++)
			fprintf(fp, "-- (%lf, %lf) ", pts[i].x, pts[i].y);
		fprintf(fp, "-- cycle;\n");
	}

	inline void clip_circle(const Circle &C) {
		fprintf(fp, "\\clip (%lf, %lf) circle (%lf);\n", C.c.x, C.c.y, C.r);
	}

	inline void clip_rectangle(const Point &A, const Point &B) {
		fprintf(fp, "\\clip (%lf, %lf) rectangle (%lf, %lf);\n", A.x, A.y, B.x, B.y);
	}

	// ── Segments ───────────────────────────────────────────────

	inline void draw_segment(const Point &A, const Point &B, const std::string &opts = "") {
		if (opts.empty())
			fprintf(fp, "\\draw (%lf, %lf) -- (%lf, %lf);\n", A.x, A.y, B.x, B.y);
		else
			fprintf(fp, "\\draw [%s] (%lf, %lf) -- (%lf, %lf);\n", opts.c_str(), A.x, A.y, B.x, B.y);
	}

	inline void draw_segment(const std::vector<Point> &pts, const std::string &opts = "") {
		for (size_t i = 1; i < pts.size(); i++)
			draw_segment(pts[i - 1], pts[i], opts);
	}

	// ── Polygons & polylines ───────────────────────────────────

	inline void draw_polygon(const std::vector<Point> &v, bool cycle = true, const std::string &opts = "") {
		if (opts.empty())
			fprintf(fp, "\\draw (%lf, %lf) ", v[0].x, v[0].y);
		else
			fprintf(fp, "\\draw [%s] (%lf, %lf) ", opts.c_str(), v[0].x, v[0].y);
		for (size_t i = 1; i < v.size(); i++)
			fprintf(fp, "-- (%lf, %lf) ", v[i].x, v[i].y);
		if (cycle && v.size() > 2) fprintf(fp, "-- cycle");
		fprintf(fp, ";\n");
	}

	inline void draw_polygon(const std::vector<std::vector<Point> > &v, bool cycle = true, const std::string &opts = "") {
		for (size_t i = 0; i < v.size(); i++)
			draw_polygon(v[i], cycle, opts);
	}

	inline void match_point(const std::vector<Point> &v, bool cycle = true) {
		draw_polygon(v, cycle);
	}

	inline void match_point(const std::vector<std::vector<Point> > &v, bool cycle = true) {
		draw_polygon(v, cycle);
	}

	inline void fill_polygon(const std::vector<Point> &v, const std::string &color = "gray",
							 const std::string &opacity = "") {
		fprintf(fp, "\\begin{pgfonlayer}{background}\n");
		std::string opts = "fill=" + color;
		if (!opacity.empty()) opts += ", fill opacity=" + opacity;
		opts += ", draw=none";
		draw_polygon(v, true, opts);
		fprintf(fp, "\\end{pgfonlayer}\n");
	}

	inline void filldraw_polygon(const std::vector<Point> &v, const std::string &fill_color,
								  const std::string &draw_color = "black",
								  double opacity = 0.3) {
		fprintf(fp, "\\begin{pgfonlayer}{background}\n");
		char buf1[256];
		sprintf(buf1, "fill=%s, fill opacity=%.2f, draw=none", fill_color.c_str(), opacity);
		draw_polygon(v, true, std::string(buf1));
		fprintf(fp, "\\end{pgfonlayer}\n");
		char buf2[256];
		sprintf(buf2, "draw=%s", draw_color.c_str());
		draw_polygon(v, true, std::string(buf2));
	}

	// ── Circles ────────────────────────────────────────────────

	inline void draw_circle(const Circle &C, const std::string &opts = "") {
		if (opts.empty())
			fprintf(fp, "\\draw (%lf, %lf) circle (%lf);\n", C.c.x, C.c.y, C.r);
		else
			fprintf(fp, "\\draw [%s] (%lf, %lf) circle (%lf);\n", opts.c_str(), C.c.x, C.c.y, C.r);
	}

	inline void fill_circle(const Circle &C, const std::string &color = "gray",
							double opacity = 0.3) {
		fprintf(fp, "\\begin{pgfonlayer}{background}\n");
		fprintf(fp, "\\fill [%s, fill opacity=%.2f] (%lf, %lf) circle (%lf);\n",
			color.c_str(), opacity, C.c.x, C.c.y, C.r);
		fprintf(fp, "\\end{pgfonlayer}\n");
	}

	inline void filldraw_circle(const Circle &C, const std::string &fill_color = "gray",
								const std::string &draw_color = "black", double opacity = 0.3) {
		fprintf(fp, "\\begin{pgfonlayer}{background}\n");
		fprintf(fp, "\\fill [%s, fill opacity=%.2f] (%lf, %lf) circle (%lf);\n",
			fill_color.c_str(), opacity, C.c.x, C.c.y, C.r);
		fprintf(fp, "\\end{pgfonlayer}\n");
		fprintf(fp, "\\draw [draw=%s] (%lf, %lf) circle (%lf);\n",
			draw_color.c_str(), C.c.x, C.c.y, C.r);
	}

	// ── Arcs ───────────────────────────────────────────────────

	inline void draw_arc(const Point &center, double radius,
						  double start_deg, double end_deg, const std::string &opts = "") {
		Point A = point_rotate(Point(center.x + radius, center.y), center, start_deg * PI / 180.);
		if (opts.empty())
			fprintf(fp, "\\draw (%lf, %lf) arc (%lf:%lf:%lf);\n",
				A.x, A.y, start_deg, end_deg, radius);
		else
			fprintf(fp, "\\draw [%s] (%lf, %lf) arc (%lf:%lf:%lf);\n",
				opts.c_str(), A.x, A.y, start_deg, end_deg, radius);
	}

	inline void draw_arc_rad(const Point &center, double radius,
							  double start_rad, double end_rad, const std::string &opts = "") {
		draw_arc(center, radius, start_rad * 180. / PI, end_rad * 180. / PI, opts);
	}

	inline void draw_angle_arc(const Point &A, const Point &B, const Point &C,
								double radius = -1, const std::string &opts = "") {
		if (radius < 0) radius = default_angle_radius;
		double a1 = atan2(A.y - B.y, A.x - B.x);
		double a2 = atan2(C.y - B.y, C.x - B.x);
		if (a1 < 0) a1 += 2 * PI;
		if (a2 < 0) a2 += 2 * PI;
		double da = a2 - a1;
		if (fabs(da) > PI) da = da > 0 ? da - 2. * PI : da + 2. * PI;
		double start_deg = a1 * 180. / PI;
		double end_deg = (a1 + da) * 180. / PI;
		draw_arc(B, radius, start_deg, end_deg, opts);
	}

	inline void draw_right_angle(const Point &A, const Point &B, const Point &C,
								  double sz = -1) {
		if (sz < 0) sz = default_right_angle_sz;
		Point v1 = A - B, v2 = C - B;
		double d1 = norm(v1), d2 = norm(v2);
		if (d1 < GLOBAL::eps || d2 < GLOBAL::eps) return;
		Point u1 = v1 * (sz / d1);
		Point u2 = v2 * (sz / d2);
		Point p1 = B + u1;
		Point p2 = B + u1 + u2;
		Point p3 = B + u2;
		fprintf(fp, "\\draw (%lf, %lf) -- (%lf, %lf) -- (%lf, %lf);\n",
			p1.x, p1.y, p2.x, p2.y, p3.x, p3.y);
	}

	// ── Ellipse & Rectangle ────────────────────────────────────

	inline void draw_ellipse(const Point &center, double rx, double ry,
							 const std::string &opts = "") {
		if (opts.empty())
			fprintf(fp, "\\draw (%lf, %lf) ellipse (%lf and %lf);\n", center.x, center.y, rx, ry);
		else
			fprintf(fp, "\\draw [%s] (%lf, %lf) ellipse (%lf and %lf);\n",
				opts.c_str(), center.x, center.y, rx, ry);
	}

	inline void fill_ellipse(const Point &center, double rx, double ry,
							 const std::string &color = "gray", double opacity = 0.3) {
		fprintf(fp, "\\begin{pgfonlayer}{background}\n");
		fprintf(fp, "\\fill [%s, fill opacity=%.2f] (%lf, %lf) ellipse (%lf and %lf);\n",
			color.c_str(), opacity, center.x, center.y, rx, ry);
		fprintf(fp, "\\end{pgfonlayer}\n");
	}

	inline void draw_rectangle(const Point &A, const Point &B, const std::string &opts = "") {
		if (opts.empty())
			fprintf(fp, "\\draw (%lf, %lf) rectangle (%lf, %lf);\n", A.x, A.y, B.x, B.y);
		else
			fprintf(fp, "\\draw [%s] (%lf, %lf) rectangle (%lf, %lf);\n",
				opts.c_str(), A.x, A.y, B.x, B.y);
	}

	inline void fill_rectangle(const Point &A, const Point &B, const std::string &color = "gray",
								double opacity = 0.3) {
		fprintf(fp, "\\begin{pgfonlayer}{background}\n");
		fprintf(fp, "\\fill [%s, fill opacity=%.2f] (%lf, %lf) rectangle (%lf, %lf);\n",
			color.c_str(), opacity, A.x, A.y, B.x, B.y);
		fprintf(fp, "\\end{pgfonlayer}\n");
	}

	inline void filldraw_rectangle(const Point &A, const Point &B, const std::string &fill_color,
									const std::string &draw_color = "black", double opacity = 0.3) {
		fprintf(fp, "\\begin{pgfonlayer}{background}\n");
		fprintf(fp, "\\fill [%s, fill opacity=%.2f] (%lf, %lf) rectangle (%lf, %lf);\n",
			fill_color.c_str(), opacity, A.x, A.y, B.x, B.y);
		fprintf(fp, "\\end{pgfonlayer}\n");
		fprintf(fp, "\\draw [draw=%s] (%lf, %lf) rectangle (%lf, %lf);\n",
			draw_color.c_str(), A.x, A.y, B.x, B.y);
	}

	// ── Arrows ─────────────────────────────────────────────────

	inline void draw_arrow(const Point &A, const Point &B, const std::string &opts = "") {
		std::string o = opts.empty() ? "->" : "->, " + opts;
		fprintf(fp, "\\draw [%s] (%lf, %lf) -- (%lf, %lf);\n",
			o.c_str(), A.x, A.y, B.x, B.y);
	}

	inline void draw_double_arrow(const Point &A, const Point &B, const std::string &opts = "") {
		std::string o = opts.empty() ? "<->" : "<->, " + opts;
		fprintf(fp, "\\draw [%s] (%lf, %lf) -- (%lf, %lf);\n",
			o.c_str(), A.x, A.y, B.x, B.y);
	}

	inline void draw_vector(const Point &A, const Point &B, const std::string &opts = "") {
		draw_arrow(A, B, opts);
	}

	// ── Points & dots ──────────────────────────────────────────

	inline void draw_point(const Point &P, const std::string &opts = "") {
		if (opts.empty())
			fprintf(fp, "\\fill (%lf, %lf) circle (%lf);\n", P.x, P.y, stress_size);
		else
			fprintf(fp, "\\fill [%s] (%lf, %lf) circle (%lf);\n",
				opts.c_str(), P.x, P.y, stress_size);
	}

	inline void draw_point(const std::vector<Point> &vP, const std::string &opts = "") {
		for (size_t i = 0; i < vP.size(); i++) draw_point(vP[i], opts);
	}

	inline void stress(const Point &P) { draw_point(P); }
	inline void stress(const std::vector<Point> &vP) { draw_point(vP); }

	inline void draw_point_label(const Point &P, const std::string &label_text,
								  const std::string &dir = "NE", const std::string &opts = "") {
		draw_point(P, opts);
		label_math(P, label_text, dir);
	}

	inline void draw_point_labels(const std::vector<Point> &vP,
								   const std::vector<std::string> &vLabels,
								   const std::vector<std::string> &vDirs,
								   const std::string &opts = "") {
		for (size_t i = 0; i < vP.size(); i++)
			draw_point_label(vP[i], vLabels[i], vDirs[i], opts);
	}
	// Convenience: auto‑choose directions via smart_dir()
	inline void smart_point_label(const Point &P, const std::string &label_text,
								   const std::string &opts = "") {
		draw_point_label(P, label_text, smart_dir(P), opts);
	}

	inline void smart_point_labels(const std::vector<Point> &vP,
								    const std::vector<std::string> &vLabels,
								    const std::string &opts = "") {
		for (size_t i = 0; i < vP.size(); i++)
			draw_point_label(vP[i], vLabels[i], smart_dir(vP[i]), opts);
	}

	// ── Labels & text ──────────────────────────────────────────

	inline void label(const Point &P, const std::string &text, const std::string &dir = "",
					  const std::string &opts = "") {
		if (dir.empty()) {
			if (opts.empty())
				fprintf(fp, "\\node at (%lf, %lf) {%s};\n", P.x, P.y, text.c_str());
			else
				fprintf(fp, "\\node [%s] at (%lf, %lf) {%s};\n",
					opts.c_str(), P.x, P.y, text.c_str());
		} else {
			Point Q = Nod(P, dir, node_size);
			_label_anchors.push_back(Q);  // track for smart_dir
			if (opts.empty())
				fprintf(fp, "\\node at (%lf, %lf) {%s};\n", Q.x, Q.y, text.c_str());
			else
				fprintf(fp, "\\node [%s] at (%lf, %lf) {%s};\n",
					opts.c_str(), Q.x, Q.y, text.c_str());
		}
	}

	inline void node(const Point &P, const std::string &dir, const std::string &Ps) {
		label(P, "$" + Ps + "$", dir);
	}

	inline void node(const std::vector<Point> &vP, const std::vector<std::string> &vPd,
					 const std::vector<std::string> &vPs) {
		for (size_t i = 0; i < vP.size(); i++)
			node(vP[i], vPd[i], vPs[i]);
	}

	inline void label_math(const Point &P, const std::string &formula, const std::string &dir = "",
						   const std::string &opts = "") {
		label(P, "$" + formula + "$", dir, opts);
	}

	inline void label_math(const std::vector<Point> &vP, const std::vector<std::string> &vFormulas,
						   const std::vector<std::string> &vDirs, const std::string &opts = "") {
		for (size_t i = 0; i < vP.size(); i++)
			label_math(vP[i], vFormulas[i], vDirs[i], opts);
	}

	inline void formula(const Point &P, const std::string &text, double scl = 1.0,
						const std::string &opts = "") {
		char buf[512];
		sprintf(buf, "scale=%.2f", scl);
		std::string o = std::string(buf) + (opts.empty() ? "" : ", " + opts);
		fprintf(fp, "\\node [%s] at (%lf, %lf) {$%s$};\n",
			o.c_str(), P.x, P.y, text.c_str());
		_obstacle_pts.push_back(P);  // avoid collision with formulas
	}

	// ── Right angle and marks ──────────────────────────────────

	inline void parallel_mark(const Point &A, const Point &B, int count = 1,
							   double offset = 0.18) {
		Point mid = line_division(A, B);
		Point dir = B - A;
		double len = norm(dir);
		Point perp(-dir.y / len, dir.x / len);
		for (int k = 0; k < count; k++) {
			double sgn = (k % 2 == 0) ? 1. : -1.;
			Point p1 = mid + perp * (offset * sgn);
			Point p2 = mid + perp * (offset * sgn * 1.8);
			fprintf(fp, "\\draw (%lf, %lf) -- (%lf, %lf);\n", p1.x, p1.y, p2.x, p2.y);
		}
	}

	inline void equal_mark(const Point &A, const Point &B, int count = 1,
							double offset = 0.18) {
		parallel_mark(A, B, count, offset);
	}

	// ── Bezier curves ──────────────────────────────────────────

	inline void draw_bezier(const Point &P0, const Point &P1, const Point &P2,
							const Point &P3, const std::string &opts = "") {
		if (opts.empty())
			fprintf(fp, "\\draw (%lf, %lf) .. controls (%lf, %lf) and (%lf, %lf) .. (%lf, %lf);\n",
				P0.x, P0.y, P1.x, P1.y, P2.x, P2.y, P3.x, P3.y);
		else
			fprintf(fp, "\\draw [%s] (%lf, %lf) .. controls (%lf, %lf) and (%lf, %lf) .. (%lf, %lf);\n",
				opts.c_str(), P0.x, P0.y, P1.x, P1.y, P2.x, P2.y, P3.x, P3.y);
	}

	// ── Function plots ─────────────────────────────────────────

	inline void draw_func(double (*f)(double), const double &a, const double &b,
						  const int &n, const std::string &opts = "") {
		std::vector<bezier3> plt = bezier_decomposition(f, a, b, n);
		if (opts.empty())
			fprintf(fp, "\\draw (%lf, %lf) .. controls (%lf, %lf) and (%lf, %lf) .. (%lf, %lf)",
				plt[0][0].x, plt[0][0].y, plt[0][1].x, plt[0][1].y,
				plt[0][2].x, plt[0][2].y, plt[0][3].x, plt[0][3].y);
		else
			fprintf(fp, "\\draw [%s] (%lf, %lf) .. controls (%lf, %lf) and (%lf, %lf) .. (%lf, %lf)",
				opts.c_str(), plt[0][0].x, plt[0][0].y, plt[0][1].x, plt[0][1].y,
				plt[0][2].x, plt[0][2].y, plt[0][3].x, plt[0][3].y);
		for (int i = 1; i < n - 1; i++)
			fprintf(fp, "\n    .. controls (%lf, %lf) and (%lf, %lf) .. (%lf, %lf)",
				plt[i][1].x, plt[i][1].y, plt[i][2].x, plt[i][2].y,
				plt[i][3].x, plt[i][3].y);
		fprintf(fp, ";\n\n");
	}

	// ── Parametric curves ──────────────────────────────────────

	inline void draw_curve(const std::vector<Point> &vP, bool tension = true,
						   const std::string &opts = "") {
		if (vP.empty()) return;
		if (tension) {
			if (opts.empty())
				fprintf(fp, "\\draw plot [smooth] coordinates {");
			else
				fprintf(fp, "\\draw [%s] plot [smooth] coordinates {", opts.c_str());
			for (size_t i = 0; i < vP.size(); i++)
				fprintf(fp, "(%lf, %lf) ", vP[i].x, vP[i].y);
			fprintf(fp, "};\n");
		} else {
			fprintf(fp, "\\draw (%lf, %lf)", vP[0].x, vP[0].y);
			for (size_t i = 1; i < vP.size(); i++)
				fprintf(fp, " -- (%lf, %lf)", vP[i].x, vP[i].y);
			fprintf(fp, ";\n");
		}
	}

	// ── Dashed / dotted / styled shortcuts ─────────────────────

	inline void draw_dashed(const Point &A, const Point &B) {
		draw_segment(A, B, "dashed");
	}

	inline void draw_dotted(const Point &A, const Point &B) {
		draw_segment(A, B, "dotted");
	}

	inline void draw_thick(const Point &A, const Point &B) {
		draw_segment(A, B, "thick");
	}

	inline void draw_dashed_polygon(const std::vector<Point> &v, bool cycle = true) {
		draw_polygon(v, cycle, "dashed");
	}

	inline void draw_dotted_polygon(const std::vector<Point> &v, bool cycle = true) {
		draw_polygon(v, cycle, "dotted");
	}

	inline void draw_dashed_circle(const Circle &C) {
		draw_circle(C, "dashed");
	}

	inline void draw_dotted_circle(const Circle &C) {
		draw_circle(C, "dotted");
	}

	// ── Line shortcuts ─────────────────────────────────────────

	inline void draw_line(const Line &l, double l_l, double l_r,
						  bool l_l_d = false, bool l_r_d = false, const std::string &opts = "") {
		draw_segment(l_l_d ? PX(l, l_l) : PY(l, l_l),
					 l_r_d ? PX(l, l_r) : PY(l, l_r), opts);
	}

	inline void draw_line_frame(const Line &l, double xl, double xr,
								double yd, double yu, bool l_l_d = false, bool l_r_d = false,
								double pad = 0.5) {
		double x1 = l_l_d ? xl : xl - pad;
		double x2 = l_r_d ? xr : xr + pad;
		double y1 = l_l_d ? yd - pad : yd;
		double y2 = l_r_d ? yu + pad : yu;
		Point A = l_l_d ? PX(l, x1) : PY(l, y1);
		Point B = l_r_d ? PX(l, x2) : PY(l, y2);
		draw_segment(A, B);
	}

	// ── Hatching & patterns ────────────────────────────────────

	inline void hatch_polygon(const std::vector<Point> &v, const std::string &pattern = "north east lines",
							   const std::string &color = "black") {
		fprintf(fp, "\\begin{pgfonlayer}{background}\n");
		fprintf(fp, "\\fill [pattern=%s, pattern color=%s, draw=none] ", pattern.c_str(), color.c_str());
		fprintf(fp, "(%lf, %lf) ", v[0].x, v[0].y);
		for (size_t i = 1; i < v.size(); i++)
			fprintf(fp, "-- (%lf, %lf) ", v[i].x, v[i].y);
		fprintf(fp, "-- cycle;\n");
		fprintf(fp, "\\end{pgfonlayer}\n");
	}

	inline void hatch_circle(const Circle &C, const std::string &pattern = "north east lines",
							  const std::string &color = "black") {
		fprintf(fp, "\\begin{pgfonlayer}{background}\n");
		fprintf(fp, "\\fill [pattern=%s, pattern color=%s, draw=none] (%lf, %lf) circle (%lf);\n",
			pattern.c_str(), color.c_str(), C.c.x, C.c.y, C.r);
		fprintf(fp, "\\end{pgfonlayer}\n");
	}

	// ── Shading ────────────────────────────────────────────────

	inline void shade_polygon(const std::vector<Point> &v, const std::string &color1 = "gray",
							   const std::string &color2 = "white", double opacity = 0.3) {
		fprintf(fp, "\\begin{pgfonlayer}{background}\n");
		fprintf(fp, "\\shade [top color=%s, bottom color=%s, fill opacity=%.2f, draw=none] ",
			color1.c_str(), color2.c_str(), opacity);
		fprintf(fp, "(%lf, %lf) ", v[0].x, v[0].y);
		for (size_t i = 1; i < v.size(); i++)
			fprintf(fp, "-- (%lf, %lf) ", v[i].x, v[i].y);
		fprintf(fp, "-- cycle;\n");
		fprintf(fp, "\\end{pgfonlayer}\n");
	}

	inline void shade_circle(const Circle &C, const std::string &color1 = "gray",
							  const std::string &color2 = "white", double opacity = 0.3) {
		fprintf(fp, "\\begin{pgfonlayer}{background}\n");
		fprintf(fp, "\\shade [inner color=%s, outer color=%s, fill opacity=%.2f, draw=none] (%lf, %lf) circle (%lf);\n",
			color1.c_str(), color2.c_str(), opacity, C.c.x, C.c.y, C.r);
		fprintf(fp, "\\end{pgfonlayer}\n");
	}
};

#endif
