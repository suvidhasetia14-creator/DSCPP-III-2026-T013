(async () => {
  const $ = (id) => document.getElementById(id),
    from = $("from"),
    to = $("to"),
    status = $("status"),
    summary = $("summary"),
    stops = $("stops"),
    warnings = $("warnings"),
    routeBuildings = $("route-buildings");
  let all = [],
    graph;
  function fill(select, query) {
    const value = select.value;
    select.replaceChildren(
      ...all
        .filter((n) => n.name.toLowerCase().includes(query.toLowerCase()))
        .map((n) => new Option(n.name, n.id)),
    );
    if ([...select.options].some((o) => o.value === value))
      select.value = value;
  }
  function buildingsAlongPath(path) {
    const byId = new Map(graph.nodes.map((n) => [n.id, n]));
    return path
      .map((id) => byId.get(id))
      .filter((n) => n && n.type !== "junction")
      .map((n) => ({ id: n.id, name: n.name }));
  }
  function showResult(r) {
    const along = buildingsAlongPath(r.path || []);
    summary.textContent = r.found
      ? ${r.totalDistanceM} m · ${r.estimatedWalkMinutes} minutes
      : r.reason || "No route found";
    stops.replaceChildren(
      ...(r.stops || []).map((s) => {
        const li = document.createElement("li");
        li.textContent = s.name;
        return li;
      }),
    );
    warnings.replaceChildren(
      ...(r.warnings || []).map((w) => {
        const li = document.createElement("li");
        li.textContent = w;
        return li;
      }),
    );
    routeBuildings.replaceChildren(
      ...along.map((s) => {
        const li = document.createElement("li");
        li.textContent = s.name;
        return li;
      }),
    );
  }
  try {
    const [n, g] = await Promise.all([
      CampusAPI.getNodes(),
      CampusAPI.getGraph(),
    ]);
    all = n.nodes;
    graph = g;
    fill(from, "");
    fill(to, "");
    from.value = "gate_2";
    to.value = "cs_it_block";
    CampusMap.init(graph, $("map"));
    CampusMap.setEndpoints(from.value, to.value);
    status.textContent = "Select a start and destination to begin.";
  } catch (e) {
    status.textContent = e.message;
    return;
  }
  ["from-search", "to-search"].forEach(
    (id, i) => ($(id).oninput = (e) => fill(i ? to : from, e.target.value)),
  );
  from.onchange = () => CampusMap.setEndpoints(from.value, to.value);
  to.onchange = () => CampusMap.setEndpoints(from.value, to.value);
  $("search").onclick = async () => {
    if (from.value === to.value) {
      status.textContent = "Choose two different locations.";
      return;
    }
    status.textContent = "Computing route…";
    try {
      const r = await CampusAPI.route(from.value, to.value);
      if (!r.found) {
        status.textContent = r.reason;
        return;
      }
      showResult(r);
      $("route-title").textContent =
        ${from.selectedOptions[0].text} to ${to.selectedOptions[0].text};
      $("planner").classList.add("hidden");
      $("route-screen").classList.remove("hidden");
      CampusMap.setRouteOnly(true);
      CampusMap.render(r.path || []);
    } catch (e) {
      status.textContent = e.message;
    }
  };
  $("swap").onclick = () => {
    const v = from.value;
    from.value = to.value;
    to.value = v;
    CampusMap.setEndpoints(from.value, to.value);
    if (from.value && to.value) $("search").click();
  };
  $("clear").onclick = () => {
    from.value = "";
    to.value = "";
    $("from-search").value = "";
    $("to-search").value = "";
    summary.textContent = "";
    stops.replaceChildren();
    warnings.replaceChildren();
    status.textContent = "Select a start and destination to begin.";
    CampusMap.setEndpoints("", "");
    CampusMap.setRouteOnly(false);
    CampusMap.render([]);
  };
  $("back-to-planner").onclick = () => {
    $("route-screen").classList.add("hidden");
    $("planner").classList.remove("hidden");
    CampusMap.setRouteOnly(false);
    CampusMap.render([]);
    status.textContent = "Select a start and destination to begin.";
  };
  $("zoom-in").onclick = () => CampusMap.zoomIn();
  $("zoom-out").onclick = () => CampusMap.zoomOut();
  $("reset-map").onclick = () => CampusMap.reset();
})();
