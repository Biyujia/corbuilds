## tikz.hpp — TikZ Drawing Engine

Built on PDFLaTeX + TikZ, `tikz.hpp` provides the `tikz_picture` class for programmatic geometric drawing in LaTeX. All functions write TikZ commands into a `.tex` file, which is compiled to PDF via `pdflatex`.

### Workflow

```cpp
tikz_picture pic;
pic.set_file_name("output.tex");
pic.global_scale = 0.85;     // set before open_file()
pic.open_file();              // preamble + \begin{tikzpicture} + scale scope
// ... drawing commands ...
pic.run();                    // close() + compile()
```

### Configurable Fields

| Field | Default | Description |
|---|---|---|
| `node_size` | `0.4` | Label offset from points |
| `stress_size` | `0.05` | Dot radius for `draw_point` |
| `axis_draw_O` | `true` | Whether to draw O label on axes |
| `grid_step` | `0.5` | Grid spacing |
| `default_angle_radius` | `0.35` | Arc radius for `draw_angle_arc` |
| `default_right_angle_sz` | `0.25` | Size of right angle symbol |
| `default_opacity` | `0.3` | Default fill opacity |
| `default_fill_color` | `"gray"` | Default fill color |
| `global_scale` | `1.0` | Global scale factor applied to entire picture |
| `label_min_sep_dist` | `3.0` | Minimum anchor separation multiplier for `smart_dir` (× `node_size`) |
| `picture_center_x` | `0.0` | X-coordinate of picture center for label direction priority |
| `picture_center_y` | `0.0` | Y-coordinate of picture center for label direction priority |

### Drawing Function Reference

`opts` parameters accept any valid TikZ key=value pairs, e.g. `"thick, red, dashed"`.

---

#### File & Lifecycle

| Function | Description |
|---|---|
| `set_file_name(s)` | Set output `.tex` filename |
| `open_file()` | Write preamble with all required `\usetikzlibrary` includes |
| `close()` | Close `tikzpicture` and document |
| `compile()` | Run `pdflatex` |
| `run()` | `close()` then `compile()` |
| `comment(text)` | Write a `%` comment line into the TeX file |

---

#### Coordinate System

| Function | Description |
|---|---|
| `coord(P, name)` | Define named TikZ coordinate `(name)` at point P |
| `set_axis(xl, xr, yd, yu, dir)` | Draw x/y axes from xl..xr, yd..yu and draw the original point at direction dir (if DrawO is on). |
| `set_grid(xl, xr, yd, yu)` | Draw grid in given rectangle |
| `set_axis_grid(xl, xr, yd, yu, dir, edge_d)` | Combined axes + grid with padding |

---

#### Segments

| Function | Description |
|---|---|
| `draw_segment(A, B)` | Line segment from A to B |
| `draw_segment(A, B, opts)` | Segment with TikZ options |
| `draw_segment(pts, opts)` | Segments connecting consecutive points |
| `draw_dashed(A, B)` | Dashed segment |
| `draw_dotted(A, B)` | Dotted segment |
| `draw_thick(A, B)` | Thick segment |
| `draw_arrow(A, B, opts)` | Arrow A→B (`->` tip) |
| `draw_double_arrow(A, B, opts)` | Double arrow A↔B (`<->` tip) |
| `draw_vector(A, B, opts)` | Alias for `draw_arrow` |
| `draw_line(l, l_l, l_r, ...)` | Draw line l between x/y bounds |

---

#### Polygons & Polylines

| Function | Description |
|---|---|
| `draw_polygon(pts, cycle, opts)` | Polyline through points; `cycle=true` closes the path |
| `draw_polygon(vpts, cycle, opts)` | Batch multiple polylines |
| `match_point(pts, cycle)` | Legacy alias for `draw_polygon` |
| `fill_polygon(pts, color, opacity)` | Fill polygon with solid color (rendered on background layer) |
| `filldraw_polygon(pts, fill_color, draw_color, opacity)` | Fill (background) + outline (main) polygon |
| `draw_dashed_polygon(pts, cycle)` | Dashed polyline |
| `draw_dotted_polygon(pts, cycle)` | Dotted polyline |

---

#### Circles

| Function | Description |
|---|---|
| `draw_circle(C)` | Draw circle outline |
| `draw_circle(C, opts)` | Circle outline with options |
| `fill_circle(C, color, opacity)` | Filled circle |
| `filldraw_circle(C, fill, draw, opacity)` | Fill + outline circle |
| `draw_dashed_circle(C)` | Dashed circle |
| `draw_dotted_circle(C)` | Dotted circle |

---

#### Arcs & Angle Markings

| Function | Description |
|---|---|
| `draw_arc(center, r, start_deg, end_deg, opts)` | Arc from start to end angle (degrees) |
| `draw_arc_rad(center, r, start_rad, end_rad, opts)` | Arc from start to end angle (radians) |
| `draw_angle_arc(A, B, C, radius, opts)` | Arc marking angle ∠ABC with given radius |
| `draw_right_angle(A, B, C, sz)` | Right angle symbol └ at vertex B (no-op if A=B or B=C) |

---

#### Ellipse & Rectangle

| Function | Description |
|---|---|
| `draw_ellipse(center, rx, ry, opts)` | Ellipse with horizontal/vertical radii |
| `fill_ellipse(center, rx, ry, color, opacity)` | Filled ellipse |
| `draw_rectangle(A, B, opts)` | Rectangle from corners A and B |
| `fill_rectangle(A, B, color, opacity)` | Filled rectangle |
| `filldraw_rectangle(A, B, fill, draw, opacity)` | Fill + outline rectangle |

