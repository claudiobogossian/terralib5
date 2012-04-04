if TE_BUILD_QT_MODULE
include ../m4/tlqtextras.am
endif

nobase_include_HEADERS = \
	terralib.h \
	terralib/linuxplatformdefines.h \
	terralib/Config.h
	
nodist_nobase_include_HEADERS =	

lib_LTLIBRARIES =

BUILT_SOURCES =

EXTRA_DIST =

CLEANFILES =

#########################################################################
# common module

lib_LTLIBRARIES += ../bin/libterralib_common.la

nobase_include_HEADERS += \
	terralib/common.h \
	terralib/common_fw.h \
	terralib/common/AbstractFactory.h \
	terralib/common/BaseVisitable.h \
	terralib/common/BaseVisitor.h \
	terralib/common/ByteSwapUtils.h \
	terralib/common/Comparators.h \
	terralib/common/Config.h \
	terralib/common/Distance.h \
	terralib/common/Enums.h \
	terralib/common/Exception.h \
	terralib/common/FactoryDictionary.h \
	terralib/common/Globals.h \
	terralib/common/HexUtils.h \
	terralib/common/Holder.h \
	terralib/common/Library.h \
	terralib/common/LibraryManager.h \
	terralib/common/LinuxUtils.h \
	terralib/common/LoggedException.h \
	terralib/common/Logger.h \
	terralib/common/Module.h \
	terralib/common/ModuleUtils.h \
	terralib/common/ParameterizedAbstractFactory.h \
	terralib/common/PlatformUtils.h \
	terralib/common/Singleton.h \
	terralib/common/Static.h \
	terralib/common/STLUtils.h \
	terralib/common/StringUtils.h \
	terralib/common/TerraLib.h \
	terralib/common/ThreadingPolicies.h \
	terralib/common/Translator.h \
	terralib/common/TreeItem.h \
	terralib/common/UnitOfMeasure.h \
	terralib/common/UnitsOfMeasureManager.h \
	terralib/common/URI/authority.h \
	terralib/common/URI/domain_name.h \
	terralib/common/URI/fragment.h \
	terralib/common/URI/ip_address.h \
	terralib/common/URI/ipv6_address.h \
	terralib/common/URI/path.h \
	terralib/common/URI/query.h \
	terralib/common/URI/scheme.h \
	terralib/common/URI/uri.h \
	terralib/common/URI/urisyn.h \
	terralib/common/URI/utils.h \
	terralib/common/URL.h \
	terralib/common/Version.h \
	terralib/common/Visitor.h \
	terralib/common/WinUtils.h        

___bin_libterralib_common_la_SOURCES = \
	terralib/common/Distance.cpp \
	terralib/common/Exception.cpp \
	terralib/common/Globals.cpp \
	terralib/common/Library.cpp \
	terralib/common/LibraryManager.cpp \
	terralib/common/LinuxUtils.cpp \
	terralib/common/LoggedException.cpp \
	terralib/common/Logger.cpp \
	terralib/common/Module.cpp \
	terralib/common/PlatformUtils.cpp \
	terralib/common/TerraLib.cpp \
	terralib/common/Translator.cpp \
	terralib/common/UnitOfMeasure.cpp \
	terralib/common/UnitsOfMeasureManager.cpp \
	terralib/common/URI/authority.cpp \
	terralib/common/URI/domain_name.cpp \
	terralib/common/URI/fragment.cpp \
	terralib/common/URI/ip_address.cpp \
	terralib/common/URI/ipv6_address.cpp \
	terralib/common/URI/path.cpp \
	terralib/common/URI/query.cpp \
	terralib/common/URI/scheme.cpp \
	terralib/common/URI/uri.cpp \
	terralib/common/URI/urisyn.cpp \
	terralib/common/URI/utils.cpp \
	terralib/common/URL.cpp \
	terralib/common/Version.cpp \
	terralib/common/WinUtils.cpp
	
#########################################################################
# color module

lib_LTLIBRARIES += ../bin/libterralib_color.la

___bin_libterralib_color_la_LIBADD = \
	../bin/libterralib_common.la

nobase_include_HEADERS += \
	terralib/color.h \
	terralib/color_fw.h \
	terralib/color/ColorBar.h \
	terralib/color/ColorSchemeCatalog.h \
	terralib/color/ColorSchemeCatalogManager.h \
	terralib/color/ColorSchemeGroup.h \
	terralib/color/ColorScheme.h \
	terralib/color/ColorTransform.h \
	terralib/color/Config.h \
	terralib/color/Module.h \
	terralib/color/RGBAColor.h

___bin_libterralib_color_la_SOURCES = \
	terralib/color/ColorBar.cpp \
	terralib/color/ColorSchemeCatalog.cpp \
	terralib/color/ColorSchemeCatalogManager.cpp \
	terralib/color/ColorScheme.cpp \
	terralib/color/ColorSchemeGroup.cpp \
	terralib/color/ColorTransform.cpp \
	terralib/color/Module.cpp

#########################################################################
# xml module

lib_LTLIBRARIES += ../bin/libterralib_xml.la

___bin_libterralib_xml_la_LIBADD = \
	../bin/libterralib_common.la

nobase_include_HEADERS += \
	terralib/xml/Config.h \
	terralib/xml/Enums.h \
	terralib/xml/Exception.h \
	terralib/xml/Module.h \
	terralib/xml/ReaderFactory.h \
	terralib/xml/Reader.h \
	terralib/xml/Writer.h

___bin_libterralib_xml_la_SOURCES = \
	terralib/xml/Exception.cpp \
	terralib/xml/Module.cpp \
	terralib/xml/Reader.cpp \
	terralib/xml/ReaderFactory.cpp \
	terralib/xml/Writer.cpp
	
#########################################################################
# datatype module

lib_LTLIBRARIES += ../bin/libterralib_datatype.la

___bin_libterralib_datatype_la_LIBADD = \
	../bin/libterralib_common.la

nobase_include_HEADERS += \
	terralib/datatype.h \
	terralib/datatype_fw.h \
	terralib/datatype/AbstractDataConverter.h \
	terralib/datatype/AbstractData.h \
	terralib/datatype/Array.h \
	terralib/datatype/ArrayProperty.h \
	terralib/datatype/ByteArray.h \
	terralib/datatype/CompositeData.h \
	terralib/datatype/CompositeProperty.h \
	terralib/datatype/Config.h \
	terralib/datatype/DataConverterManager.h \
	terralib/datatype/DataType.h \
	terralib/datatype/DataTypeManager.h \
	terralib/datatype/DateDuration.h \
	terralib/datatype/Date.h \
	terralib/datatype/DatePeriod.h \
	terralib/datatype/DateTime.h \
	terralib/datatype/DateTimeInstant.h \
	terralib/datatype/DateTimePeriod.h \
	terralib/datatype/DateTimeProperty.h \
	terralib/datatype/DateTimeUtils.h \
	terralib/datatype/Enums.h \
	terralib/datatype/Exception.h \
	terralib/datatype/Int32ToStringConverter.h \
	terralib/datatype/Module.h \
	terralib/datatype/NumericProperty.h \
	terralib/datatype/OrdinalInstant.h \
	terralib/datatype/OrdinalPeriod.h \
	terralib/datatype/Property.h \
	terralib/datatype/SimpleData.h \
	terralib/datatype/SimpleProperty.h \
	terralib/datatype/StringProperty.h \
	terralib/datatype/TimeDuration.h \
	terralib/datatype/TimeInstant.h \
	terralib/datatype/TimeInstantTZ.h \
	terralib/datatype/TimePeriod.h \
	terralib/datatype/TimePeriodTZ.h \
	terralib/datatype/Utils.h

___bin_libterralib_datatype_la_SOURCES = \
	terralib/datatype/AbstractDataConverter.cpp \
	terralib/datatype/AbstractData.cpp \
	terralib/datatype/Array.cpp \
	terralib/datatype/ArrayProperty.cpp \
	terralib/datatype/ByteArray.cpp \
	terralib/datatype/CompositeData.cpp \
	terralib/datatype/CompositeProperty.cpp \
	terralib/datatype/DataConverterManager.cpp \
	terralib/datatype/DataType.cpp \
	terralib/datatype/DataTypeManager.cpp \
	terralib/datatype/Date.cpp \
	terralib/datatype/DateDuration.cpp \
	terralib/datatype/DatePeriod.cpp \
	terralib/datatype/DateTime.cpp \
	terralib/datatype/DateTimeInstant.cpp \
	terralib/datatype/DateTimePeriod.cpp \
	terralib/datatype/DateTimeProperty.cpp \
	terralib/datatype/DateTimeUtils.cpp \
	terralib/datatype/Exception.cpp \
	terralib/datatype/Int32ToStringConverter.cpp \
	terralib/datatype/Module.cpp \
	terralib/datatype/NumericProperty.cpp \
	terralib/datatype/OrdinalInstant.cpp \
	terralib/datatype/OrdinalPeriod.cpp \
	terralib/datatype/Property.cpp \
	terralib/datatype/SimpleData.cpp \
	terralib/datatype/SimpleProperty.cpp \
	terralib/datatype/StringProperty.cpp \
	terralib/datatype/TimeDuration.cpp \
	terralib/datatype/TimeInstant.cpp \
	terralib/datatype/TimeInstantTZ.cpp \
	terralib/datatype/TimePeriod.cpp \
	terralib/datatype/TimePeriodTZ.cpp \
	terralib/datatype/Utils.cpp

