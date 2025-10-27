# CMakeList.txt : CMake project for MeshDecimator, include source and define
# project specific logic here.

# CMakeList.txt : subproject MeshDecimator

cmake_minimum_required(VERSION 3.21)   
project(MeshDecimator CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Standard install dirs
include(GNUInstallDirs)

find_package(OpenMesh CONFIG REQUIRED)

add_executable(MeshDecimator
  main.cpp
  MeshDecimator.cpp
  io_helper.cpp
  src/Cost.cpp
  src/Guards.cpp
  src/Internals.cpp
  src/Placement.cpp

  Headers/MeshDecimator.h
  io_helper.h
  Headers/MeshTypes.h
  Headers/Cost.h
  Headers/Guards.h
  Headers/Placement.h
  Headers/Internals.h
)

# Link OpenMesh (both modern and legacy target names)
if (TARGET OpenMesh::Core)
  target_link_libraries(MeshDecimator PRIVATE OpenMesh::Core OpenMesh::Tools)
else()
  target_link_libraries(MeshDecimator PRIVATE OpenMeshCore OpenMeshTools)
endif()

# (Optional) If you switch to short includes like #include "MeshTypes.h"
target_include_directories(MeshDecimator PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}/Headers
  ${CMAKE_CURRENT_SOURCE_DIR}
)

if (MSVC)
  target_compile_definitions(MeshDecimator PRIVATE _USE_MATH_DEFINES NOMINMAX)
endif()

# Install rules

# 1) Executable
install(TARGETS MeshDecimator
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
  BUNDLE  DESTINATION ${CMAKE_INSTALL_BINDIR}
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

# 2) Runtime DLLs (dynamic vcpkg triplet)
if (WIN32)
  install(FILES $<TARGET_RUNTIME_DLLS:MeshDecimator>
          DESTINATION ${CMAKE_INSTALL_BINDIR})
  install(CODE "message(STATUS \"Installed runtime DLLs: $<TARGET_RUNTIME_DLLS:MeshDecimator>\")")
endif()

# 3) Examples
if (EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/Examples")
  install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/Examples/
          DESTINATION ${CMAKE_INSTALL_DATADIR}/mesh-decimator/Examples
          FILES_MATCHING PATTERN "*.obj")
endif()

# 4) Public headers
if (EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/Headers")
  install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/Headers/
          DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/mesh-decimator
          FILES_MATCHING PATTERN "*.h")
endif()

# 5) Source files (so the grader sees the code in the runtime ZIP)
file(GLOB_RECURSE MD_SOURCES CONFIGURE_DEPENDS
  "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/*.h"
  "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/src/*.h"
  "${CMAKE_CURRENT_SOURCE_DIR}/Headers/*.h"
)
if (MD_SOURCES)
  list(REMOVE_DUPLICATES MD_SOURCES)
  install(FILES ${MD_SOURCES}
          DESTINATION ${CMAKE_INSTALL_DATADIR}/mesh-decimator/source)
endif()

# 6) README + docs from repo root or here
if (EXISTS "${CMAKE_SOURCE_DIR}/README.md")
  install(FILES "${CMAKE_SOURCE_DIR}/README.md"
          DESTINATION ${CMAKE_INSTALL_DATADIR}/mesh-decimator)
endif()

foreach(DOCROOT IN ITEMS "${CMAKE_SOURCE_DIR}/docs" "${CMAKE_CURRENT_SOURCE_DIR}/docs")
  if (EXISTS "${DOCROOT}")
    install(DIRECTORY "${DOCROOT}/"
            DESTINATION ${CMAKE_INSTALL_DATADIR}/mesh-decimator/docs
            FILES_MATCHING PATTERN "*.pdf")
  endif()
endforeach()

# 7) Include your SUBPROJECT CMakeLists too (so binary ZIP shows build recipe)
install(FILES "${CMAKE_CURRENT_SOURCE_DIR}/CMakeLists.txt"
        DESTINATION ${CMAKE_INSTALL_DATADIR}/mesh-decimator/source)

#  (optional) tests 
if (BUILD_TESTING)
  enable_testing()
  set(EXAMPLE_MESHES armadillo.obj Old_Teapot.obj)
  foreach(mesh IN LISTS EXAMPLE_MESHES)
    get_filename_component(base "${mesh}" NAME_WE)
    add_test(NAME "decim_${base}"
      COMMAND $<TARGET_FILE:MeshDecimator>
              "${CMAKE_CURRENT_SOURCE_DIR}/Examples/${mesh}"
              "${CMAKE_CURRENT_BINARY_DIR}/${base}_out.obj"
              3000
    )
  endforeach()
endif()
