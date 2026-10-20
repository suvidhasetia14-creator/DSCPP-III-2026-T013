window.CampusMap = (() => {
  const W = 1030,
    H = 1280,
    ox = 18,
    oy = 20;
  let graph,
    svg,
    world,
    byId = {},
    lastRoute = [],
    startId = "",
    endId = "",
    routeOnly = false,
    zoom = 1,
    panX = 0,
    panY = 0,
    drag = null;
  const colors = {
    building: "#d9e2ec",
    court: "#b7e3df",
    cafe: "#ffe0a6",
    canteen: "#ffd28a",
    hostel: "#d8c4ed",
    entrance: "#f8b4ae",
    "open area": "#bfe3bd",
    shop: "#ffd28a",
    area: "#cbd5df",
    unlabelled: "#e5e7eb",
  };
  // Spread the dense north-west cluster while retaining the long south road.
  const layout = {
    gate_2: { x: 55, y: 820 },
    gate_1: { x: 485, y: 820 },
    parking: { x: 175, y: 700 },
    mechanical_block: { x: 390, y: 700 },
    chanakya_block: { x: 105, y: 235 },
    parent_lab: { x: 270, y: 285 },
    santosh_library: { x: 470, y: 300 },
    tuck_shop: { x: 70, y: 365 },
    btech_block: { x: 520, y: 410 },
    happiness_cafe: { x: 850, y: 350 },
    small_box_a: { x: 125, y: 450 },
    small_box_b: { x: 270, y: 450 },
    small_box_c: { x: 415, y: 450 },
    j_l1: { x: 55, y: 345 },
    j_l2: { x: 150, y: 345 },
    j_l3: { x: 265, y: 345 },
    j_l4: { x: 390, y: 345 },
    j_l5: { x: 520, y: 345 },
  };
  const pos = (n) => {
    const p = layout[n.id] || {
      x:
        n.x < 250
          ? n.x * 1.25
          : n.x > 600
            ? 650 + (n.x - 600) * 1.05
            : 312.5 + (n.x - 250),
      y: n.y < 450 ? n.y * 1.1 : 495 + (n.y - 450) * 0.95,
    };
    return {
      x: ox + Math.min(970, p.x * 1.08),
      y: oy + Math.min(1200, p.y * 1.08),
    };
  };
  const esc = (s) =>
    String(s).replace(
      /[&<>"]/g,
      (c) => ({ "&": "&amp;", "<": "&lt;", ">": "&gt;", '"': "&quot;" })[c],
    );
  function pathBetween(a, b) {
    const p = pos(a),
      q = pos(b);
    return M${p.x},${p.y} L${q.x},${q.y};
  }
  function labelLines(name) {
    const words = name.split(" "),
      lines = [];
    let line = "";
    for (const word of words) {
      if ((line + " " + word).trim().length > 16) {
        lines.push(line);
        line = word;
      } else line = (line + " " + word).trim();
    }
    if (line) lines.push(line);
    return lines.slice(0, 3);
  }
  function render(route = []) {
    lastRoute = route;
    const active = new Set(route);
    world.innerHTML = "";
    world.insertAdjacentHTML(
      "beforeend",
      '<rect class="map-bg" width="1030" height="1280"/>',
    );
    world.insertAdjacentHTML(
      "beforeend",
      '<path class="campus-boundary" d="M35 50 H990 V1230 H35 Z"/>',
    );
    graph.edges.forEach((e) => {
      const a = byId[e.from],
        b = byId[e.to],
        onRoute =
          !routeOnly ||
          route.some((id, i) => {
            const next = route[i + 1];
            return (
              (id === e.from && next === e.to) ||
              (!e.oneWay && id === e.to && next === e.from)
            );
          });
      if (!a || !b || !onRoute) return;
      const d = pathBetween(a, b);
      world.insertAdjacentHTML(
        "beforeend",
        <path class="road-casing" d="${d}"/><path class="road-surface ${e.distanceM === null ? "unknown" : ""}" d="${d}"/>,
      );
    });
    if (route.length > 1) {
      for (let i = 1; i < route.length; i++) {
        const a = byId[route[i - 1]],
          b = byId[route[i]];
        if (a && b)
          world.insertAdjacentHTML(
            "beforeend",
            <path class="route-line" d="${pathBetween(a, b)}"/>,
          );
      }
    }
    graph.nodes
      .filter((n) => n.type !== "junction")
      .forEach((n) => {
        const p = pos(n),
          visible = routeOnly || active.has(n.id),
          r = active.has(n.id) ? 13 : 8,
          selected = active.has(n.id);
        let card = "";
        if (visible) {
          const lines = labelLines(n.name),
            w = Math.max(
              106,
              Math.min(205, Math.max(...lines.map((x) => x.length)) * 8 + 30),
            ),
            h = lines.length * 17 + 26,
            text = lines
              .map(
                (l, i) =>
                  <tspan x="${p.x}" dy="${i ? "17" : 0}">${esc(l)}</tspan>,
              )
              .join("");
          card = <rect class="building-card" x="${p.x - w / 2}" y="${p.y - h / 2 - 24}" width="${w}" height="${h}" rx="10" fill="${colors[n.type] || colors.building}"/><text class="building-label" x="${p.x}" y="${p.y - (lines.length - 1) * 8 - 24}" text-anchor="middle">${text}</text>;
        }
        world.insertAdjacentHTML(
          "beforeend",
          <g class="location-dot-group ${selected ? "route-stop" : ""}">${card}<circle class="location-dot-map" cx="${p.x}" cy="${p.y}" r="${r}"/><circle class="location-halo" cx="${p.x}" cy="${p.y}" r="${r + 7}"/></g>,
        );
      });
    const start = byId[startId || route[0]],
      end = byId[endId || route[route.length - 1]];
    if (start) {
      const p = pos(start);
      world.insertAdjacentHTML(
        "beforeend",
        <circle class="location-dot" cx="${p.x}" cy="${p.y}" r="13"/><circle class="location-dot-pulse" cx="${p.x}" cy="${p.y}" r="22"/>,
      );
    }
    if (end) {
      const p = pos(end);
      world.insertAdjacentHTML(
        "beforeend",
        <path class="destination-pin" d="M${p.x},${p.y + 25} L${p.x - 16},${p.y - 12} Q${p.x},${p.y - 27} ${p.x + 16},${p.y - 12} Z"/><circle class="destination-pin-core" cx="${p.x}" cy="${p.y - 10}" r="5"/>,
      );
    }
    applyView();
  }
  function applyView() {
    world.setAttribute(
      "transform",
      translate(${panX} ${panY}) translate(${W / 2} ${H / 2}) scale(${zoom}) translate(${-W / 2} ${-H / 2}),
    );
  }
  function reset() {
    zoom = 1;
    panX = 0;
    panY = 0;
    applyView();
  }
  function init(g, s) {
    graph = g;
    svg = s;
    byId = Object.fromEntries(g.nodes.map((n) => [n.id, n]));
    svg.innerHTML = '<g id="map-world"></g>';
    world = svg.querySelector("#map-world");
    svg.addEventListener(
      "wheel",
      (e) => {
        e.preventDefault();
        zoom = Math.max(
          0.75,
          Math.min(1.8, zoom + (e.deltaY < 0 ? 0.1 : -0.1)),
        );
        applyView();
      },
      { passive: false },
    );
    svg.addEventListener("pointerdown", (e) => {
      drag = { x: e.clientX - panX, y: e.clientY - panY };
      svg.setPointerCapture(e.pointerId);
    });
    svg.addEventListener("pointermove", (e) => {
      if (drag) {
        panX = e.clientX - drag.x;
        panY = e.clientY - drag.y;
        applyView();
      }
    });
    svg.addEventListener("pointerup", () => (drag = null));
    render();
  }
  return {
    init,
    render,
    setEndpoints(a, b) {
      startId = a || "";
      endId = b || "";
      render(lastRoute);
    },
    setRouteOnly(v) {
      routeOnly = v;
      render(lastRoute);
    },
    setJunctionsVisible() {},
    zoomIn() {
      zoom = Math.min(1.8, zoom + 0.15);
      applyView();
    },
    zoomOut() {
      zoom = Math.max(0.75, zoom - 0.15);
      applyView();
    },
    reset,
  };
})();