#########################################################################
# plugin module

lib_LTLIBRARIES += ../bin/libterralib_plugin.la

___bin_libterralib_plugin_la_LIBADD = \
	../bin/libterralib_common.la

nobase_include_HEADERS += \
	terralib/plugin.h \
	terralib/plugin/Config.h \
	terralib/plugin/CppPlugin.h \
	terralib/plugin/CppPluginProxyFactory.h \
	terralib/plugin/CppPluginProxy.h \
	terralib/plugin/Module.h \
	terralib/plugin/PluginFactory.h \
	terralib/plugin/Plugin.h \
	terralib/plugin/PluginInfo.h \
	terralib/plugin/PluginManager.h
		
___bin_libterralib_plugin_la_SOURCES = \
	terralib/plugin/CppPlugin.cpp \
	terralib/plugin/CppPluginProxy.cpp \
	terralib/plugin/CppPluginProxyFactory.cpp \
	terralib/plugin/Module.cpp \
	terralib/plugin/Plugin.cpp \
	terralib/plugin/PluginFactory.cpp \
	terralib/plugin/PluginManager.cpp
	
#########################################################################
# srs module	
	
lib_LTLIBRARIES += ../bin/libterralib_srs.la

___bin_libterralib_srs_la_LIBADD = \
	../bin/libterralib_common.la

nobase_include_HEADERS += \
	terralib/srs.h \
	terralib/srs_fw.h \
	terralib/srs/Config.h \
	terralib/srs/Converter.h \
	terralib/srs/CoordinateSystemEPSGFactory.h \
	terralib/srs/CoordinateSystemFactory.h \
	terralib/srs/CoordinateSystem.h \
	terralib/srs/CoordinateSystemProj4Factory.h \
	terralib/srs/CoordinateSystemWKTFactory.h \
	terralib/srs/Datum.h \
	terralib/srs/Ellipsoid.h \
	terralib/srs/EPSGDefinitions.h \
	terralib/srs/Exception.h \
	terralib/srs/GeographicCoordinateSystem.h \
	terralib/srs/Module.h \
	terralib/srs/Proj4Definitions.h \
	terralib/srs/ProjectedCoordinateSystem.h \
	terralib/srs/WKTActions.h \
	terralib/srs/WKTParser.h \
	terralib/srs/WKTReader.h
	
___bin_libterralib_srs_la_SOURCES = \
	terralib/srs/Converter.cpp \
	terralib/srs/CoordinateSystem.cpp \
	terralib/srs/CoordinateSystemEPSGFactory.cpp \
	terralib/srs/CoordinateSystemFactory.cpp \
	terralib/srs/CoordinateSystemProj4Factory.cpp \
	terralib/srs/CoordinateSystemWKTFactory.cpp \
	terralib/srs/Datum.cpp \
	terralib/srs/Ellipsoid.cpp \
	terralib/srs/Exception.cpp \
	terralib/srs/GeographicCoordinateSystem.cpp \
	terralib/srs/Module.cpp \
	terralib/srs/ProjectedCoordinateSystem.cpp \
	terralib/srs/WKTActions.cpp \
	terralib/srs/WKTReader.cpp		
	
#########################################################################
# geometry module		
	
lib_LTLIBRARIES += ../bin/libterralib_geometry.la

___bin_libterralib_geometry_la_LIBADD = \
	../bin/libterralib_common.la \
	../bin/libterralib_datatype.la \
	../bin/libterralib_srs.la

nobase_include_HEADERS += \
	terralib/geometry.h \
	terralib/geometry_fw.h \
	terralib/geometry/AbstractPoint.h \
	terralib/geometry/CircularString.h \
	terralib/geometry/CompoundCurve.h \
	terralib/geometry/Config.h \
	terralib/geometry/Coord2D.h \
	terralib/geometry/Curve.h \
	terralib/geometry/CurvePolygon.h \
	terralib/geometry/Enums.h \
	terralib/geometry/Envelope.h \
	terralib/geometry/Exception.h \
	terralib/geometry/GeometryCollection.h \
	terralib/geometry/GeometryFactory.h \
	terralib/geometry/Geometry.h \
	terralib/geometry/GeometryProperty.h \
	terralib/geometry/GEOSGeometryFactory.h \
	terralib/geometry/GEOSReader.h \
	terralib/geometry/GEOSWriter.h \
	terralib/geometry/LinearRing.h \
	terralib/geometry/Line.h \
	terralib/geometry/LineString.h \
	terralib/geometry/Module.h \
	terralib/geometry/MultiCurve.h \
	terralib/geometry/MultiLineString.h \
	terralib/geometry/MultiPoint.h \
	terralib/geometry/MultiPolygon.h \
	terralib/geometry/MultiSurface.h \
	terralib/geometry/Point.h \
	terralib/geometry/PointKd.h \
	terralib/geometry/PointM.h \
	terralib/geometry/PointZ.h \
	terralib/geometry/PointZM.h \
	terralib/geometry/Polygon.h \
	terralib/geometry/PolyhedralSurface.h \
	terralib/geometry/Surface.h \
	terralib/geometry/TIN.h \
	terralib/geometry/Triangle.h \
	terralib/geometry/Utils.h \
	terralib/geometry/Visitor.h \
	terralib/geometry/WKBReader.h \
	terralib/geometry/WKBSize.h \
	terralib/geometry/WKBWriter.h \
	terralib/geometry/WKTActions.h \
	terralib/geometry/WKTParser.h \
	terralib/geometry/WKTReader.h \
	terralib/geometry/WKTWriter.h
		
___bin_libterralib_geometry_la_SOURCES = \
	terralib/geometry/AbstractPoint.cpp \
	terralib/geometry/CircularString.cpp \
	terralib/geometry/CompoundCurve.cpp \
	terralib/geometry/Curve.cpp \
	terralib/geometry/CurvePolygon.cpp \
	terralib/geometry/Envelope.cpp \
	terralib/geometry/Exception.cpp \
	terralib/geometry/GeometryCollection.cpp \
	terralib/geometry/Geometry.cpp \
	terralib/geometry/GeometryFactory.cpp \
	terralib/geometry/GeometryProperty.cpp \
	terralib/geometry/GEOSGeometryFactory.cpp \
	terralib/geometry/GEOSReader.cpp \
	terralib/geometry/GEOSWriter.cpp \
	terralib/geometry/LinearRing.cpp \
	terralib/geometry/Line.cpp \
	terralib/geometry/LineString.cpp \
	terralib/geometry/Module.cpp \
	terralib/geometry/MultiCurve.cpp \
	terralib/geometry/MultiLineString.cpp \
	terralib/geometry/MultiPoint.cpp \
	terralib/geometry/MultiPolygon.cpp \
	terralib/geometry/MultiSurface.cpp \
	terralib/geometry/Point.cpp \
	terralib/geometry/PointKd.cpp \
	terralib/geometry/PointM.cpp \
	terralib/geometry/PointZ.cpp \
	terralib/geometry/PointZM.cpp \
	terralib/geometry/Polygon.cpp \
	terralib/geometry/PolyhedralSurface.cpp \
	terralib/geometry/Surface.cpp \
	terralib/geometry/TIN.cpp \
	terralib/geometry/Triangle.cpp \
	terralib/geometry/Utils.cpp \
	terralib/geometry/Visitor.cpp \
	terralib/geometry/WKBReader.cpp \
	terralib/geometry/WKBSize.cpp \
	terralib/geometry/WKBWriter.cpp \
	terralib/geometry/WKTActions.cpp \
	terralib/geometry/WKTReader.cpp \
	terralib/geometry/WKTWriter.cpp

#########################################################################
# Annotation text module	

lib_LTLIBRARIES += ../bin/libterralib_annotationtext.la

___bin_libterralib_annotationtext_la_LIBADD = \
	../bin/libterralib_common.la \
	../bin/libterralib_datatype.la \
	../bin/libterralib_geometry.la \
	../bin/libterralib_srs.la

nobase_include_HEADERS += \
	terralib/annotationtext.h \
	terralib/annotationtext_fw.h \
	terralib/annotationtext/Attributes.h \
	terralib/annotationtext/Config.h \
	terralib/annotationtext/Element.h \
	terralib/annotationtext/Enums.h \
	terralib/annotationtext/Module.h \
	terralib/annotationtext/Text.h
		
