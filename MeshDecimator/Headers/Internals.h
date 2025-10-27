#pragma once
#include "MeshTypes.h"
#include <queue>

namespace decim {

	struct EdgeCost {

		Edgehandle e;
		double cost;
		bool operator<(const EdgeCost& o) const {
			return cost > o.cost; //min heap
		}
	};
	//Openmesh bookkeeping required for collapses
	void request_status_and_normals(MyMesh& mesh);
	//reinsert edges incident to a vertex with refreshed costs
	void reinsert_incident_edges(MyMesh& m, Vertexhandle v, std::priority_queue<EdgeCost>& pq);
}