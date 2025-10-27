#include "Internals.h"
#include "Cost.h"

namespace decim {

	void request_status_and_normals(MyMesh& mesh)
	{
		mesh.request_vertex_status();
		mesh.request_edge_status();
		mesh.request_face_status();
		mesh.request_halfedge_status();
		mesh.request_face_normals();
		mesh.update_face_normals();

	}

	void reinsert_incident_edges(MyMesh& m, Vertexhandle v, std::priority_queue<EdgeCost>& pq)
	{
		if (!v.is_valid() || m.status(v).deleted())
		{
			return;
		}
		for (auto ve = m.ve_begin(v); ve.is_valid(); ++ve)
		{
			auto eh = *ve;
			if (!m.status(eh).deleted()) {
				pq.push({ eh, collapse_cost(m, eh) });
			}

		}
	}
}