___bin_libterralib_annotationtext_la_SOURCES = \
	terralib/annotationtext/Attributes.cpp \
	terralib/annotationtext/Element.cpp \
	terralib/annotationtext/Module.cpp \
	terralib/annotationtext/Text.cpp

#########################################################################
# raster module	

lib_LTLIBRARIES += ../bin/libterralib_raster.la

___bin_libterralib_raster_la_LIBADD = \
	../bin/libterralib_common.la \
	../bin/libterralib_datatype.la \
	../bin/libterralib_geometry.la \
	../bin/libterralib_srs.la

nobase_include_HEADERS += \
	terralib/raster.h \
	terralib/raster_fw.h \
	terralib/raster/Band.h \
	terralib/raster/BandIterator.h \
	terralib/raster/BandIteratorWindow.h \
	terralib/raster/BandProperty.h \
	terralib/raster/BandSummary.h \
	terralib/raster/BlockUtils.h \
	terralib/raster/Config.h \
	terralib/raster/Enums.h \
	terralib/raster/Exception.h \
	terralib/raster/Grid.h \
	terralib/raster/Interpolator.h \
	terralib/raster/Module.h \
	terralib/raster/RasterFactory.h \
	terralib/raster/Raster.h \
	terralib/raster/RasterIterator.h \
	terralib/raster/RasterProperty.h \
	terralib/raster/RasterSummary.h \
	terralib/raster/RasterSummaryManager.h \
	terralib/raster/Reprojection.h \
	terralib/raster/Utils.h


___bin_libterralib_raster_la_SOURCES = \
	terralib/raster/Band.cpp \
	terralib/raster/BandProperty.cpp \
	terralib/raster/BandSummary.cpp \
	terralib/raster/BlockUtils.cpp \
	terralib/raster/Exception.cpp \
	terralib/raster/Grid.cpp \
	terralib/raster/Interpolator.cpp \
	terralib/raster/Module.cpp \
	terralib/raster/Raster.cpp \
	terralib/raster/RasterFactory.cpp \
	terralib/raster/RasterProperty.cpp \
	terralib/raster/RasterSummaryManager.cpp \
	terralib/raster/Reprojection.cpp \
	terralib/raster/Utils.cpp
	
#########################################################################
# sam module		
	
nobase_include_HEADERS += \
	terralib/sam.h \
	terralib/sam/rtree.h \
	terralib/sam/rtree/Index.h \
	terralib/sam/rtree/Branch.h \
	terralib/sam/rtree/Node.h \
	terralib/sam/rtree/PartitionVars.h
  	
#########################################################################
# dataaccess module	  	

lib_LTLIBRARIES += ../bin/libterralib_dataaccess.la

___bin_libterralib_dataaccess_la_LIBADD = \
	../bin/libterralib_common.la \
	../bin/libterralib_datatype.la \
	../bin/libterralib_geometry.la \
	../bin/libterralib_srs.la

nobase_include_HEADERS += \
	terralib/dataaccess.h \
	terralib/dataaccess_fw.h \
	terralib/dataaccess/Config.h \
	terralib/dataaccess/dataset/CheckConstraint.h \
	terralib/dataaccess/dataset/Constraint.h \
	terralib/dataaccess/dataset/DataSet.h \
	terralib/dataaccess/dataset/DataSetItem.h \
	terralib/dataaccess/dataset/DataSetPersistence.h \
	terralib/dataaccess/dataset/DataSetType.h \
	terralib/dataaccess/dataset/DataSetTypePersistence.h \
	terralib/dataaccess/dataset/ForeignKey.h \
	terralib/dataaccess/dataset_fw.h \
	terralib/dataaccess/dataset_h.h \
	terralib/dataaccess/dataset/Index.h \
	terralib/dataaccess/dataset/PrimaryKey.h \
	terralib/dataaccess/dataset/Sequence.h \
	terralib/dataaccess/dataset/UniqueKey.h \
	terralib/dataaccess/datasource/AbstractConnectionPool.h \
	terralib/dataaccess/datasource/BatchExecutor.h \
	terralib/dataaccess/datasource/ConnectionPoolManager.h \
	terralib/dataaccess/datasource/DataSourceCatalog.h \
	terralib/dataaccess/datasource/DataSourceCatalogLoader.h \
	terralib/dataaccess/datasource/DataSourceFactory.h \
	terralib/dataaccess/datasource/DataSource.h \
	terralib/dataaccess/datasource/DataSourceManager.h \
	terralib/dataaccess/datasource/DataSourceTransactor.h \
	terralib/dataaccess/datasource_fw.h \
	terralib/dataaccess/datasource_h.h \
	terralib/dataaccess/datasource/PreparedQuery.h \
	terralib/dataaccess/datasource/ScopedTransaction.h \
	terralib/dataaccess/Enums.h \
	terralib/dataaccess/Exception.h \
	terralib/dataaccess/iterator_fw.h \
	terralib/dataaccess/iterator_h.h \
	terralib/dataaccess/Module.h \
	terralib/dataaccess/query/Add.h \
	terralib/dataaccess/query/And.h \
	terralib/dataaccess/query/BinaryFunction.h \
	terralib/dataaccess/query/BinaryOpEncoder.h \
	terralib/dataaccess/query/BinaryOp.h \
	terralib/dataaccess/query/DataSetName.h \
	terralib/dataaccess/query/Distinct.h \
	terralib/dataaccess/query/Div.h \
	terralib/dataaccess/query/EqualTo.h \
	terralib/dataaccess/query/Expression.h \
	terralib/dataaccess/query/Field.h \
	terralib/dataaccess/query/Fields.h \
	terralib/dataaccess/query/FromFunctionCall.h \
	terralib/dataaccess/query/From.h \
	terralib/dataaccess/query/FromItem.h \
	terralib/dataaccess/query/FunctionCatalog.h \
	terralib/dataaccess/query/FunctionCatalogManager.h \
	terralib/dataaccess/query/FunctionDefn.h \
	terralib/dataaccess/query/FunctionEncoder.h \
	terralib/dataaccess/query/Function.h \
	terralib/dataaccess/query/FunctionNames.h \
	terralib/dataaccess/query/FunctionNoArgsEncoder.h \
	terralib/dataaccess/query/FunctionParameter.h \
	terralib/dataaccess/query_fw.h \
	terralib/dataaccess/query/GreaterThan.h \
	terralib/dataaccess/query/GreaterThanOrEqualTo.h \
	terralib/dataaccess/query/GroupBy.h \
	terralib/dataaccess/query/GroupByItem.h \
	terralib/dataaccess/query/Having.h \
	terralib/dataaccess/query_h.h \
	terralib/dataaccess/query/In.h \
	terralib/dataaccess/query/IsNull.h \
	terralib/dataaccess/query/JoinCondition.h \
	terralib/dataaccess/query/JoinConditionOn.h \
	terralib/dataaccess/query/JoinConditionUsing.h \
	terralib/dataaccess/query/Join.h \
	terralib/dataaccess/query/LessThan.h \
	terralib/dataaccess/query/LessThanOrEqualTo.h \
	terralib/dataaccess/query/Like.h \
	terralib/dataaccess/query/LiteralByteArray.h \
	terralib/dataaccess/query/LiteralDateTime.h \
	terralib/dataaccess/query/LiteralDouble.h \
	terralib/dataaccess/query/LiteralEnvelope.h \
	terralib/dataaccess/query/LiteralGeom.h \
	terralib/dataaccess/query/Literal.h \
	terralib/dataaccess/query/LiteralInt16.h \
	terralib/dataaccess/query/LiteralInt32.h \
	terralib/dataaccess/query/LiteralInt64.h \
	terralib/dataaccess/query/LiteralString.h \
	terralib/dataaccess/query/Mul.h \
	terralib/dataaccess/query/NotEqualTo.h \
	terralib/dataaccess/query/Not.h \
	terralib/dataaccess/query/OrderBy.h \
	terralib/dataaccess/query/OrderByItem.h \
	terralib/dataaccess/query/Or.h \
	terralib/dataaccess/query/PropertyName.h \
	terralib/dataaccess/query/Query.h \
	terralib/dataaccess/query/QueryVisitor.h \
	terralib/dataaccess/query/Select.h \
	terralib/dataaccess/query/SQLDialect.h \
	terralib/dataaccess/query/SQLFunctionEncoder.h \
	terralib/dataaccess/query/SQLVisitor.h \
	terralib/dataaccess/query/ST_Beyond.h \
	terralib/dataaccess/query/ST_Contains.h \
	terralib/dataaccess/query/ST_Crosses.h \
	terralib/dataaccess/query/ST_Disjoint.h \
	terralib/dataaccess/query/ST_DistanceBuffer.h \
	terralib/dataaccess/query/ST_DWithin.h \
	terralib/dataaccess/query/ST_Equals.h \
	terralib/dataaccess/query/ST_Intersects.h \
	terralib/dataaccess/query/ST_Overlaps.h \
	terralib/dataaccess/query/ST_Relate.h \
	terralib/dataaccess/query/ST_Touches.h \
	terralib/dataaccess/query/ST_Transform.h \
	terralib/dataaccess/query/ST_Within.h \
	terralib/dataaccess/query/Sub.h \
	terralib/dataaccess/query/SubSelect.h \
	terralib/dataaccess/query/Substring.h \
	terralib/dataaccess/query/Sum.h \
	terralib/dataaccess/query/TemplateEncoder.h \
	terralib/dataaccess/query/UnaryFunction.h \
	terralib/dataaccess/query/UnaryOpEncoder.h \
	terralib/dataaccess/query/UnaryOp.h \
	terralib/dataaccess/query/Where.h
		
