if TE_BUILD_QT_BASED_EXAMPLE
include ../m4/tlqtextras.am
endif

EXTRA_DIST = \
	Config.h
	
lib_LTLIBRARIES =	

bin_PROGRAMS =

AM_LDFLAGS="-L../bin"

AM_CPPFLAGS="-I$(srcdir)/../src"

#########################################################################
# cppplugin example

if TE_BUILD_EXAMPLE
lib_LTLIBRARIES += ../bin/libterralib_examples_cppplugin.la
endif

___bin_libterralib_examples_cppplugin_la_LIBADD = \
	-lterralib_common \
	-lterralib_plugin
	
___bin_libterralib_examples_cppplugin_la_SOURCES = \
	cppplugin/helloworld/HelloWorld.h \
	cppplugin/helloworld/HelloWorld.cpp

#########################################################################
# dataaccess example

if TE_BUILD_EXAMPLE
bin_PROGRAMS += ../bin/terralib_examples_dataaccess
endif

___bin_terralib_examples_dataaccess_LDADD = \
	-lterralib_common \
	-lterralib_geometry \
	-lterralib_dataaccess \
	-lterralib_datatype \
	-lterralib_raster \
	-lterralib_srs \
	-lterralib_memory \
	-lterralib_plugin

___bin_terralib_examples_dataaccess_SOURCES = \
	dataaccess/DataAccessExamples.h \
	dataaccess/Config.h \
	dataaccess/OGRExample.cpp \
	dataaccess/MemoryExample.cpp \
	dataaccess/main.cpp \
	dataaccess/PersistenceExample.cpp \
	dataaccess/AddingDataSetPropertyValues.cpp \
	dataaccess/AddingProperty.cpp \
	dataaccess/LoadModules.cpp \
	dataaccess/PrintDataSets.cpp \
	dataaccess/PrintCatalog.cpp \
	dataaccess/RetrieveUsingSpatialFilter.cpp \
	dataaccess/AddingSpatialIndex.cpp \
	dataaccess/PrintDataSet.cpp \
	dataaccess/QueryExample.cpp \
	dataaccess/ProcessingFeatureData.cpp \
	dataaccess/AddingPrimaryKey.cpp \
	dataaccess/CopyingDataSetToDataSource.cpp \
	dataaccess/AddingUniqueKey.cpp \
	dataaccess/PostGISExample.cpp \
	dataaccess/DataSetGetExtent.cpp \
	dataaccess/GDALExample.cpp \
	dataaccess/CreatingDataSetInMemory.cpp \
	dataaccess/DroppingDataSource.cpp \
	dataaccess/CreatingDataSetType.cpp \
	dataaccess/CreatingEmptyDataSource.cpp \
	dataaccess/CopyingData.cpp \
	dataaccess/DroppingDataSetType.cpp \
	dataaccess/DroppingDataSetTypeProperty.cpp 

#########################################################################
# factory example

if TE_BUILD_EXAMPLE
bin_PROGRAMS += ../bin/terralib_examples_factory
endif

___bin_terralib_examples_factory_LDADD = \
	-lterralib_common
	
___bin_terralib_examples_factory_SOURCES = \
	factory/Animals.h \
	factory/AnimalsFactory.h \
	factory/main.cpp \
	factory/AnimalsFactory.cpp \
	factory/Animals.cpp
	
#########################################################################
# geometry example	

if TE_BUILD_EXAMPLE
bin_PROGRAMS += ../bin/terralib_examples_geometry
endif

___bin_terralib_examples_geometry_LDADD = \
	-lterralib_common \
	-lterralib_datatype \
	-lterralib_geometry \
	-lterralib_srs
	
___bin_terralib_examples_geometry_SOURCES = \
	geometry/GeometryExamples.h \
	geometry/SetOperations.cpp \
	geometry/main.cpp \
	geometry/MeasurementOperations.cpp \
	geometry/SpatialRelations.cpp \
	geometry/GeometryConstructors.cpp \
	geometry/Wkt.cpp
	
