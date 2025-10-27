#pragma once
#include "MeshTypes.h"

namespace decim {
	//triangle normal (unit, zero vector if degenerate)
	MyMesh::Point tri_normal(const MyMesh::Point& a,
		const MyMesh::Point& b,
		const MyMesh::Point& c);

	//angle utilities in degrees
	float angle_between(const MyMesh::Point& u, const MyMesh::Point& v);
	float min_angle_degree_tri(const MyMesh::Point& a,
		const MyMesh::Point& b,
		const MyMesh::Point& c);
	//step 3 : safety -> forbid flips, degenerates, skinny triangles
	//checks both one rings (v_keep and v_del)
	//ignores the two faces that will be removed
	bool would_flip_or_degenerate(MyMesh& m,
		Vertexhandle v_keep,
		Vertexhandle v_del,
		const MyMesh::Point& p_new,
		float min_angle_degree);
}