___bin_libterralib_dataaccess_la_SOURCES = \
	terralib/dataaccess/dataset/CheckConstraint.cpp \
	terralib/dataaccess/dataset/Constraint.cpp \
	terralib/dataaccess/dataset/DataSet.cpp \
	terralib/dataaccess/dataset/DataSetItem.cpp \
	terralib/dataaccess/dataset/DataSetPersistence.cpp \
	terralib/dataaccess/dataset/DataSetType.cpp \
	terralib/dataaccess/dataset/DataSetTypePersistence.cpp \
	terralib/dataaccess/dataset/ForeignKey.cpp \
	terralib/dataaccess/dataset/Index.cpp \
	terralib/dataaccess/dataset/PrimaryKey.cpp \
	terralib/dataaccess/dataset/Sequence.cpp \
	terralib/dataaccess/dataset/UniqueKey.cpp \
	terralib/dataaccess/datasource/AbstractConnectionPool.cpp \
	terralib/dataaccess/datasource/BatchExecutor.cpp \
	terralib/dataaccess/datasource/ConnectionPoolManager.cpp \
	terralib/dataaccess/datasource/DataSourceCatalog.cpp \
	terralib/dataaccess/datasource/DataSourceCatalogLoader.cpp \
	terralib/dataaccess/datasource/DataSource.cpp \
	terralib/dataaccess/datasource/DataSourceFactory.cpp \
	terralib/dataaccess/datasource/DataSourceManager.cpp \
	terralib/dataaccess/datasource/DataSourceTransactor.cpp \
	terralib/dataaccess/datasource/PreparedQuery.cpp \
	terralib/dataaccess/datasource/ScopedTransaction.cpp \
	terralib/dataaccess/Exception.cpp \
	terralib/dataaccess/Module.cpp \
	terralib/dataaccess/query/Add.cpp \
	terralib/dataaccess/query/And.cpp \
	terralib/dataaccess/query/BinaryFunction.cpp \
	terralib/dataaccess/query/BinaryOp.cpp \
	terralib/dataaccess/query/BinaryOpEncoder.cpp \
	terralib/dataaccess/query/DataSetName.cpp \
	terralib/dataaccess/query/Div.cpp \
	terralib/dataaccess/query/EqualTo.cpp \
	terralib/dataaccess/query/Expression.cpp \
	terralib/dataaccess/query/Field.cpp \
	terralib/dataaccess/query/FromFunctionCall.cpp \
	terralib/dataaccess/query/FromItem.cpp \
	terralib/dataaccess/query/FunctionCatalog.cpp \
	terralib/dataaccess/query/FunctionCatalogManager.cpp \
	terralib/dataaccess/query/Function.cpp \
	terralib/dataaccess/query/FunctionDefn.cpp \
	terralib/dataaccess/query/FunctionEncoder.cpp \
	terralib/dataaccess/query/FunctionNames.cpp \
	terralib/dataaccess/query/FunctionNoArgsEncoder.cpp \
	terralib/dataaccess/query/FunctionParameter.cpp \
	terralib/dataaccess/query/GreaterThan.cpp \
	terralib/dataaccess/query/GreaterThanOrEqualTo.cpp \
	terralib/dataaccess/query/GroupByItem.cpp \
	terralib/dataaccess/query/Having.cpp \
	terralib/dataaccess/query/In.cpp \
	terralib/dataaccess/query/IsNull.cpp \
	terralib/dataaccess/query/JoinCondition.cpp \
	terralib/dataaccess/query/JoinConditionOn.cpp \
	terralib/dataaccess/query/JoinConditionUsing.cpp \
	terralib/dataaccess/query/Join.cpp \
	terralib/dataaccess/query/LessThan.cpp \
	terralib/dataaccess/query/LessThanOrEqualTo.cpp \
	terralib/dataaccess/query/Like.cpp \
	terralib/dataaccess/query/LiteralByteArray.cpp \
	terralib/dataaccess/query/Literal.cpp \
	terralib/dataaccess/query/LiteralDateTime.cpp \
	terralib/dataaccess/query/LiteralDouble.cpp \
	terralib/dataaccess/query/LiteralEnvelope.cpp \
	terralib/dataaccess/query/LiteralGeom.cpp \
	terralib/dataaccess/query/LiteralInt16.cpp \
	terralib/dataaccess/query/LiteralInt32.cpp \
	terralib/dataaccess/query/LiteralInt64.cpp \
	terralib/dataaccess/query/LiteralString.cpp \
	terralib/dataaccess/query/Mul.cpp \
	terralib/dataaccess/query/Not.cpp \
	terralib/dataaccess/query/NotEqualTo.cpp \
	terralib/dataaccess/query/Or.cpp \
	terralib/dataaccess/query/OrderByItem.cpp \
	terralib/dataaccess/query/PropertyName.cpp \
	terralib/dataaccess/query/Query.cpp \
	terralib/dataaccess/query/QueryVisitor.cpp \
	terralib/dataaccess/query/Select.cpp \
	terralib/dataaccess/query/SQLDialect.cpp \
	terralib/dataaccess/query/SQLFunctionEncoder.cpp \
	terralib/dataaccess/query/SQLVisitor.cpp \
	terralib/dataaccess/query/ST_Beyond.cpp \
	terralib/dataaccess/query/ST_Contains.cpp \
	terralib/dataaccess/query/ST_Crosses.cpp \
	terralib/dataaccess/query/ST_Disjoint.cpp \
	terralib/dataaccess/query/ST_DistanceBuffer.cpp \
	terralib/dataaccess/query/ST_DWithin.cpp \
	terralib/dataaccess/query/ST_Equals.cpp \
	terralib/dataaccess/query/ST_Intersects.cpp \
	terralib/dataaccess/query/ST_Overlaps.cpp \
	terralib/dataaccess/query/ST_Relate.cpp \
	terralib/dataaccess/query/ST_Touches.cpp \
	terralib/dataaccess/query/ST_Transform.cpp \
	terralib/dataaccess/query/ST_Within.cpp \
	terralib/dataaccess/query/Sub.cpp \
	terralib/dataaccess/query/SubSelect.cpp \
	terralib/dataaccess/query/Substring.cpp \
	terralib/dataaccess/query/Sum.cpp \
	terralib/dataaccess/query/TemplateEncoder.cpp \
	terralib/dataaccess/query/UnaryFunction.cpp \
	terralib/dataaccess/query/UnaryOp.cpp \
	terralib/dataaccess/query/UnaryOpEncoder.cpp \
	terralib/dataaccess/query/Where.cpp

#########################################################################
# memory module	

lib_LTLIBRARIES += ../bin/libterralib_memory.la

___bin_libterralib_memory_la_LIBADD = \
	../bin/libterralib_common.la \
	../bin/libterralib_dataaccess.la \
	../bin/libterralib_datatype.la \
	../bin/libterralib_geometry.la \
	../bin/libterralib_plugin.la \
	../bin/libterralib_raster.la
		
nobase_include_HEADERS += \
	terralib/memory.h \
	terralib/memory_fw.h \
	terralib/memory/Band.h \
	terralib/memory/Config.h \
	terralib/memory/DataSet.h \
	terralib/memory/DataSetItem.h \
	terralib/memory/DataSetPersistence.h \
	terralib/memory/DataSetTypePersistence.h \
	terralib/memory/DataSourceCatalogLoader.h \
	terralib/memory/DataSourceFactory.h \
	terralib/memory/DataSource.h \
	terralib/memory/DataSourceTransactor.h \
	terralib/memory/Exception.h \
	terralib/memory/Globals.h \
	terralib/memory/RasterFactory.h \
	terralib/memory/Raster.h \
	terralib/memory/TiledBand.h

___bin_libterralib_memory_la_SOURCES = \
	terralib/memory/Band.cpp \
	terralib/memory/DataSet.cpp \
	terralib/memory/DataSetItem.cpp \
	terralib/memory/DataSetPersistence.cpp \
	terralib/memory/DataSetTypePersistence.cpp \
	terralib/memory/DataSourceCatalogLoader.cpp \
	terralib/memory/DataSource.cpp \
	terralib/memory/DataSourceFactory.cpp \
	terralib/memory/DataSourceTransactor.cpp \
	terralib/memory/Exception.cpp \
	terralib/memory/Globals.cpp \
	terralib/memory/Raster.cpp \
	terralib/memory/RasterFactory.cpp \
	terralib/memory/TiledBand.cpp

