#include "Guards.h"

namespace decim {

	MyMesh::Point tri_normal(const MyMesh::Point& a,
		const MyMesh::Point& b,
		const MyMesh::Point& c)
	{
        //cross product of two vectors
        MyMesh::Point n = OpenMesh::cross(b - a, c - a);
        if (n.sqrnorm() == 0)
        {
            return n;
        }
        n.normalize();
        return n;
	}

    float angle_between(const MyMesh::Point& u, const MyMesh::Point& v) {
        double du2 = OpenMesh::dot(u, u), dv2 = OpenMesh::dot(v, v);
        if (du2 <= 0.0 || dv2 <= 0.0) return 0.0f;
        double du = std::sqrt(du2), dv = std::sqrt(dv2);
        double c = clampd(OpenMesh::dot(u, v) / (du * dv), -1.0, 1.0);
        return float(std::acos(c) * 180.0 / M_PI);
    }

    float min_angle_degree_tri(const MyMesh::Point& a,
        const MyMesh::Point& b,
        const MyMesh::Point& c) {
        float A = angle_between(b - a, c - a);
        float B = angle_between(a - b, c - b);
        float C = angle_between(a - c, b - c);
        return std::min(A, std::min(B, C));
    }

    // Skip faces that contain BOTH vertices (they are the two faces that will be deleted).
    bool would_flip_or_degenerate(MyMesh& m,
        Vertexhandle v_keep,
        Vertexhandle v_del,
        const MyMesh::Point& p_new, 
        float min_angle_degree)
    {
        auto check_face = [&](Facehandle f)->bool {
            if (!f.is_valid() || m.status(f).deleted()) return false;
            //build old/new triangles (first 3 vertices)
            MyMesh::Point a, b, c, a0, b0, c0;
            bool has_keep = false, has_del = false;
            int i = 0;
            for (auto fv = m.fv_begin(f); fv.is_valid() && i < 3; ++fv, ++i) {
                auto vh = *fv;
                has_keep |= (vh == v_keep);
                has_del |= (vh == v_del);

                const MyMesh::Point P0 = m.point(vh);
                const MyMesh::Point P = (vh == v_keep || vh == v_del) ? p_new : P0;

                if (i == 0) { a0 = P0; a = P; }
                else if (i == 1) { b0 = P0; b = P; }
                else { c0 = P0; c = P; }
            }

            // Faces incident to both vertices are adjacent to the collapsing edge → will be removed.
            if (has_keep && has_del) return false;

            const MyMesh::Point old_n = tri_normal(a0, b0, c0);
            const MyMesh::Point new_n = tri_normal(a, b, c);

            const float AREA_EPS = 1e-10f;                 // adjust for your mesh scale
            if (old_n.sqrnorm() < AREA_EPS) return true;   // near-degenerate
            if (new_n.sqrnorm() < AREA_EPS) return true;   // would become near-degenerate
            if (OpenMesh::dot(old_n, new_n) <= 0.0f) return true; // orientation flip

            //forbid very skinny triangles (needle shapes)
            if (min_angle_degree > 0.0f && min_angle_degree_tri(a, b, c) < min_angle_degree) return true;

            return false;
            };  

        if (!v_keep.is_valid() || m.status(v_keep).deleted()) return true;
        if (!v_del.is_valid() || m.status(v_del).deleted())  return true;

        for (auto vf = m.vf_begin(v_keep); vf.is_valid(); ++vf)
            if (check_face(*vf)) return true;

        for (auto vf = m.vf_begin(v_del); vf.is_valid(); ++vf)
            if (check_face(*vf)) return true;

        return false; // safe
    }

}