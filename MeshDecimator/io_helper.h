#pragma once

#include<iostream>
#include<OpenMesh/Core/IO/MeshIO.hh>
#include<OpenMesh/Core/Mesh/TriMesh_ArrayKernelT.hh>
#include<filesystem>
#include<cstdlib>
#include<vector>
#include "Headers/MeshTypes.h"

// read/write files

namespace decim {

	bool loadMesh(const std::string& filename, MyMesh& mesh);
	bool saveMesh(const std::string& filename, MyMesh& mesh);

	bool run_by_default_no_args(int argc, const char* argv0, 
		std::string& inputPath, std::string& outputPath);

	void print_usage();

	void wait_if_no_arg(int argc);
}

