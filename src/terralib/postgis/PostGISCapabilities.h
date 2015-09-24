/* !!This file will be replaced by a XML file loaded at run time!! */

// DataType Capabilites
te::da::DataTypeCapabilities dataTypeCapabilities;
dataTypeCapabilities.setSupportAll();
dataTypeCapabilities.setSupportBit(false);
dataTypeCapabilities.setSupportChar(false);
dataTypeCapabilities.setSupportUChar(false);
dataTypeCapabilities.setSupportUInt16(false);
dataTypeCapabilities.setSupportUInt32(false);
dataTypeCapabilities.setSupportUInt64(false);
dataTypeCapabilities.setSupportComposite(false);
dataTypeCapabilities.setSupportDataset(false);
dataTypeCapabilities.setSupportCInt16(false);
dataTypeCapabilities.setSupportCInt32(false);
dataTypeCapabilities.setSupportCFloat(false);
dataTypeCapabilities.setSupportCDouble(false);
dataTypeCapabilities.setSupportPolymorphic(false);

// DataSetType Capabilites
te::da::DataSetTypeCapabilities dataSetTypeCapabilities;
dataSetTypeCapabilities.setSupportAll();
dataSetTypeCapabilities.setSupportQuadTreeIndex(false);

// DataSet Capabilities
te::da::DataSetCapabilities dataSetCapabilities;
dataSetCapabilities.setSupportAll();

// Query Capabilities
te::da::QueryCapabilities queryCapabilities;
queryCapabilities.setSupportAll();

/* Logical Operators */
queryCapabilities.addLogicalOperator("and");
queryCapabilities.addLogicalOperator("or");
queryCapabilities.addLogicalOperator("not");

/* Arithmetic Operators */
queryCapabilities.addArithmeticOperator("+");
queryCapabilities.addArithmeticOperator("-");
queryCapabilities.addArithmeticOperator("*");
queryCapabilities.addArithmeticOperator("/");

/* Comparsion Operators */
queryCapabilities.addComparsionOperator("=");
queryCapabilities.addComparsionOperator("<>");
queryCapabilities.addComparsionOperator(">");
queryCapabilities.addComparsionOperator("<");
queryCapabilities.addComparsionOperator(">=");
queryCapabilities.addComparsionOperator("<=");

/* Functions*/
queryCapabilities.addFunction("upper");
queryCapabilities.addFunction("lower");
queryCapabilities.addFunction("sum");
queryCapabilities.addFunction("avg");
queryCapabilities.addFunction("coalesce");
queryCapabilities.addFunction("count");
queryCapabilities.addFunction("max");
queryCapabilities.addFunction("min");
queryCapabilities.addFunction("stddev_pop");
queryCapabilities.addFunction("var_pop");

/* Spatial Operators */
queryCapabilities.addSpatialOperator("st_boundary");
queryCapabilities.addSpatialOperator("st_coorddim");
queryCapabilities.addSpatialOperator("st_dimension");
queryCapabilities.addSpatialOperator("st_endpoint");
queryCapabilities.addSpatialOperator("st_envelope");
queryCapabilities.addSpatialOperator("st_exteriorring");
queryCapabilities.addSpatialOperator("st_geometryn");
queryCapabilities.addSpatialOperator("st_geometrytype");
queryCapabilities.addSpatialOperator("st_interiorringn");
queryCapabilities.addSpatialOperator("st_isclosed");
queryCapabilities.addSpatialOperator("st_isempty");
queryCapabilities.addSpatialOperator("st_isring");
queryCapabilities.addSpatialOperator("st_issimple");
queryCapabilities.addSpatialOperator("st_isvalid");

queryCapabilities.addSpatialOperator("st_ndims");
queryCapabilities.addSpatialOperator("st_nrings");
queryCapabilities.addSpatialOperator("st_numgeometries");
queryCapabilities.addSpatialOperator("st_numinteriorrings");
queryCapabilities.addSpatialOperator("st_numpoints");
queryCapabilities.addSpatialOperator("st_pointn");
queryCapabilities.addSpatialOperator("st_srid");
queryCapabilities.addSpatialOperator("st_startpoint");
queryCapabilities.addSpatialOperator("st_x");
queryCapabilities.addSpatialOperator("st_y");
queryCapabilities.addSpatialOperator("st_z");
queryCapabilities.addSpatialOperator("st_m");
queryCapabilities.addSpatialOperator("st_transform");
queryCapabilities.addSpatialOperator("st_dfullywithin");
queryCapabilities.addSpatialOperator("st_dwithin");

queryCapabilities.addSpatialTopologicOperator("st_contains");
queryCapabilities.addSpatialTopologicOperator("st_covers");
queryCapabilities.addSpatialTopologicOperator("st_coveredby");
queryCapabilities.addSpatialTopologicOperator("st_crosses");
queryCapabilities.addSpatialTopologicOperator("st_disjoint");
queryCapabilities.addSpatialTopologicOperator("st_equals");
queryCapabilities.addSpatialTopologicOperator("st_overlaps");
queryCapabilities.addSpatialTopologicOperator("st_relate");
queryCapabilities.addSpatialTopologicOperator("st_touches");
queryCapabilities.addSpatialTopologicOperator("st_intersects");
queryCapabilities.addSpatialTopologicOperator("st_within");

queryCapabilities.addSpatialMetricOperator("st_area");
queryCapabilities.addSpatialMetricOperator("st_distance");
queryCapabilities.addSpatialMetricOperator("st_length");
queryCapabilities.addSpatialMetricOperator("st_perimeter");

queryCapabilities.addSpatialNewGeomOperator("st_centroid");
queryCapabilities.addSpatialNewGeomOperator("st_difference");
queryCapabilities.addSpatialNewGeomOperator("st_buffer");
queryCapabilities.addSpatialNewGeomOperator("st_convexhull");
queryCapabilities.addSpatialNewGeomOperator("st_difference");
queryCapabilities.addSpatialNewGeomOperator("st_intersection");
queryCapabilities.addSpatialNewGeomOperator("st_symdifference");
queryCapabilities.addSpatialNewGeomOperator("st_union");
/* - end Spatial Operators */

// DataSouce Capabilities
te::da::DataSourceCapabilities capabilities;
capabilities.setAccessPolicy(te::common::RWAccess);
capabilities.setSupportAll();
capabilities.setDataTypeCapabilities(dataTypeCapabilities);
capabilities.setDataSetTypeCapabilities(dataSetTypeCapabilities);
capabilities.setDataSetCapabilities(dataSetCapabilities);
capabilities.setQueryCapabilities(queryCapabilities);

DataSource::setCapabilities(capabilities);
