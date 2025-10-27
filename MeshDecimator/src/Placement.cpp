#include "Headers/Placement.h"
#include "Headers/MeshTypes.h"
#include<algorithm>

namespace decim {

	MyMesh::Point project_centroid_onto_edge(const MyMesh& m, Vertexhandle v_keep, Vertexhandle v_del)
	{
		MyMesh::Point c(0, 0, 0); int cnt = 0;
		auto accumulate = [&](Vertexhandle v) {
			for (auto vv = m.cvv_begin(v); vv.is_valid(); ++vv) {
				auto u = *vv; if (u == v_keep || u == v_del) continue;
				c += m.point(u); ++cnt;
			}
			};
		accumulate(v_keep); accumulate(v_del);
		if (cnt == 0) return (m.point(v_keep) + m.point(v_del)) * 0.5f;

		c *= (1.0f / cnt);
		MyMesh::Point p0 = m.point(v_keep), p1 = m.point(v_del);
		MyMesh::Point d = p1 - p0;
		double denom = OpenMesh::dot(d, d);
		if (denom <= 0.0) return p0;
		double t = OpenMesh::dot(c - p0, d) / denom;
		t = std::max(0.0, std::min(1.0, t));
		return p0 + d * t;

	}
}