window.CampusAPI = {
  async getNodes() {
    const r = await fetch("/api/nodes");
    if (!r.ok) throw Error("Could not load campus nodes");
    return r.json();
  },
  async getGraph() {
    const r = await fetch("/api/graph");
    if (!r.ok) throw Error("Could not load campus graph");
    return r.json();
  },
  async route(from, to) {
    const r = await fetch(
      /api/route?from=${encodeURIComponent(from)}&to=${encodeURIComponent(to)},
    );
    const d = await r.json();
    if (!r.ok) throw Error(d.error || "Route unavailable");
    return d;
  },
};
