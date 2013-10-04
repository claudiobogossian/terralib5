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
dataSetCapabilities.setSupportInsertion(false);
dataSetCapabilities.setSupportUpdate(false);
dataSetCapabilities.setSupportDeletion(false);

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

queryCapabilities.addSpatialOperator("st_contains");
queryCapabilities.addSpatialOperator("st_covers");
queryCapabilities.addSpatialOperator("st_coveredby");
queryCapabilities.addSpatialOperator("st_crosses");
queryCapabilities.addSpatialOperator("st_disjoint");
queryCapabilities.addSpatialOperator("st_equals");
queryCapabilities.addSpatialOperator("st_overlaps");
queryCapabilities.addSpatialOperator("st_relate");
queryCapabilities.addSpatialOperator("st_touches");
queryCapabilities.addSpatialOperator("st_intersects");
queryCapabilities.addSpatialOperator("st_within");

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
queryCapabilities.addSpatialOperator("st_area");
queryCapabilities.addSpatialOperator("st_centroid");
queryCapabilities.addSpatialOperator("st_difference");
queryCapabilities.addSpatialOperator("st_distance");
queryCapabilities.addSpatialOperator("st_dfullywithin");
queryCapabilities.addSpatialOperator("st_dwithin");
queryCapabilities.addSpatialOperator("st_length");
queryCapabilities.addSpatialOperator("st_perimeter");
queryCapabilities.addSpatialOperator("st_buffer");
queryCapabilities.addSpatialOperator("st_convexhull");
queryCapabilities.addSpatialOperator("st_difference");
queryCapabilities.addSpatialOperator("st_intersection");
queryCapabilities.addSpatialOperator("st_symdifference");
queryCapabilities.addSpatialOperator("st_union");
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
