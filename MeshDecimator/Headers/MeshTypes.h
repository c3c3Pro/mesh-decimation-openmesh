#pragma once

#include<OpenMesh/Core/Mesh/TriMesh_ArrayKernelT.hh>
#include<OpenMesh/Core/IO/MeshIO.hh>
#include<OpenMesh/Core/Geometry/VectorT.hh>
#include<cmath>
#include<algorithm>

namespace decim {
	//choose default traits
	struct Traits : public OpenMesh::DefaultTraits {};
	//my frequently used mesh type
	using MyMesh = OpenMesh::TriMesh_ArrayKernelT<Traits>;

	//handle aliases for readability
	using Vertexhandle = OpenMesh::VertexHandle;
	using Halfedgehandle = OpenMesh::HalfedgeHandle;
	using Edgehandle = OpenMesh::EdgeHandle;
	using Facehandle = OpenMesh::FaceHandle;
	
	//small utility
	static inline double clampd(double x, double a, double b)
	{
		return std::clamp(x, a, b);
	}

}