#########################################################################
# st module	

lib_LTLIBRARIES += ../bin/libterralib_st.la

___bin_libterralib_st_la_LIBADD = \
	../bin/libterralib_common.la \
	../bin/libterralib_datatype.la \
	../bin/libterralib_geometry.la \
	../bin/libterralib_raster.la
		
nobase_include_HEADERS += \
	terralib/st.h \
	terralib/st_fw.h \
	terralib/st/Config.h \
	terralib/st/coverage/Coverage.h \
	terralib/st/coverage/RasterCoverage.h \
	terralib/st/interpolator/AbstractInterpolator.h \
	terralib/st/interpolator/AbstractInterpolatorParams.h \
	terralib/st/interpolator/TemporalNearestNeigInterpolator.h \
	terralib/st/movingobject/MovingObject.h \
	terralib/st/movingobject/MovingObjectIterator.h \
	terralib/st/movingobject/Trajectory.h \
	terralib/st/movingobject/TrajectoryIterator.h \
	terralib/st/observation/Observation.h \
	terralib/st/observation/ObservationSet.h \
	terralib/st/observation/ObservationSetIterator.h \
	terralib/st/observation/SpatioTemporalObservationSet.h \
	terralib/st/timeseries/LocatedTimeSeries.h \
	terralib/st/timeseries/TimeSeries.h \
	terralib/st/timeseries/TimeSeriesIterator.h \
	terralib/st/Utils.h

___bin_libterralib_st_la_SOURCES = \
	terralib/st/coverage/Coverage.cpp \
	terralib/st/coverage/RasterCoverage.cpp \
	terralib/st/interpolator/TemporalNearestNeigInterpolator.cpp \
	terralib/st/movingobject/MovingObject.cpp \
	terralib/st/movingobject/MovingObjectIterator.cpp \
	terralib/st/movingobject/Trajectory.cpp \
	terralib/st/movingobject/TrajectoryIterator.cpp \
	terralib/st/observation/Observation.cpp \
	terralib/st/observation/ObservationSet.cpp \
	terralib/st/observation/ObservationSetIterator.cpp \
	terralib/st/observation/SpatioTemporalObservationSet.cpp \
	terralib/st/timeseries/LocatedTimeSeries.cpp \
	terralib/st/timeseries/TimeSeries.cpp \
	terralib/st/timeseries/TimeSeriesIterator.cpp \
	terralib/st/Utils.cpp

#########################################################################
# stloader module	

lib_LTLIBRARIES += ../bin/libterralib_stloader.la

___bin_libterralib_stloader_la_LIBADD = \
	../bin/libterralib_common.la \
	../bin/libterralib_datatype.la \
	../bin/libterralib_dataaccess.la \
	../bin/libterralib_geometry.la \
	../bin/libterralib_st.la \
	../bin/libterralib_xml.la
		
nobase_include_HEADERS += \
	terralib/stloader.h \
	terralib/stloader_fw.h \
	terralib/stloader/Config.h \
	terralib/stloader/DataLoader.h \
	terralib/stloader/LocatedTimeSeriesDataSetMetadata.h \
	terralib/stloader/MovingObjectDataSetMetadata.h \
	terralib/stloader/ObservationDataSetMetadata.h \
	terralib/stloader/Utils.h

___bin_libterralib_stloader_la_SOURCES = \
	terralib/stloader/DataLoaderCoverage.cpp \
	terralib/stloader/DataLoader.cpp \
	terralib/stloader/DataLoaderMovingObjects.cpp \
	terralib/stloader/LocatedTimeSeriesDataSetMetadata.cpp \
	terralib/stloader/MovingObjectDataSetMetadata.cpp \
	terralib/stloader/ObservationDataSetMetadata.cpp \
	terralib/stloader/Utils.cpp

#########################################################################
# gml module	

lib_LTLIBRARIES += ../bin/libterralib_gml.la

___bin_libterralib_gml_la_LIBADD = \
	../bin/libterralib_common.la \
	../bin/libterralib_geometry.la \
	../bin/libterralib_srs.la
		
nobase_include_HEADERS += \
	terralib/gml.h \
	terralib/gml/Config.h \
	terralib/gml/Envelope.h \
	terralib/gml/Module.h

___bin_libterralib_gml_la_SOURCES = \
	terralib/gml/Envelope.cpp \
	terralib/gml/Module.cpp

#########################################################################
# se module	

lib_LTLIBRARIES += ../bin/libterralib_filter.la

___bin_libterralib_filter_la_LIBADD = \
	../bin/libterralib_common.la \
	../bin/libterralib_dataaccess.la \
	../bin/libterralib_datatype.la \
	../bin/libterralib_geometry.la \
	../bin/libterralib_gml.la \
	../bin/libterralib_srs.la
		
nobase_include_HEADERS += \
	terralib/fe.h \
	terralib/fe_fw.h \
	terralib/fe/AbstractOp.h \
	terralib/fe/Add.h \
	terralib/fe/And.h \
	terralib/fe/ArithmeticOperators.h \
	terralib/fe/BBOXOp.h \
	terralib/fe/Beyond.h \
	terralib/fe/BinaryComparisonOp.h \
	terralib/fe/BinaryLogicOp.h \
	terralib/fe/BinaryOperator.h \
	terralib/fe/BinarySpatialOp.h \
	terralib/fe/ComparisonOperators.h \
	terralib/fe/ComparisonOp.h \
	terralib/fe/Config.h \
	terralib/fe/Contains.h \
	terralib/fe/Crosses.h \
	terralib/fe/Disjoint.h \
	terralib/fe/DistanceBuffer.h \
	terralib/fe/Div.h \
	terralib/fe/DWithin.h \
	terralib/fe/Equals.h \
	terralib/fe/Expression.h \
	terralib/fe/FilterCapabilities.h \
	terralib/fe/Filter.h \
	terralib/fe/Function.h \
	terralib/fe/FunctionName.h \
	terralib/fe/GeometryOperands.h \
	terralib/fe/Globals.h \
	terralib/fe/IdCapabilities.h \
	terralib/fe/Intersects.h \
	terralib/fe/Literal.h \
	terralib/fe/LogicOp.h \
	terralib/fe/Module.h \
	terralib/fe/Mul.h \
	terralib/fe/Not.h \
	terralib/fe/ObjectId.h \
	terralib/fe/Or.h \
	terralib/fe/Overlaps.h \
	terralib/fe/PropertyIsBetween.h \
	terralib/fe/PropertyIsEqualTo.h \
	terralib/fe/PropertyIsGreaterThan.h \
	terralib/fe/PropertyIsGreaterThanOrEqualTo.h \
	terralib/fe/PropertyIsLessThan.h \
	terralib/fe/PropertyIsLessThanOrEqualTo.h \
	terralib/fe/PropertyIsLike.h \
	terralib/fe/PropertyIsNotEqualTo.h \
	terralib/fe/PropertyIsNull.h \
	terralib/fe/PropertyName.h \
	terralib/fe/ScalarCapabilities.h \
	terralib/fe/SortBy.h \
	terralib/fe/SortProperty.h \
	terralib/fe/SpatialCapabilities.h \
	terralib/fe/SpatialOperator.h \
	terralib/fe/SpatialOperators.h \
	terralib/fe/SpatialOp.h \
	terralib/fe/Sub.h \
	terralib/fe/Touches.h \
	terralib/fe/UnaryLogicOp.h \
	terralib/fe/Visitor.h \
	terralib/fe/Within.h

