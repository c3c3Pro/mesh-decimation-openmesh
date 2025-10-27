// MeshDecimator.cpp : Defines the entry point for the application.
//

#include "Headers/MeshDecimator.h"
#include "Headers/Internals.h"
#include "Headers/Cost.h"
#include "Headers/Guards.h"
#include "Headers/Placement.h"
#include <queue>
#include <iostream>

namespace decim {

	void decimateByNormalCost(MyMesh& mesh, const DecimateOptions& opt)
	{

		request_status_and_normals(mesh);

		const size_t start_faces = mesh.n_faces();
		size_t goal = (opt.target_faces > 0 && (size_t)opt.target_faces < start_faces)
			? (size_t)opt.target_faces
			: start_faces / 2;
		//decrement it per collapse
		size_t live_faces = start_faces;
		const double sharp_cos = std::cos(opt.sharp_angle_degree * M_PI / 180.0);

		//step 0 : seed queue with current costs
		std::priority_queue<EdgeCost> pq;
		for (auto e = mesh.edges_begin(); e != mesh.edges_end(); ++e)
		{
			if (!mesh.status(*e).deleted())
			{
				pq.push({ *e, collapse_cost(mesh, *e) });
			}
		}
		//update status for debugging
		size_t collapses = 0, skipped_boundary = 0, skipped_sharp = 0, illegal_dir = 0, deleted_edge = 0, pops = 0;
		size_t guard = mesh.n_edges() * 5;

		//pop best, filter, try directions, place, collapse, reinsert neighbors
		while (!pq.empty() && live_faces > goal && guard--)
		{
			auto top = pq.top();
			pq.pop();
			++pops;
			auto eh = top.e;

			if (mesh.status(eh).deleted())
			{
				++deleted_edge;
				continue;
			}
			//boundary + sharpness filters
			if (opt.preserve_boundary && mesh.is_boundary(eh))
			{
				++skipped_boundary;
				continue;
			}
			auto h0 = mesh.halfedge_handle(eh, 0), h1 = mesh.halfedge_handle(eh, 1);
			auto f0 = mesh.face_handle(h0), f1 = mesh.face_handle(h1);
			if (f0.is_valid() && f1.is_valid())
			{
				if (dihedral_cosine(mesh, eh) < sharp_cos)
				{
					++skipped_sharp;
					continue;
				}
			}
			Halfedgehandle handle;
			Vertexhandle v_del, v_keep;
			MyMesh::Point p_new;
			bool ok = false;
			//lambda expression for trying both directions
			auto try_dir = [&](Halfedgehandle h) -> bool {
				if (!mesh.is_collapse_ok(h)) return false;
				v_del = mesh.from_vertex_handle(h);
				v_keep = mesh.to_vertex_handle(h);
				//placement
				const MyMesh::Point candidate = project_centroid_onto_edge(mesh, v_keep, v_del);
				//safety check against flips, degenerate, skinny triangles
				if (would_flip_or_degenerate(mesh, v_keep, v_del, candidate, opt.min_angle_degree))
					return false;

				handle = h;
				p_new = candidate;
				return true;
				
				};
			if (try_dir(h0)) ok = true;
			else if (try_dir(h1)) ok = true;
			if (!ok)
			{
				++illegal_dir;
				continue;
			}
			//how many faces will be removed 
			//1 on border, 2 in interior
			size_t removed = 0;
			auto fh1_opp_he = mesh.opposite_halfedge_handle(handle);
			auto fh0 = mesh.face_handle(handle);
			auto fh1 = mesh.face_handle(fh1_opp_he);

			if (fh0.is_valid())
				++removed;
			if (fh1.is_valid())
				++removed;

			//commit
			mesh.set_point(v_keep, p_new);
			mesh.collapse(handle);
			++collapses;
			live_faces -= removed;
			//progress print to console for debug
			if ((collapses & 511) == 0)
			{
				std::cout << "[info] collapses= " << collapses
					<< " faces= " << live_faces
					<< " pq_size= " << pq.size() << "\n";
			}
			//refresh neighbor's cost
			reinsert_incident_edges(mesh, v_del, pq);
			reinsert_incident_edges(mesh, v_keep, pq);
		}
		mesh.garbage_collection();
		mesh.update_face_normals();
		//print out the final outcome for debug
		std::cout << "Normal cost decimator: " << collapses
			<< " collapses. Faces " << start_faces
			<< " -> " << mesh.n_faces() << "\n";
		std::cout << "Skipped boundary: " << skipped_boundary
			<< ", sharp: " << skipped_sharp
			<< ", illegal: " << illegal_dir
			<< ", already-deleted: " << deleted_edge
			<< ", pops: " << pops << "\n";
	}
}
