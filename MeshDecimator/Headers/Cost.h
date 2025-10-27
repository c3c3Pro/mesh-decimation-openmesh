#pragma once
#include "MeshTypes.h"

namespace decim {
	//step 1 : local geometric primitives
	double edge_length(const MyMesh& m, Edgehandle e);
	MyMesh::Point face_normal_geom(const MyMesh& m, Facehandle f);
	double dihedral_cosine(const MyMesh& m, Edgehandle e);

	//step 2 : cost = length x curvature penalty (simple heuristic approach)
	double curvature_penalty(const MyMesh& m, Edgehandle e);
	double collapse_cost(const MyMesh& m, Edgehandle e);

}