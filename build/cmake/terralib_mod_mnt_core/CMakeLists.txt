#
# Include path to all dependent libraries
#
include_directories(${Boost_INCLUDE_DIR})
 
#
# Export as DLL on Microsoft Windows
#
if(WIN32)
  add_definitions(-DTEMNTDLL)
endif()
 
#
# Select all the files for this project.
#
file(GLOB TERRALIB_SRC_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/mnt/core/*.cpp)
file(GLOB TERRALIB_HDR_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/mnt/core/*.h)
 
#
# Group files in specific folders for some IDEs (VC++, Xcode, Eclipse).
#
source_group("Source Files"  FILES ${TERRALIB_SRC_FILES})
source_group("Header Files"  FILES ${TERRALIB_HDR_FILES})
 
#
# Define the core module as a shared library.
#
add_library(terralib_mod_mnt_core SHARED ${TERRALIB_SRC_FILES} ${TERRALIB_HDR_FILES})
 
#
# Link with dependent libraries and modules.
#
target_link_libraries(terralib_mod_mnt_core terralib_mod_common terralib_mod_raster terralib_mod_geometry terralib_mod_memory terralib_mod_dataaccess terralib_mod_datatype)
 
#
# Versioning the new module library on Unix-like systems.
#
set_target_properties(terralib_mod_mnt_core
                      PROPERTIES VERSION ${TERRALIB_VERSION_MAJOR}.${TERRALIB_VERSION_MINOR}
                                 SOVERSION ${TERRALIB_VERSION_MAJOR}.${TERRALIB_VERSION_MINOR}
                                 INSTALL_NAME_DIR "@executable_path/../lib")
 
#
# Register module stuffs for installation.
#
install(TARGETS terralib_mod_mnt_core
        EXPORT terralib-targets
        RUNTIME DESTINATION ${TERRALIB_DESTINATION_RUNTIME} COMPONENT runtime
        LIBRARY DESTINATION ${TERRALIB_DESTINATION_LIBRARY} COMPONENT runtime
        ARCHIVE DESTINATION ${TERRALIB_DESTINATION_ARCHIVE} COMPONENT runtime)
 
install(FILES ${TERRALIB_HDR_FILES}
        DESTINATION ${TERRALIB_DESTINATION_HEADERS}/terralib/mnt/core COMPONENT devel)
 
#
# Register module in CMake export file.
#
export(TARGETS terralib_mod_mnt_core APPEND FILE ${CMAKE_BINARY_DIR}/terralib-exports.cmake)