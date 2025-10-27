// MeshDecimator.h : Include file for standard system include files,
// or project specific include files.

#pragma once


#include "MeshTypes.h"

namespace decim {

	struct DecimateOptions {
		int target_faces          = -1;
		bool preserve_boundary    = true;
		double sharp_angle_degree = 30.0;
		float min_angle_degree    = 12.0f;
		bool do_relax             = false;
		int relax_iters           = 2;
		float relax_lambda        = 0.2f;
	};

	void decimateByNormalCost(MyMesh& mesh, const DecimateOptions& opt);
}

