
find_package(Boost ${_Boost_VERSION} REQUIRED COMPONENTS system filesystem)
if(Boost_FOUND)
  set(DEP_INCLUDES ${Boost_INCLUDE_DIRS})
  set(DEP_LIBS ${Boost_LIBRARIES})
endif()

#Definitions for windows compiling
if(WIN32)
  add_definitions(-D_CRT_SECURE_NO_WARNINGS -DTEPLUGINDLL -DBOOST_ALL_NO_LIB)
endif(WIN32)

add_definitions(-DPLUGINS_PATH=\"${CMAKE_RUNTIME_OUTPUT_DIRECTORY}\")

list (APPEND DEP_LIBS terralib_common terralib_xml)

# Select the source and header files
file(GLOB SRCS ${SRCDIR}/*.cpp)
file(GLOB HDRS ${SRCDIR}/*.h)

#exporting module information
exportModuleInformation("plugin" "${SRCDIR}" "plugin")