___bin_libterralib_filter_la_SOURCES = \
	terralib/fe/AbstractOp.cpp \
	terralib/fe/Add.cpp \
	terralib/fe/And.cpp \
	terralib/fe/ArithmeticOperators.cpp \
	terralib/fe/BBOXOp.cpp \
	terralib/fe/Beyond.cpp \
	terralib/fe/BinaryComparisonOp.cpp \
	terralib/fe/BinaryLogicOp.cpp \
	terralib/fe/BinaryOperator.cpp \
	terralib/fe/BinarySpatialOp.cpp \
	terralib/fe/ComparisonOp.cpp \
	terralib/fe/ComparisonOperators.cpp \
	terralib/fe/Contains.cpp \
	terralib/fe/Crosses.cpp \
	terralib/fe/Disjoint.cpp \
	terralib/fe/DistanceBuffer.cpp \
	terralib/fe/Div.cpp \
	terralib/fe/DWithin.cpp \
	terralib/fe/Equals.cpp \
	terralib/fe/Expression.cpp \
	terralib/fe/FilterCapabilities.cpp \
	terralib/fe/Filter.cpp \
	terralib/fe/Function.cpp \
	terralib/fe/FunctionName.cpp \
	terralib/fe/GeometryOperands.cpp \
	terralib/fe/Globals.cpp \
	terralib/fe/IdCapabilities.cpp \
	terralib/fe/Intersects.cpp \
	terralib/fe/Literal.cpp \
	terralib/fe/LogicOp.cpp \
	terralib/fe/Module.cpp \
	terralib/fe/Mul.cpp \
	terralib/fe/Not.cpp \
	terralib/fe/ObjectId.cpp \
	terralib/fe/Or.cpp \
	terralib/fe/Overlaps.cpp \
	terralib/fe/PropertyIsBetween.cpp \
	terralib/fe/PropertyIsEqualTo.cpp \
	terralib/fe/PropertyIsGreaterThan.cpp \
	terralib/fe/PropertyIsGreaterThanOrEqualTo.cpp \
	terralib/fe/PropertyIsLessThan.cpp \
	terralib/fe/PropertyIsLessThanOrEqualTo.cpp \
	terralib/fe/PropertyIsLike.cpp \
	terralib/fe/PropertyIsNotEqualTo.cpp \
	terralib/fe/PropertyIsNull.cpp \
	terralib/fe/PropertyName.cpp \
	terralib/fe/ScalarCapabilities.cpp \
	terralib/fe/SortBy.cpp \
	terralib/fe/SortProperty.cpp \
	terralib/fe/SpatialCapabilities.cpp \
	terralib/fe/SpatialOp.cpp \
	terralib/fe/SpatialOperator.cpp \
	terralib/fe/SpatialOperators.cpp \
	terralib/fe/Sub.cpp \
	terralib/fe/Touches.cpp \
	terralib/fe/UnaryLogicOp.cpp \
	terralib/fe/Visitor.cpp \
	terralib/fe/Within.cpp

#########################################################################
# se module	

lib_LTLIBRARIES += ../bin/libterralib_se.la

___bin_libterralib_se_la_LIBADD = \
	../bin/libterralib_common.la \
	../bin/libterralib_color.la \
	../bin/libterralib_filter.la \
	../bin/libterralib_xlink.la
		
nobase_include_HEADERS += \
	terralib/se.h \
	terralib/se_fw.h \
	terralib/se/AnchorPoint.h \
	terralib/se/Categorize.h \
	terralib/se/ChangeCase.h \
	terralib/se/ChannelSelection.h \
	terralib/se/ColorMap.h \
	terralib/se/Concatenate.h \
	terralib/se/Config.h \
	terralib/se/ContrastEnhancement.h \
	terralib/se/CoverageStyle.h \
	terralib/se/Description.h \
	terralib/se/Displacement.h \
	terralib/se/ExternalGraphic.h \
	terralib/se/FeatureTypeStyle.h \
	terralib/se/Fill.h \
	terralib/se/Font.h \
	terralib/se/FormatDate.h \
	terralib/se/FormatNumber.h \
	terralib/se/Function.h \
	terralib/se/Graphic.h \
	terralib/se/GraphicStroke.h \
	terralib/se/Halo.h \
	terralib/se/ImageOutline.h \
	terralib/se/InlineContent.h \
	terralib/se/Interpolate.h \
	terralib/se/InterpolationPoint.h \
	terralib/se/LabelPlacement.h \
	terralib/se/LinePlacement.h \
	terralib/se/LineSymbolizer.h \
	terralib/se/MapItem.h \
	terralib/se/Mark.h \
	terralib/se/Module.h \
	terralib/se/ParameterValue.h \
	terralib/se/PointPlacement.h \
	terralib/se/PointSymbolizer.h \
	terralib/se/PolygonSymbolizer.h \
	terralib/se/RasterSymbolizer.h \
	terralib/se/Recode.h \
	terralib/se/Rule.h \
	terralib/se/SelectedChannel.h \
	terralib/se/ShadedRelief.h \
	terralib/se/StringLength.h \
	terralib/se/StringPosition.h \
	terralib/se/Stroke.h \
	terralib/se/Style.h \
	terralib/se/Substring.h \
	terralib/se/SvgParameter.h \
	terralib/se/Symbolizer.h \
	terralib/se/SymbologyManager.h \
	terralib/se/TextSymbolizer.h \
	terralib/se/Trim.h \
	terralib/se/Visitor.h

___bin_libterralib_se_la_SOURCES = \
	terralib/se/AnchorPoint.cpp \
	terralib/se/Categorize.cpp \
	terralib/se/ChangeCase.cpp \
	terralib/se/ChannelSelection.cpp \
	terralib/se/ColorMap.cpp \
	terralib/se/Concatenate.cpp \
	terralib/se/ContrastEnhancement.cpp \
	terralib/se/CoverageStyle.cpp \
	terralib/se/Description.cpp \
	terralib/se/Displacement.cpp \
	terralib/se/ExternalGraphic.cpp \
	terralib/se/FeatureTypeStyle.cpp \
	terralib/se/Fill.cpp \
	terralib/se/Font.cpp \
	terralib/se/FormatDate.cpp \
	terralib/se/FormatNumber.cpp \
	terralib/se/Function.cpp \
	terralib/se/Graphic.cpp \
	terralib/se/GraphicStroke.cpp \
	terralib/se/Halo.cpp \
	terralib/se/ImageOutline.cpp \
	terralib/se/InlineContent.cpp \
	terralib/se/Interpolate.cpp \
	terralib/se/InterpolationPoint.cpp \
	terralib/se/LabelPlacement.cpp \
	terralib/se/LinePlacement.cpp \
	terralib/se/LineSymbolizer.cpp \
	terralib/se/MapItem.cpp \
	terralib/se/Mark.cpp \
	terralib/se/Module.cpp \
	terralib/se/ParameterValue.cpp \
	terralib/se/PointPlacement.cpp \
	terralib/se/PointSymbolizer.cpp \
	terralib/se/PolygonSymbolizer.cpp \
	terralib/se/RasterSymbolizer.cpp \
	terralib/se/Recode.cpp \
	terralib/se/Rule.cpp \
	terralib/se/SelectedChannel.cpp \
	terralib/se/ShadedRelief.cpp \
	terralib/se/StringLength.cpp \
	terralib/se/StringPosition.cpp \
	terralib/se/Stroke.cpp \
	terralib/se/Style.cpp \
	terralib/se/Substring.cpp \
	terralib/se/SvgParameter.cpp \
	terralib/se/Symbolizer.cpp \
	terralib/se/SymbologyManager.cpp \
	terralib/se/TextSymbolizer.cpp \
	terralib/se/Trim.cpp

#########################################################################
# map tools module	

lib_LTLIBRARIES += ../bin/libterralib_maptools.la

___bin_libterralib_maptools_la_LIBADD = \
	../bin/libterralib_annotationtext.la \
	../bin/libterralib_color.la \
	../bin/libterralib_common.la \
	../bin/libterralib_dataaccess.la \
	../bin/libterralib_datatype.la \
	../bin/libterralib_filter.la \
	../bin/libterralib_geometry.la \
	../bin/libterralib_gml.la \
	../bin/libterralib_se.la \
	../bin/libterralib_xlink.la
		
nobase_include_HEADERS += \
	terralib/maptools.h \
	terralib/maptools_fw.h \
	terralib/maptools/AbstractLayer.h \
	terralib/maptools/AbstractMapDisplay.h \
	terralib/maptools/AbstractRenderer.h \
	terralib/maptools/Canvas.h \
	terralib/maptools/Config.h \
	terralib/maptools/DataGridOperation.h \
	terralib/maptools/Enums.h \
	terralib/maptools/FolderLayer.h \
	terralib/maptools/Layer.h \
	terralib/maptools/LayerRenderer.h \
	terralib/maptools/MapDisplay.h \
	terralib/maptools/Module.h \
	terralib/maptools/PtMarker.h \
	terralib/maptools/QueryEncoder.h \
	terralib/maptools/QueryLayer.h \
	terralib/maptools/QueryLayerRenderer.h

___bin_libterralib_maptools_la_SOURCES = \
	terralib/maptools/AbstractLayer.cpp \
	terralib/maptools/DataGridOperation.cpp \
	terralib/maptools/FolderLayer.cpp \
	terralib/maptools/Layer.cpp \
	terralib/maptools/LayerRenderer.cpp \
	terralib/maptools/MapDisplay.cpp \
	terralib/maptools/Module.cpp \
	terralib/maptools/PtMarker.cpp \
	terralib/maptools/QueryEncoder.cpp \
	terralib/maptools/QueryLayer.cpp \
	terralib/maptools/QueryLayerRenderer.cpp

#########################################################################
# org module		

if TE_BUILD_OGR_MODULE
lib_LTLIBRARIES += ../bin/libterralib_ogr.la

___bin_libterralib_ogr_la_LIBADD = \
	../bin/libterralib_common.la \
	../bin/libterralib_dataaccess.la \
	../bin/libterralib_datatype.la \
	../bin/libterralib_geometry.la \
	../bin/libterralib_memory.la \
	../bin/libterralib_plugin.la
		