---

#### Points & Labels

| Function | Description |
|---|---|
| `draw_point(P)` | Filled circle dot at P (size `stress_size`) |
| `draw_point(P, opts)` | Dot with options (e.g. `"red"`) |
| `draw_point(vP, opts)` | Batch dots |
| `stress(P)` / `stress(vP)` | Legacy alias for `draw_point` |
| `draw_point_label(P, label, dir, opts)` | Dot + text label at compass offset |
| `draw_point_labels(vP, vLabels, vDirs, opts)` | Batch dots with labels |
| `label(P, text)` | Text exactly at P |
| `label(P, text, dir, opts)` | Text offset by `node_size` in direction `dir` (`"N"`, `"NE"`, `"S"`, etc.) |
| `label_math(P, formula, dir, opts)` | LaTeX math: `$formula$` |
| `label_math(vP, vFormulas, vDirs, opts)` | Batch math labels |
| `node(P, dir, Ps)` | Legacy: `label(P, "$"+Ps+"$", dir)` |
| `formula(P, text, scale, opts)` | Scaled math formula at point |

---

#### Bezier Curves & Plots

| Function | Description |
|---|---|
| `draw_bezier(P0, P1, P2, P3, opts)` | Cubic Bezier with control points |
| `draw_curve(pts, smooth, opts)` | Curve through points; `smooth=true` uses spline, `false` uses polyline |
| `draw_func(f, a, b, n)` | Function plot using Bezier decomposition (n = sample points) |
| `draw_func(f, a, b, n, opts)` | Function plot with TikZ options |

---

#### Scoping & Clipping

| Function | Description |
|---|---|
| `begin_scope(opts)` | Begin scoped group with optional style |
| `end_scope()` | End scoped group |
| `clip_polygon(pts)` | Clipping region = polygon |
| `clip_circle(C)` | Clipping region = circle |
| `clip_rectangle(A, B)` | Clipping region = rectangle |

---

#### Patterns, Shading & Marks

| Function | Description |
|---|---|
| `hatch_polygon(pts, pattern, color)` | Fill polygon with hatching pattern (`"north east lines"`, `"crosshatch"`, etc.) |
| `hatch_circle(C, pattern, color)` | Fill circle with hatching |
| `shade_polygon(pts, c1, c2, opacity)` | Gradient shade polygon (top→bottom), opacity default `0.3` |
| `shade_circle(C, c1, c2, opacity)` | Radial gradient shade circle (inner→outer), opacity default `0.3` |
| `parallel_mark(A, B, count, offset)` | Draw parallel tick marks on segment AB (1–3 lines) |
| `equal_mark(A, B, count, offset)` | Alias for `parallel_mark` (equal-length notation) |

---

#### Smart Labeling (Experimental, not smart enough up to now:( )

**Description**: The `smart_dir` function tries to automatically assign directions to each point so that the labels look neat and won't cover essential parts of the picture (such as other labels, lines or axes). The function tries to make the label be in the outer side of the canvas, and assign every label a 'hit-box' so that they don't collide. 

**Plans**: Smarter algorithms are on the schedule (such as assigning 'hit-boxes' for every geometry object (basically selecting points on the geometric object at fixed intervals and assigning every point a 'hit-box') and loosen the conditions of assigning directions).

| Function | Description |
|---|---|
| `smart_dir(P)` | Assign a direction automatically for the label of point P |
| `smart_dirs(vP)` | Assign directions for the points in vP in order |
| `smart_point_label(P, label_text, opts)` | Label the point P in the direction assigned by `smart_dir` with label_text | 
| `smart_point_label(vP, vlabel_text, opts)` | Label the points in vP in the direction assigned by `smart_dirs` with vlabel_text | 

---

### TikZ Options Quick Reference

Common option strings passed as the `opts` parameter:

| Option | Effect |
|---|---|
| `thick`, `thin`, `ultra thick` | Line width |
| `red`, `blue`, `gray`, `green!60!black` | Color |
| `dashed`, `dotted`, `densely dashed` | Line pattern |
| `->`, `<-`, `<->`, `stealth-` | Arrow tips |
| `fill=red`, `fill opacity=0.3` | Fill properties |
| `draw=none` | No outline |
| `scale=1.5`, `xshift=1cm` | Transforms |

### Available TikZ Libraries

Automatically loaded by `open_file()`:

| Library | Purpose |
|---|---|
| `patterns` | `hatch_polygon`, `hatch_circle` |
| `arrows.meta` | Custom arrow tips |
| `calc` | Coordinate calculations |
| `intersections` | Path intersection computation |
| `angles` | Angle pic support |
| `quotes` | Quoted labels on pics |

### Available LaTeX Packages

Automatically loaded by `open_file()`:

| Package | Purpose |
|---|---|
| `amsmath, amssymb` | Math formatting; enables `\text{}` inside `$...$` |

### Layer System

`open_file()` declares a `background` layer beneath the `main` layer.
All fill, shade, and hatch operations (`fill_polygon`, `fill_circle`, `shade_polygon`, `hatch_polygon`, `filldraw_polygon`, `filldraw_circle`, `filldraw_rectangle`, `fill_ellipse`, etc.) are drawn on the background layer, ensuring they never obscure grid lines, axes, outlines, labels, or any content drawn on the main layer.