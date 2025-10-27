# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.
#
cmake_minimum_required (VERSION 3.21)


# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project ("MeshDecimator")

include(CTest)

# Ship README & PDFs (root/docs or subproject/docs)
install(FILES "${CMAKE_SOURCE_DIR}/README.md"
        DESTINATION "${CMAKE_INSTALL_DATADIR}/mesh-decimator"
        OPTIONAL)
file(GLOB DOC_PDFS
     "${CMAKE_SOURCE_DIR}/MeshDecimator/*.pdf"
     "${CMAKE_SOURCE_DIR}/docs/*.pdf")
if(DOC_PDFS)
  install(FILES ${DOC_PDFS}
          DESTINATION "${CMAKE_INSTALL_DATADIR}/mesh-decimator/docs")
endif()

# Include sub-projects.
add_subdirectory ("MeshDecimator")

# Packaging (CPack) 
set(CPACK_PACKAGE_NAME "MeshDecimator")
set(CPACK_PACKAGE_VENDOR "slbt0676")
set(CPACK_PACKAGE_VERSION "1.0.0")

# Binary package
set(CPACK_GENERATOR "ZIP")

# SOURCE package 
set(CPACK_SOURCE_GENERATOR "ZIP")
set(CPACK_SOURCE_IGNORE_FILES
  "/build/;/.git/;/.vs/;CMakeUserPresets.json;CMakeSettings.json;\\.DS_Store;Thumbs\\.db;~$;\\.#"
)

include(CPack)
