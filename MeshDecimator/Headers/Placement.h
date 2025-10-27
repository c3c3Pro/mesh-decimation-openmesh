#pragma once

#include "MeshTypes.h"

namespace decim {

	//step 4: placement -> project the one-ring centroid onto the edge 
	//more stable than midpoint
	MyMesh::Point project_centroid_onto_edge(const MyMesh& m,
		Vertexhandle v_keep,
		Vertexhandle v_del);

}