nobase_include_HEADERS += \
	terralib/ogr.h \
	terralib/ogr_fw.h \
	terralib/ogr/CatalogLoader.h \
	terralib/ogr/Config.h \
	terralib/ogr/DataSet.h \
	terralib/ogr/DataSetPersistence.h \
	terralib/ogr/DataSetTypePersistence.h \
	terralib/ogr/DataSourceFactory.h \
	terralib/ogr/DataSource.h \
	terralib/ogr/DataSourceTransactor.h \
	terralib/ogr/Globals.h \
	terralib/ogr/Platform.h \
	terralib/ogr/Utils.h

___bin_libterralib_ogr_la_SOURCES = \
	terralib/ogr/CatalogLoader.cpp \
	terralib/ogr/DataSet.cpp \
	terralib/ogr/DataSetPersistence.cpp \
	terralib/ogr/DataSetTypePersistence.cpp \
	terralib/ogr/DataSource.cpp \
	terralib/ogr/DataSourceFactory.cpp \
	terralib/ogr/DataSourceTransactor.cpp \
	terralib/ogr/Globals.cpp \
	terralib/ogr/Platform.cpp \
	terralib/ogr/Utils.cpp
endif	

#########################################################################
# xlink module	

lib_LTLIBRARIES += ../bin/libterralib_xlink.la

___bin_libterralib_xlink_la_LIBADD = \
	../bin/libterralib_common.la

nobase_include_HEADERS += \
	terralib/xlink.h \
	terralib/xlink_fw.h \
	terralib/xlink/Config.h \
	terralib/xlink/Enumerators.h \
	terralib/xlink/Globals.h \
	terralib/xlink/Module.h \
	terralib/xlink/SimpleLink.h


___bin_libterralib_xlink_la_SOURCES = \
	terralib/xlink/Globals.cpp \
	terralib/xlink/Module.cpp \
	terralib/xlink/SimpleLink.cpp

#########################################################################
# grib module	  
	
if TE_BUILD_GRIB_MODULE
lib_LTLIBRARIES += ../bin/libterralib_grib.la

___bin_libterralib_grib_la_LIBADD = \
	../bin/libterralib_common.la \
	../bin/libterralib_dataaccess.la \
	../bin/libterralib_datatype.la \
	../bin/libterralib_geometry.la \
	../bin/libterralib_plugin.la \
	../bin/libterralib_raster.la \
	../bin/libterralib_srs.la
		
nobase_include_HEADERS += \
	terralib/grib/Band.h \
	terralib/grib/Config.h \
	terralib/grib/Enums.h \
	terralib/grib/Exception.h \
	terralib/grib/Platform.h \
	terralib/grib/RasterFactory.h \
	terralib/grib/Raster.h
	
___bin_libterralib_grib_la_SOURCES = \
	terralib/grib/Band.cpp \
	terralib/grib/Exception.cpp \
	terralib/grib/Platform.cpp \
	terralib/grib/Raster.cpp \
	terralib/grib/RasterFactory.cpp
endif	
  
#########################################################################
# gdal module	  
	
if TE_BUILD_GDAL_MODULE
lib_LTLIBRARIES += ../bin/libterralib_gdal.la

___bin_libterralib_gdal_la_LIBADD = \
	../bin/libterralib_common.la \
	../bin/libterralib_dataaccess.la \
	../bin/libterralib_datatype.la \
	../bin/libterralib_geometry.la \
	../bin/libterralib_memory.la \
	../bin/libterralib_ogr.la \
	../bin/libterralib_plugin.la \
	../bin/libterralib_raster.la \
	../bin/libterralib_srs.la
		
nobase_include_HEADERS += \
	terralib/gdal.h \
	terralib/gdal_fw.h \
	terralib/gdal/Band.h \
	terralib/gdal/CatalogLoader.h \
	terralib/gdal/Config.h \
	terralib/gdal/DataSet.h \
	terralib/gdal/DataSetPersistence.h \
	terralib/gdal/DataSetTypePersistence.h \
	terralib/gdal/DataSourceFactory.h \
	terralib/gdal/DataSource.h \
	terralib/gdal/DataSourceTransactor.h \
	terralib/gdal/Exception.h \
	terralib/gdal/Globals.h \
	terralib/gdal/Platform.h \
	terralib/gdal/RasterFactory.h \
	terralib/gdal/Raster.h \
	terralib/gdal/Utils.h

___bin_libterralib_gdal_la_SOURCES = \
	terralib/gdal/Band.cpp \
	terralib/gdal/CatalogLoader.cpp \
	terralib/gdal/DataSet.cpp \
	terralib/gdal/DataSetPersistence.cpp \
	terralib/gdal/DataSetTypePersistence.cpp \
	terralib/gdal/DataSource.cpp \
	terralib/gdal/DataSourceFactory.cpp \
	terralib/gdal/DataSourceTransactor.cpp \
	terralib/gdal/Exception.cpp \
	terralib/gdal/Globals.cpp \
	terralib/gdal/Platform.cpp \
	terralib/gdal/Raster.cpp \
	terralib/gdal/RasterFactory.cpp \
	terralib/gdal/Utils.cpp

endif	
	
#########################################################################
# postgis module	 	
	
if TE_BUILD_POSTGIS_MODULE
lib_LTLIBRARIES += ../bin/libterralib_postgis.la

___bin_libterralib_postgis_la_LIBADD = \
	../bin/libterralib_common.la \
	../bin/libterralib_dataaccess.la \
	../bin/libterralib_datatype.la \
	../bin/libterralib_geometry.la \
	../bin/libterralib_plugin.la \
	../bin/libterralib_srs.la \
	../bin/libterralib_memory.la

nobase_include_HEADERS += \
	terralib/postgis.h \
	terralib/postgis_fw.h \
	terralib/postgis/CatalogLoader.h \
	terralib/postgis/Config.h \
	terralib/postgis/Connection.h \
	terralib/postgis/ConnectionPool.h \
	terralib/postgis/DataSet.h \
	terralib/postgis/DataSetPersistence.h \
	terralib/postgis/DataSetTypePersistence.h \
	terralib/postgis/DataSourceFactory.h \
	terralib/postgis/DataSource.h \
	terralib/postgis/DataTypes.h \
	terralib/postgis/EWKBReader.h \
	terralib/postgis/EWKBWriter.h \
	terralib/postgis/Exception.h \
	terralib/postgis/Globals.h \
	terralib/postgis/Platform.h \
	terralib/postgis/PostGISCapabilities.h \
	terralib/postgis/PostGISDialect.h \
	terralib/postgis/PreparedQuery.h \
	terralib/postgis/SQLVisitor.h \
	terralib/postgis/Transactor.h \
	terralib/postgis/Utils.h

___bin_libterralib_postgis_la_SOURCES = \
	terralib/postgis/CatalogLoader.cpp \
	terralib/postgis/Connection.cpp \
	terralib/postgis/ConnectionPool.cpp \
	terralib/postgis/DataSet.cpp \
	terralib/postgis/DataSetPersistence.cpp \
	terralib/postgis/DataSetTypePersistence.cpp \
	terralib/postgis/DataSource.cpp \
	terralib/postgis/DataSourceFactory.cpp \
	terralib/postgis/DataTypes.cpp \
	terralib/postgis/EWKBReader.cpp \
	terralib/postgis/EWKBWriter.cpp \
	terralib/postgis/Exception.cpp \
	terralib/postgis/Globals.cpp \
	terralib/postgis/Platform.cpp \
	terralib/postgis/PreparedQuery.cpp \
	terralib/postgis/SQLVisitor.cpp \
	terralib/postgis/Transactor.cpp \
	terralib/postgis/Utils.cpp

endif

#########################################################################
# Raster Processing module		

lib_LTLIBRARIES += ../bin/libterralib_rp.la

___bin_libterralib_rp_la_LIBADD = \
	../bin/libterralib_common.la \
	../bin/libterralib_dataaccess.la \
	../bin/libterralib_datatype.la \
	../bin/libterralib_geometry.la \
	../bin/libterralib_memory.la \
	../bin/libterralib_plugin.la \
	../bin/libterralib_raster.la \
	../bin/libterralib_srs.la
    
nobase_include_HEADERS += \
	terralib/rp.h \
	terralib/rp_fw.h \
	terralib/rp/Config.h \
	terralib/rp/Matrix.h \
	terralib/rp/Macros.h \
	terralib/rp/Module.h \
	terralib/rp/Exception.h \
	terralib/rp/AbstractParameters.h \
	terralib/rp/Algorithm.h \
	terralib/rp/AlgorithmParameters.h \
	terralib/rp/RasterHandler.h \
	terralib/rp/Functions.h \
	terralib/rp/Contrast.h \
	terralib/rp/SegmenterDummyStrategy.h \
	terralib/rp/SegmenterIdsManager.h \
	terralib/rp/SegmenterSegmentsBlock.h \
	terralib/rp/SegmenterStrategyParameters.h \
	terralib/rp/SegmenterStrategy.h \
	terralib/rp/SegmenterStrategyFactory.h \
	terralib/rp/SegmenterRegionGrowingStrategy.h \
	terralib/rp/Segmenter.h 

