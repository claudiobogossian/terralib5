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

// DataSouce Capabilities
te::da::DataSourceCapabilities capabilities;
capabilities.setAccessPolicy(te::common::RWAccess);
capabilities.setSupportAll();
capabilities.setDataTypeCapabilities(dataTypeCapabilities);
capabilities.setDataSetTypeCapabilities(dataSetTypeCapabilities);
capabilities.setDataSetCapabilities(dataSetCapabilities);
capabilities.setQueryCapabilities(queryCapabilities);

DataSource::setCapabilities(capabilities);
