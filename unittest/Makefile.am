EXTRA_DIST = \
	Config.h \
	report.xsl
	
check_PROGRAMS =

AM_LDFLAGS="-L../bin"

AM_CPPFLAGS="-I$(srcdir)/../src"

#########################################################################
# dataaccess test

if TE_BUILD_GDAL_BASED_TEST
if TE_BUILD_POSTGIS_BASED_TEST
check_PROGRAMS += ../bin/terralib_unittest_dataaccess

___bin_terralib_unittest_dataaccess_LDADD = \
	-lterralib_common \
	-lterralib_geometry \
	-lterralib_dataaccess \
	-lterralib_datatype \
	-lterralib_raster \
	-lterralib_srs \
	-lterralib_memory \
	-lterralib_plugin \
	-lterralib_gdal \
	-lterralib_postgis
	
___bin_terralib_unittest_dataaccess_SOURCES = \
	dataaccess/TsDataSetType.h \
	dataaccess/TsDataSourceTransactor.h \
	dataaccess/TsDataSourceCatalogLoader.h \
	dataaccess/TsDataSourceCatalog.h \
	dataaccess/Config.h \
	dataaccess/TsDataSetTypePersistence.h \
	dataaccess/TsDataSource.h \
	dataaccess/TsDataSourceCatalog.cpp \
	dataaccess/TsDataSource.cpp \
	dataaccess/TsDataSourceCatalogLoader.cpp \
	dataaccess/TsDataSourceTransactor.cpp \
	dataaccess/TsDataSetType.cpp \
	dataaccess/TsDataSetTypePersistence.cpp \
	dataaccess/TsPropertyTypes.cpp \
	dataaccess/TsPropertyTypes.h \
	postgis/TsManagerPostGIS.h \
	postgis/TsPostGISTransactor.h \
	postgis/TsDataSetTypePersistencePostGIS.h \
	postgis/TsPostGIS.h \
	postgis/TsPostGISTransactor.cpp \
	postgis/TsPostGIS.cpp \
	postgis/TsManagerPostGIS.cpp \
	postgis/TsDataSetTypePersistencePostGIS.cpp \
	postgis/TsPostGISCatalogLoader.cpp \
	postgis/TsPostGISCatalogLoader.h \
	gdal/TsGdalDataSource.h \
	gdal/TsGdalTransactor.h \
	gdal/TsManagerGdal.h \
	gdal/TsGdalDataSource.cpp \
	gdal/TsGdalTransactor.cpp \
	gdal/TsManagerGdal.cpp \
	dataaccess/main.cpp
	
endif
endif	
#########################################################################
# datatype test

check_PROGRAMS += ../bin/terralib_unittest_datatype

___bin_terralib_unittest_datatype_LDADD = \
	-lterralib_common \
	-lterralib_geometry \
	-lterralib_dataaccess \
	-lterralib_datatype \
	-lterralib_raster \
	-lterralib_srs \
	-lterralib_memory \
	-lterralib_plugin

___bin_terralib_unittest_datatype_SOURCES = \
	datatype/TsByteArray.h \
	datatype/TsByteArray.cpp \
	datatype/main.cpp

#	datatype/TsDateTime.cpp
#	datatype/TsDateTime.h
	
#########################################################################
# geometry test	

check_PROGRAMS += ../bin/terralib_unittest_geometry

___bin_terralib_unittest_geometry_LDADD = \
	-lterralib_common \
	-lterralib_geometry \
	-lterralib_dataaccess \
	-lterralib_datatype \
	-lterralib_srs
	
___bin_terralib_unittest_geometry_SOURCES = \
	geometry/TsEnvelop.h \
	geometry/TsEnvelop.cpp \
	geometry/TsGeometry.h \
	geometry/TsGeometry.cpp \
	geometry/TsWKBReader.h \
	geometry/TsWKBReader.cpp \
	geometry/main.cpp
	
#########################################################################
# srs test	

check_PROGRAMS += ../bin/terralib_unittest_srs

___bin_terralib_unittest_srs_LDADD = \
	-lterralib_common \
	-lterralib_geometry \
	-lterralib_dataaccess \
	-lterralib_datatype \
	-lterralib_srs

___bin_terralib_unittest_srs_SOURCES = \
	srs/main.cpp \
	srs/TsSrs.cpp \
	srs/TsSrs.h

#########################################################################
# rp (raster processing) tests

EXTRA_DIST += rp/LoadModules.h

if TE_BUILD_OGR_BASED_TEST
if TE_BUILD_GDAL_BASED_TEST
RP_TESTS_LDADD = \
	-lterralib_common \
	-lterralib_geometry \
	-lterralib_dataaccess \
	-lterralib_datatype \
	-lterralib_plugin \
	-lterralib_raster \
	-lterralib_srs \
	-lterralib_memory \
	-lterralib_rp
	
check_PROGRAMS += ../bin/terralib_unittest_rp_Matrix
___bin_terralib_unittest_rp_Matrix_LDADD = $(RP_TESTS_LDADD)
___bin_terralib_unittest_rp_Matrix_SOURCES = \
	rp/TsMatrix.h \
	rp/TsMatrix.cpp \
	rp/TsMatrixMain.cpp	
	
check_PROGRAMS += ../bin/terralib_unittest_rp_Contrast
___bin_terralib_unittest_rp_Contrast_LDADD = $(RP_TESTS_LDADD)
___bin_terralib_unittest_rp_Contrast_SOURCES = \
	rp/TsContrast.h \
	rp/TsContrast.cpp \
	rp/TsContrastMain.cpp
	
check_PROGRAMS += ../bin/terralib_unittest_rp_Segmenter
___bin_terralib_unittest_rp_Segmenter_LDADD = $(RP_TESTS_LDADD)
___bin_terralib_unittest_rp_Segmenter_SOURCES = \
	rp/TsSegmenter.h \
	rp/TsSegmenter.cpp \
	rp/TsSegmenterMain.cpp
		
endif
endif

#########################################################################
# qtwidgets tests

if TE_BUILD_QT_BASED_TEST
if TE_BUILD_POSTGIS_BASED_TEST

QTWIDGETS_TESTS_LDADD = \
	-lterralib_common \
	-lterralib_geometry \
	-lterralib_dataaccess \
	-lterralib_datatype \
	-lterralib_plugin \
	-lterralib_raster \
	-lterralib_srs \
	-lterralib_memory \
	-lterralib_qtwidgets \
	-lterralib_postgis \
	-lterralib_annotationtext \
	-lterralib_maptools \
	-lterralib_gml \
	-lterralib_filter \
	-lterralib_xlink
	
check_PROGRAMS += ../bin/terralib_unittest_qtwidgets
___bin_terralib_unittest_qtwidgets_LDADD = $(QTWIDGETS_TESTS_LDADD)
___bin_terralib_unittest_qtwidgets_SOURCES = \
	qtwidgets/TsCanvas.h \
	qtwidgets/TsLayerExplorer.h \
	qtwidgets/TsMapDisplay.h \
	qtwidgets/main.cpp \
	qtwidgets/TsCanvas.cpp \
	qtwidgets/TsLayerExplorer.cpp \
	qtwidgets/TsMapDisplay.cpp

endif
endif
	
#########################################################################
# Adding all build binaries to test queue
	
TESTS = $(check_PROGRAMS)	