___bin_libterralib_rp_la_SOURCES = \
	terralib/rp/Module.cpp \
	terralib/rp/Exception.cpp \
	terralib/rp/Algorithm.cpp \
	terralib/rp/AbstractParameters.cpp \
	terralib/rp/AlgorithmParameters.cpp \
	terralib/rp/RasterHandler.cpp \
	terralib/rp/Functions.cpp \
	terralib/rp/Contrast.cpp \
	terralib/rp/SegmenterDummyStrategy.cpp \
	terralib/rp/SegmenterIdsManager.cpp \
	terralib/rp/SegmenterSegmentsBlock.cpp \
	terralib/rp/SegmenterStrategyParameters.cpp \
	terralib/rp/SegmenterStrategy.cpp \
	terralib/rp/SegmenterStrategyFactory.cpp \
	terralib/rp/SegmenterRegionGrowingStrategy.cpp \
	terralib/rp/Segmenter.cpp

#########################################################################
# QT widgets module		

if TE_BUILD_QT_MODULE

lib_LTLIBRARIES += ../bin/libterralib_qtwidgets.la

___bin_libterralib_qtwidgets_la_LIBADD = \
	../bin/libterralib_annotationtext.la \
	../bin/libterralib_color.la \
	../bin/libterralib_common.la \
	../bin/libterralib_dataaccess.la \
	../bin/libterralib_datatype.la \
	../bin/libterralib_geometry.la \
	../bin/libterralib_maptools.la \
	../bin/libterralib_raster.la \
	../bin/libterralib_srs.la \
	../bin/libterralib_xlink.la

nobase_include_HEADERS += \
	terralib/qt/widgets.h \
	terralib/qt/widgets/Config.h \
	terralib/qt/widgets/Globals.h \
	terralib/qt/widgets/Module.h \
	terralib/qt/widgets/Utils.h \
	terralib/qt/widgets/canvas/Canvas.h \
	terralib/qt/widgets/canvas/DisplayEventHandler.h \
	terralib/qt/widgets/canvas/MapDisplay.h \
	terralib/qt/widgets/canvas/ZoomAndPan.h \
	terralib/qt/widgets/datagrid/DataGridItemDelegate.h \
	terralib/qt/widgets/datagrid/DataGridModel.h \
	terralib/qt/widgets/datagrid/DataGridView.h \
	terralib/qt/widgets/datagrid/HeaderView.h \
	terralib/qt/widgets/dataset/AddIndex.h \
	terralib/qt/widgets/dataset/CreateDataSet.h \
	terralib/qt/widgets/layer/AbstractLayerItemFactory.h \
	terralib/qt/widgets/layer/AbstractLayerItem.h \
	terralib/qt/widgets/layer/FolderLayerItemFactory.h \
	terralib/qt/widgets/layer/FolderLayerItem.h \
	terralib/qt/widgets/layer/LayerExplorer.h \
	terralib/qt/widgets/layer/LayerExplorerModel.h \
	terralib/qt/widgets/layer/LayerItemFactory.h \
	terralib/qt/widgets/layer/LayerItem.h \
	terralib/qt/widgets/layer/LegendItem.h \
	terralib/qt/widgets/property/AddProperty.h \
	terralib/qt/widgets/property/RemoveProperty.h \
	terralib/qt/widgets/property/RenameProperty.h \
	terralib/qt/widgets/property/UpdateProperty.h
		
EXTRA_DIST += \
	../images/zoom_in_cursor.xpm \
	../images/zoom_in_cursor_mask.xpm \
	../images/zoom_out_cursor.xpm \
	../images/zoom_out_cursor_mask.xpm \
	terralib/qt/widgets/dataset/AddIndex.ui \
	terralib/qt/widgets/dataset/CreateDataSet.ui \
	terralib/qt/widgets/property/AddProperty.ui \
	terralib/qt/widgets/property/RemoveProperty.ui \
	terralib/qt/widgets/property/RenameProperty.ui \
	terralib/qt/widgets/property/UpdateProperty.ui

TLQTWIDGETS_BUILTHEADERS = \
	terralib/qt/widgets/dataset/ui_AddIndex.h \
	terralib/qt/widgets/dataset/ui_CreateDataSet.h \
	terralib/qt/widgets/property/ui_AddProperty.h \
	terralib/qt/widgets/property/ui_RemoveProperty.h \
	terralib/qt/widgets/property/ui_RenameProperty.h \
	terralib/qt/widgets/property/ui_UpdateProperty.h

TLQTWIDGETS_BUILTSOURCES = \
	terralib/qt/widgets/canvas/moc_Canvas.cpp \
	terralib/qt/widgets/canvas/moc_DisplayEventHandler.cpp \
	terralib/qt/widgets/canvas/moc_MapDisplay.cpp \
	terralib/qt/widgets/canvas/moc_ZoomAndPan.cpp \
	terralib/qt/widgets/datagrid/moc_DataGridItemDelegate.cpp \
	terralib/qt/widgets/datagrid/moc_DataGridModel.cpp \
	terralib/qt/widgets/datagrid/moc_DataGridView.cpp \
	terralib/qt/widgets/datagrid/moc_HeaderView.cpp \
	terralib/qt/widgets/dataset/moc_AddIndex.cpp \
	terralib/qt/widgets/dataset/moc_CreateDataSet.cpp \
	terralib/qt/widgets/layer/moc_AbstractLayerItem.cpp \
	terralib/qt/widgets/layer/moc_AbstractLayerItemFactory.cpp \
	terralib/qt/widgets/layer/moc_FolderLayerItem.cpp \
	terralib/qt/widgets/layer/moc_FolderLayerItemFactory.cpp \
	terralib/qt/widgets/layer/moc_LayerExplorer.cpp \
	terralib/qt/widgets/layer/moc_LayerExplorerModel.cpp \
	terralib/qt/widgets/layer/moc_LayerItem.cpp \
	terralib/qt/widgets/layer/moc_LayerItemFactory.cpp \
	terralib/qt/widgets/layer/moc_LegendItem.cpp \
	terralib/qt/widgets/property/moc_AddProperty.cpp \
	terralib/qt/widgets/property/moc_RemoveProperty.cpp \
	terralib/qt/widgets/property/moc_RenameProperty.cpp \
	terralib/qt/widgets/property/moc_UpdateProperty.cpp

BUILT_SOURCES += $(TLQTWIDGETS_BUILTHEADERS) $(TLQTWIDGETS_BUILTSOURCES)

nodist_nobase_include_HEADERS += $(TLQTWIDGETS_BUILTHEADERS)

___bin_libterralib_qtwidgets_la_CPPFLAGS = \
	-I$(srcdir)/../images \
	-I$(srcdir)/terralib/qt/widgets/dataset \
	-I$(srcdir)/terralib/qt/widgets/property \
	$(AM_CPPFLAGS)
  
nodist____bin_libterralib_qtwidgets_la_SOURCES = $(TLQTWIDGETS_BUILTSOURCES)

___bin_libterralib_qtwidgets_la_SOURCES = \
	terralib/qt/widgets/Globals.cpp \
	terralib/qt/widgets/Module.cpp \
	terralib/qt/widgets/Utils.cpp \
	terralib/qt/widgets/canvas/Canvas.cpp \
	terralib/qt/widgets/canvas/DisplayEventHandler.cpp \
	terralib/qt/widgets/canvas/MapDisplay.cpp \
	terralib/qt/widgets/canvas/ZoomAndPan.cpp \
	terralib/qt/widgets/datagrid/DataGridItemDelegate.cpp \
	terralib/qt/widgets/datagrid/DataGridModel.cpp \
	terralib/qt/widgets/datagrid/DataGridView.cpp \
	terralib/qt/widgets/datagrid/HeaderView.cpp \
	terralib/qt/widgets/dataset/AddIndex.cpp \
	terralib/qt/widgets/dataset/CreateDataSet.cpp \
	terralib/qt/widgets/layer/AbstractLayerItem.cpp \
	terralib/qt/widgets/layer/AbstractLayerItemFactory.cpp \
	terralib/qt/widgets/layer/FolderLayerItem.cpp \
	terralib/qt/widgets/layer/FolderLayerItemFactory.cpp \
	terralib/qt/widgets/layer/LayerExplorer.cpp \
	terralib/qt/widgets/layer/LayerExplorerModel.cpp \
	terralib/qt/widgets/layer/LayerItem.cpp \
	terralib/qt/widgets/layer/LayerItemFactory.cpp \
	terralib/qt/widgets/layer/LegendItem.cpp \
	terralib/qt/widgets/property/AddProperty.cpp \
	terralib/qt/widgets/property/RemoveProperty.cpp \
	terralib/qt/widgets/property/RenameProperty.cpp \
	terralib/qt/widgets/property/UpdateProperty.cpp

endif	
	
######################################################################### 
# Adding all built sources to the distclean step

CLEANFILES += $(BUILT_SOURCES)	
