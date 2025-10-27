#include "io_helper.h"



namespace decim {
	namespace fs = std::filesystem;

	bool loadMesh(const std::string& filename, MyMesh& mesh)
	{
		OpenMesh::IO::Options opts;
		// allow status if present
		opts += OpenMesh::IO::Options::VertexNormal;
		opts += OpenMesh::IO::Options::FaceNormal;
		opts += OpenMesh::IO::Options::Status;

		// read the file
		if (!OpenMesh::IO::read_mesh(mesh, filename, opts))
		{
			std::cerr << "Error: cannot read " << filename << std::endl;
			return false;
		}
		// request normals so they can be written if needed
		if (!mesh.has_face_normals())
		{
			mesh.request_face_normals();
		}
		if (!mesh.has_vertex_normals())
		{
			mesh.request_vertex_colors();
		}

		std::cout << "loaded: " << mesh.n_vertices() << " vertices, " << mesh.n_faces() << " faces\n";

		return true;
	}

	bool saveMesh(const std::string& filename, MyMesh& mesh)
	{
		OpenMesh::IO::Options opts;
		OpenMesh::IO::Options::VertexNormal;
		OpenMesh::IO::Options::FaceNormal;

		if (!OpenMesh::IO::write_mesh(mesh, filename, opts))
		{
			std::cerr << "Error: cannot write " << filename << std::endl;
			return false;
		}

		std::cout << "saved to: " << filename << "\n";

		return true;
	}
	bool run_by_default_no_args(int argc, const char* argv0, std::string& inputPath, std::string& outputPath)
	{
		if (argc >= 3) return false;
		
		std::error_code ec;
		fs::path exeDir;

		if (argv0) exeDir = fs::absolute(argv0, ec).parent_path();
		fs::path cwd = fs::current_path();

		// Keep it short: check a few likely spots only
		const std::vector<fs::path> candidates = {
			cwd / "share/mesh-decimator/Examples/Old_Teapot.obj",
			cwd / "share/mesh-decimator/Examples/armadillo.obj",
			exeDir / ".." / "share" / "mesh-decimator" / "Examples" / "Old_Teapot.obj",
			exeDir / ".." / "share" / "mesh-decimator" / "Examples" / "armadillo.obj",
			cwd / "MeshDecimator" / "Examples" / "Old_Teapot.obj",
			cwd / "MeshDecimator" / "Examples" / "armadillo.obj"
		};
		for (const auto& p : candidates)
		{
			if (fs::exists(p, ec) && !ec) {
				inputPath = p.string();
				break;
			}
		}
		//no example found -> let main print usage
		if (inputPath.empty()) return false;
		//write result next to the current working dir
		outputPath = (cwd / "auto_out.obj").string();

		std::cout << "[info] No arguments; using bundled example:\n"
			<< "  input  = " << inputPath << "\n"
			<< "  output = " << outputPath << "\n";

		return true;
	}
	void print_usage()
	{
		std::cout <<
			"MeshDecimator - Edge-collapse decimator (OpenMesh)\n"
			"Usage:\n"
			"  MeshDecimator <input.obj> <output.obj> [target_faces]\n"
			"  MeshDecimator --help\n";
	}
	void wait_if_no_arg(int argc)
	{
		#ifdef _WIN32
		if (argc < 3) {                // only pause when launched without explicit args
			std::cout << "Press Enter to exit...";
			std::cin.get();
		}
		#endif
	}
}
