# IMPORTANT - Controls for the source and build tree locations
set(ROOT "${CMAKE_CURRENT_SOURCE_DIR}/../") 	# the terralib root directory (change it if it is in another directory)
set(SRCDIR ${ROOT}/third-party/qt/propertybrowser)   	# directory where the source files of the 
set(INCLDIR ${SRCDIR})                               	# directory where the header files of the 

# Set the project name
set(PROJ_NAME "qtpropertybrowser")
project(${PROJ_NAME})         							# the project name is 

find_package(Qt4 ${_Qt4_VERSION} REQUIRED COMPONENTS QtCore QtGui)

if(QT4_FOUND)
  include (${QT_USE_FILE})
  add_definitions (${QT_DEFINITIONS})

  list (APPEND _dep_incs ${QT_INCLUDE_DIR})
  list (APPEND _dep_libs ${QT_LIBRARIES})
endif()

#Definitions for windows compiling
if(WIN32)
  add_definitions(-DQT_QTPROPERTYBROWSER_EXPORT)
endif(WIN32)

set (sources ${INCLDIR}/qtpropertybrowser.cpp 
             ${INCLDIR}/qtpropertymanager.cpp
             ${INCLDIR}/qteditorfactory.cpp
             ${INCLDIR}/qtvariantproperty.cpp
             ${INCLDIR}/qttreepropertybrowser.cpp
             ${INCLDIR}/qtbuttonpropertybrowser.cpp
             ${INCLDIR}/qtgroupboxpropertybrowser.cpp
             ${INCLDIR}/qtpropertybrowserutils.cpp
 ) 

set (qt_headers ${INCLDIR}/qtpropertybrowser.h 
                ${INCLDIR}/qtpropertymanager.h
                ${INCLDIR}/qteditorfactory.h
                ${INCLDIR}/qtvariantproperty.h
                ${INCLDIR}/qttreepropertybrowser.h
                ${INCLDIR}/qtbuttonpropertybrowser.h
                ${INCLDIR}/qtgroupboxpropertybrowser.h
                ${INCLDIR}/qtpropertybrowserutils_p.h
  )

set (resources ${INCLDIR}/qtpropertybrowser.qrc)
QT4_WRAP_CPP(headers_MOC ${qt_headers})
QT4_ADD_RESOURCES(resources_RCC ${resources})

add_library(qtpropertybrowser SHARED ${sources} ${headers_MOC} ${resources_RCC})
target_link_libraries(${PROJ_NAME} ${_dep_libs})

installTarget(${PROJ_NAME} "lib" "LIBRARIES" "bin" "BINARIES")