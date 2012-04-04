/* !!This file will be replaced by a XML file loaded at run time!! */

std::map<std::string, std::string> capabilities;

capabilities["ACCESS_POLICY"] = "RW_ACCESS";

capabilities["TRANSACTION"] = "TRUE";
capabilities["DATASET_PERSISTENCE"] = "TRUE";
capabilities["DATASETTYPE_PERSISTENCE"] = "TRUE";
capabilities["PREPARED_QUERY"] = "TRUE";
capabilities["BATCH_EXECUTOR"] = "TRUE";

capabilities["BIT_DT"] = "FALSE";
capabilities["CHAR_DT"] = "FALSE";
capabilities["UCHAR_DT"] = "FALSE";
capabilities["INT16_DT"] = "TRUE";
capabilities["UINT16_DT"] = "FALSE";
capabilities["INT32_DT"] = "TRUE";
capabilities["UINT32_DT"] = "FALSE";
capabilities["INT64_DT"] = "TRUE";
capabilities["UINT64_DT"] = "FALSE";
capabilities["BOOLEAN_DT"] = "TRUE";
capabilities["FLOAT_DT"] = "TRUE";
capabilities["DOUBLE_DT"] = "TRUE";
capabilities["NUMERIC_DT"] = "TRUE";
capabilities["STRING_DT"] = "TRUE";
capabilities["BYTEARRAY_DT"] = "TRUE";
capabilities["GEOMETRY_DT"] = "TRUE";
capabilities["DATETIME_DT"] = "TRUE";
capabilities["ARRAY_DT"] = "TRUE";
capabilities["COMPOSITE_DT"] = "FALSE";
capabilities["RASTER_DT"] = "TRUE";
capabilities["DATASET_DT"] = "FALSE";

capabilities["PRIMARY_KEY"] = "TRUE";
capabilities["UNIQUE_KEY"] = "TRUE";
capabilities["FOREIGN_KEY"] = "TRUE";
capabilities["SEQUENCE"] = "TRUE";
capabilities["CHECK_CONSTRAINTS"] = "TRUE";
capabilities["INDEX"] = "TRUE";
capabilities["RTREE_INDEX"] = "TRUE";
capabilities["BTREE_INDEX"] = "TRUE";
capabilities["HASH_INDEX"] = "TRUE";
capabilities["QUADTREE_INDEX"] = "FALSE";

capabilities["DATASET_BIDIRECTIONAL"] = "TRUE";
capabilities["EFFICIENT_MOVE_PREVIOUS"] = "TRUE";
capabilities["EFFICIENT_MOVE_FIRST"] = "TRUE";
capabilities["EFFICIENT_MOVE_BEFORE_FIRST"] = "TRUE";
capabilities["EFFICIENT_MOVE_LAST"] = "TRUE";
capabilities["EFFICIENT_MOVE_AFTER_LAST"] = "TRUE";
capabilities["EFFICIENT_MOVE"] = "TRUE";
capabilities["EFFICIENT_DATASET_SIZE"] = "TRUE";
capabilities["DATASET_INSERTION"] = "FALSE";
capabilities["DATASET_UPDATE"] = "FALSE";
capabilities["DATASET_DELETION"] = "FALSE";

capabilities["SQL_DIALECT"] = "TRUE";
capabilities["INSERT_QUERY"] = "TRUE";
capabilities["UPDATE_QUERY"] = "TRUE";
capabilities["DELETE_QUERY"] = "TRUE";
capabilities["CREATE_QUERY"] = "TRUE";
capabilities["DROP_QUERY"] = "TRUE";
capabilities["ALTER_QUERY"] = "TRUE";
capabilities["SELECT_QUERY"] = "TRUE";
capabilities["SELECT_INTO_QUERY"] = "TRUE";

DataSource::setCapabilities(capabilities);
