Design notes
The application is divided into configuration, CSV parsing/loading, graph storage, route service, and HTTP serving. DataLoader validates every row and includes the source line in failures. An edge marked one-way contributes one adjacency entry; two-way contributes both. Unknown distances are represented explicitly and use a unit routing weight while returning a warning to the client.

The map uses latitude/longitude values as a simple local projection for the sample campus. Roads are rendered as quadratic SVG paths, with unknown roads dashed and the active route highlighted. The browser honors prefers-reduced-motion, and all controls remain usable on narrow screens.

The root CMake file owns testing and optional official FetchContent declarations. backend/CMakeLists.txt owns the backend library and executable. Vendor headers allow configure/build in offline environments without changing the official dependency declarations.