########################################################################
# sam example	

if TE_BUILD_EXAMPLE
bin_PROGRAMS += ../bin/terralib_examples_sam
endif

___bin_terralib_examples_sam_LDADD = \
	-lterralib_common \
	-lterralib_datatype \
	-lterralib_geometry \
	-lterralib_srs
	
___bin_terralib_examples_sam_SOURCES = \
	sam/SAMExamples.h \
	sam/main.cpp \
	sam/RTreeExamples.cpp \
	sam/LoadModules.cpp
#########################################################################
# raster example	

if TE_BUILD_GDAL_BASED_EXAMPLE
if TE_BUILD_OGR_BASED_EXAMPLE
bin_PROGRAMS += ../bin/terralib_examples_raster
endif
endif

___bin_terralib_examples_raster_LDADD = \
	-lterralib_common \
	-lterralib_geometry \
	-lterralib_dataaccess \
	-lterralib_datatype \
	-lterralib_plugin \
	-lterralib_raster \
	-lterralib_srs \
	-lterralib_memory \
	-lterralib_gdal \
	-lterralib_ogr
	
___bin_terralib_examples_raster_SOURCES = \
	raster/RasterExamples.h \
	raster/RasterizePolygonSet.cpp \
	raster/main.cpp \
	raster/SplitComplexImage.cpp \
	raster/LoadModules.cpp  \
	raster/ArithmeticWithRaster.cpp \
	raster/MakeRasterBlock.cpp \
	raster/ReprojectRaster.cpp \
	raster/ExemplifyDataacces.cpp \
	raster/ExemplifyIterators.cpp \
	raster/ResampleRaster.cpp \
	raster/GribExample.cpp \
	raster/RasterInMemory.cpp \
	raster/GribPolygonExample.cpp
	
#########################################################################
# colorbar example	

if TE_BUILD_QT_BASED_EXAMPLE
 bin_PROGRAMS += ../bin/terralib_examples_colorbar
endif

___bin_terralib_examples_colorbar_LDADD = \
	-lterralib_common \
	-lterralib_geometry \
	-lterralib_dataaccess \
	-lterralib_datatype \
	-lterralib_plugin \
	-lterralib_srs \
	-lterralib_color

___bin_terralib_examples_colorbar_SOURCES = \
	colorbar/ColorBar.h \
	colorbar/ColorBar.cpp \
	colorbar/main.cpp
		
#########################################################################
# st example	

bin_PROGRAMS += ../bin/terralib_examples_st

___bin_terralib_examples_st_LDADD = \
	-lterralib_common \
	-lterralib_geometry \
	-lterralib_dataaccess \
	-lterralib_datatype \
	-lterralib_plugin \
	-lterralib_raster \
	-lterralib_srs \
	-lterralib_memory \
	-lterralib_gdal \
	-lterralib_ogr \
	-lterralib_st \
	-lterralib_stloader
	
___bin_terralib_examples_st_SOURCES = \
	st/Config.h \
	st/STExamples.h \
	st/CoverageExamples.cpp \
	st/LoadModules.cpp \
	st/main.cpp \
	st/MovingObjectExamples.cpp \
	st/MovingObjectsFromKMLAndMetadata.cpp \
	st/MovingObjectsFromPostGIS.cpp \
	st/PrintCatalog.cpp \
	st/PrintDataSet.cpp \
	st/PrintDataSets.cpp \
	st/PrintDataSetType.cpp \
	st/PrintMovingObjectDifference.cpp \
	st/PrintMovingObjectDistance.cpp \
	st/PrintMovingObjectInfo.cpp \
	st/PrintMovingObjectIntersection.cpp \
	st/PrintMovingObjects.cpp \
	st/PrintTimeSeries.cpp \
	st/PrintTrajectories.cpp

