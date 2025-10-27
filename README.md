## MeshDecimator

A small edge-collapse mesh decimator built with CMake and OpenMesh.
Heuristic cost = edge length × curvature (dihedral-angle based),
placement = centroid projection onto the edge,
plus guards to avoid boundary damage and flipped/degenerate triangles.

## Quick Run (from the ZIP file you received)

No setup needed 

1.Unzip the archive.
2.Open PowerShell in the unzipped folder.
3.Run:
.\bin\MeshDecimator.exe

You should see a message that it auto-picked an example and wrote:
auto_out.obj
next to the executable.

use the following command to run explicitly on an example with a targeted face count:
For Windows (PowerShell):

.\bin\MeshDecimator.exe .\share\mesh-decimator\Examples\armadillo.obj .\armadillo_out.obj 3000

For macOS/Linux:

./bin/MeshDecimator ./share/mesh-decimator/Examples/armadillo.obj ./armadillo_out.obj 3000

This writes armadillo_out.obj next to the executable. 
Replace the input with Old_Teapot.obj to try the other example.
You can also try other .obj models for more testing

Command-Line usage:
MeshDecimator <input.obj> <output.obj> [target_faces]

Defaults:
target_faces : 3000
preserve_boundary = true
sharp_angle_degree = 30
min_angle_degree = 12

Note: target_faces is a goal, not a guarantee. Collapses stop early if only “bad” collapses remain 
(e.g., would flip/degenerate or break boundaries).

Use --help to print a short usage message.

## Build from source 
Requirements:

CMake >= 3.20
c++ 17 compiler (MSVC/Clang/GCC)
OpenMesh (easiest via vcpkg)

Installing vcpkg:
For windows:

cd $env:USERPROFILE
git clone https://github.com/microsoft/vcpkg.git
cd vcpkg
.\bootstrap-vcpkg.bat
.\vcpkg.exe install openmesh:x64-windows

configure, build, test:

cd "C:\path\to\MeshDecimator"
cmake -S . -B build -G "Visual Studio 17 2022" -A x64 `
  -DBUILD_TESTING=ON `
  -DCMAKE_TOOLCHAIN_FILE="$env:USERPROFILE\vcpkg\scripts\buildsystems\vcpkg.cmake"
cmake --build build --config Release --parallel
ctest --test-dir build -C Release --output-on-failure

run:

.\build\MeshDecimator\Release\MeshDecimator.exe MeshDecimator\Examples\armadillo.obj build\armadillo_out.obj 3000

With macOS/Linux:

# (optional) install openmesh via vcpkg first:
git clone https://github.com/microsoft/vcpkg $HOME/vcpkg
$HOME/vcpkg/bootstrap-vcpkg.sh
$HOME/vcpkg/vcpkg install openmesh

cmake -S . -B build -DCMAKE_BUILD_TYPE=Release -DBUILD_TESTING=ON \
  -DCMAKE_TOOLCHAIN_FILE="$HOME/vcpkg/scripts/buildsystems/vcpkg.cmake"
cmake --build build -j
ctest --test-dir build --output-on-failure
./build/MeshDecimator/MeshDecimator MeshDecimator/Examples/armadillo.obj build/armadillo_out.obj 3000

If you installed OpenMesh manually (not via vcpkg), point CMake to it:

-DOpenMesh_DIR=/path/to/OpenMesh/lib/cmake/OpenMesh

Install and Package (for distribution)

create a clean install tree and a single ZIP with the binary + examples:

cmake --install build --prefix build/install
cpack --config build/CPackConfig.cmake

The ZIP contains:

bin/MeshDecimator[.exe]
share/mesh-decimator/Examples/armadillo.obj
share/mesh-decimator/Examples/Old_Teapot.obj
and additionally the following :
README.md
Dokumentation.pdf

Tests

ctest runs two example decimations and writes results into the build directory:

decim_armadillo → armadillo_out.obj
decim_Old_Teapot → Old_Teapot_out.obj

List test:

ctest --test-dir build -N

Troubleshooting:

CMake can't find OpenMesh : use vcpkg and pass the toolchain file, or
set OpenMesh_DIR to the folder containing OpenMeshConfig.cmake

"No tests were found" : ensure top-level CMakeLists.txt has include(CTest) and 
reconfigure from a clean build/ folder.

Path issues: note that example meshes are installed under share/mesh-decimator/Examples/

## Algorithm

This implementation uses half-edge connectivity and repeatedly collapses the cheapest edge,
where cost = edge length × curvature (curvature from the dihedral cosine between adjacent face 
normals). The new vertex position is set by projecting the local neighbor centroid onto the edge,
which preserves shape better than a plain midpoint. Before collapsing, we enforce guards: keep boundaries 
if requested, skip “sharp” edges under a user angle threshold, and reject any collapse that would flip triangle 
orientation or make faces degenerate (near-zero area). As a result, the surface retains features and avoids 
artifacts while reducing faces toward the requested budget.
