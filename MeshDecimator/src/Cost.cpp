#include "Cost.h"

namespace decim {
    // difference between two edges i and j
    double edge_length(const MyMesh& m, Edgehandle e)
    {
        Halfedgehandle handle = m.halfedge_handle(e, 0);
        MyMesh::Point p0 = m.point(m.from_vertex_handle(handle));
        MyMesh::Point p1 = m.point(m.to_vertex_handle(handle));
        MyMesh::Point diff = p0 - p1;

        return std::sqrt(double(diff[0] * diff[0] + diff[1] * diff[1] + diff[2] * diff[2]));

    }
    MyMesh::Point face_normal_geom(const MyMesh& m, Facehandle f)
    {
        MyMesh::Point a, b, c; int i = 0;
        for (auto fv = m.cfv_begin(f); fv.is_valid() && i < 3; ++fv, ++i) {
            auto vh = *fv;
            const MyMesh::Point P = m.point(vh);
            if (i == 0) a = P;
            else if (i == 1) b = P;
            else             c = P;
        }
        MyMesh::Point n = OpenMesh::cross(b - a, c - a);
        if (n.sqrnorm() == 0) return n;
        n.normalize();
        return n;
    }

    // calculate the angle between two planes
    double dihedral_cosine(const MyMesh& m, Edgehandle e)
    {
        Halfedgehandle h0 = m.halfedge_handle(e, 0);
        Halfedgehandle h1 = m.halfedge_handle(e, 1);
        Facehandle f0 = m.face_handle(h0);
        Facehandle f1 = m.face_handle(h1);

        if (!f0.is_valid() || !f1.is_valid())
            return 1.0; // treat boundary as flat

        const MyMesh::Point n0 = face_normal_geom(m, f0);
        const MyMesh::Point n1 = face_normal_geom(m, f1);

        const double dp = double(n0[0] * n1[0] + n0[1] * n1[1] + n0[2] * n1[2]);
        return clampd(dp, -1.0, 1.0);
    }
    // curvature penalty in [0, 1]
    // 0 for flat, 1 for 180 degrees fold
    double curvature_penalty(const MyMesh& m, Edgehandle e)
    {
        double cosine = dihedral_cosine(m, e);
        // 0 flat, 1 for 180 (opposite)
        return (1.0 - cosine) * 0.5;
    }

    double collapse_cost(const MyMesh& m, Edgehandle e)
    {
        return edge_length(m, e) * curvature_penalty(m, e);
    }
}