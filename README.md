# Campus Navigation Web App

A responsive campus map with SVG curved roads, searchable route controls, and a C++17 HTTP backend. Routing uses Dijkstra over validated directed and two-way CSV edges. Junctions are represented as normal graph nodes and can be highlighted by the UI.

## Build and run
```sh
cmake -S . -B build
cmake --build build --config Debug -j
ctest --test-dir build --output-on-failure
./build/backend/campus_server . 8080
```
Open <http://localhost:8080>. The server logs every request and serves `frontend/` plus:

- `GET /api/nodes` — `{nodes:[{id,name,type}]}`, excluding junctions and sorted by name
- `GET /api/graph` — graph nodes with `x,y` coordinates and edges with `id,from,to,direction,distanceM`
- `GET /api/route?from=gate_2&to=cs_it_block` — success returns `{found:true,from,to,path,stops,totalDistanceM,estimatedWalkMinutes,warnings}`; an unreachable route returns `{found:false,reason,warnings}` and missing or invalid IDs return HTTP 400

Use `-DCAMPUS_FETCH_DEPS=ON` to fetch cpp-httplib and nlohmann/json from their official repositories. The checked-in vendor headers provide an offline fallback.

CSV validation reports the source file and line number. `nodes.csv` uses `id,name,type,x,y,notes`; `edges.csv` uses `id,from,to,direction,distance_m`. A blank distance is unknown, is skipped by routing, and appears in warnings. `one_way` permits travel only from `from` to `to`; `two_way` adds both directions. The x/y values are sketch coordinates, not GPS.

## Changelog

### 1.1.0
- Replaced the legacy layout with the specified backend/src, backend/tests, frontend/css, and frontend/js structure.
- Added `/api/nodes`, `/api/graph`, and `/api/route`, request logging, startup validation, junctions, unknown-distance warnings, one-way/two-way routing, and unreachable-route handling.
- Added curved SVG roads, route highlighting, search/swap/clear controls, responsive styling, and reduced-motion support.

### 1.2.0
- Replaced placeholder data with the supplied Graphic Era campus nodes and e01-e38 edge map.
- Added exact route response fields, one-decimal walking estimates, and skipped unknown-distance warnings.

### 1.3.0
- Redesigned the map with a readable Google Maps-inspired visual style, wider building spacing, labeled building cards, road casing, map controls, pan/zoom, pins, and route animation.

### 1.4.0
- Spread the right-side hostel and CS/IT area, changed roads to smoother cubic curves, added map legend, improved initial route choices, and clarified map interaction feedback.

### 1.5.0
- Reworked the Small Box area with dedicated positions and compact labels, separated its junction corridor, and increased road casing contrast so paths remain readable beneath buildings.

### 1.6.0
- Enlarged the map presentation with a taller spacious canvas and added persistent blue start-location dots and red pointed destination markers.

### 1.7.0
- Aligned Gate 1 and Gate 2 as parallel lower entrances, widened the entrance layout, and changed map roads and highlighted routes to straight segments for easier tracing.

### 1.8.0
- Moved Tuck Shop farther left from Btech Block in the visual map layout while preserving the existing graph data and route distances.

### 1.9.0
- Aligned Tuck Shop horizontally with Btech Block, widened the Gate 1/Gate 2 separation, and spread Parking and Mechanical Block between the entrances.

### 1.10.0
- Kept Tuck Shop fully inside the map while preserving its parallel alignment with Btech Block, and separated Parking from Mechanical Block to prevent label overlap.

### 1.11.0
- Corrected campus labels: Param Lab, Hotel Management Block, Law Building, and Quick Bite Cafe.

### 1.12.0
- Widened the upper campus corridor by separating Tuck Shop and Btech Block, repositioned the Small Box buildings, and increased straight-road contrast for a clearer graph.

### 1.13.0
- Moved Chanakya Block diagonally farther from the upper-left cluster and brought Santosh Library closer to Btech Block in the visual map layout.

### 1.14.0
- Removed remaining visual block collisions by separating Chanakya/Param Lab and Santosh/Btech/Happiness Cafe cards while retaining straight graph paths.

### 1.15.0
- Grouped Hotel Management Block, Law Building, and Quick Bite Cafe with consistent spacing, and shifted Btech Block left with its junction connection.

### 1.16.0
- Completed a visual collision pass by moving Happiness Cafe away from Ravi Canteen while preserving the requested lower-left building grouping.

### 1.17.0
- Changed map locations to clickable dots; individual labels appear on click, while all non-junction stops on a found route expand into labeled building cards.

### 1.18.0
- Expanded laptop map spacing and road widths for clearer long-distance tracing, and fixed dot click handling so building labels reliably appear.

### 1.23.0
- Restored bidirectional access to Civil Block so Civil Block, Basketball Court, and Badminton Court can all be selected and displayed in route-only maps from either campus entrance.
- Fixed route-only building labels to show only locations that are actual stops on the selected path, rather than unrelated buildings branching from route junctions.
- Kept all building nodes visible as dots on route maps while limiting displayed roads to the selected path; selected route stops remain labeled.
- Ensured reverse travel across two-way edges renders the corresponding route segment.
- Made map locations visual-only; building dots and route blocks are no longer clickable.
- Showed names for every non-junction building/location on the route screen while keeping non-route roads hidden.

### 1.19.0
- Removed visible junction dots and codes; junctions remain internal routing points that connect roads without adding map clutter.

### 1.20.0
- Added a two-stage route experience: the initial screen only asks for start and destination, then a route-only screen displays just the selected path, route stops, distance, and navigation controls.

### 1.21.0
- Added route-context building labels and a “Buildings along this path” list for buildings connected to the selected route corridor.

### 1.22.0
- Added distances for Hotel Management Block (20 m), Law Building (15 m), and Quick Bite Cafe (15 m); e30 remains an unknown-distance edge.

### 1.0.0
- Initial campus graph demo and static web application.
