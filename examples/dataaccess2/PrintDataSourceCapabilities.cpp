// Examples
#include "DataAccessExamples.h"

// TerraLib

// STL
#include <iostream>
#include <string>

void PrintHelper(const std::string& label, const bool& value)
{
  std::cout << "  - " << label << ": ";
  value ? std::cout << "Yes" << std::endl : std::cout << "No" << std::endl;
}

void PrintDataSourceCapabilities(te::da::DataSource* ds)
{
  if(ds == 0)
  {
    std::cout << "The given datasource is NULL!" << std::endl;
    return;
  }

  // Retrieves the data source capabilities
  const te::da::DataSourceCapabilities& capabilities = ds->getCapabilities();

  // Let's print the informed capabilities...
  std::cout << "== DataSource Capabilities ==" << std::endl;
  std::cout << "  - Access Policy: ";
  switch(capabilities.getAccessPolicy())
  {
    case te::common::NoAccess:
      std::cout << "NoAccess (No access allowed)" << std::endl;
    break;

    case te::common::RAccess:
      std::cout << "RAccess (Read-only access allowed)" << std::endl;
    break;

    case te::common::WAccess:
      std::cout << "WAccess (Write-only access allowed)" << std::endl;
    break;

    case te::common::RWAccess:
      std::cout << "RWAccess (Read and write access allowed)" << std::endl;
    break;
  }

  PrintHelper("Support Transactions", capabilities.supportsTransactions());
  PrintHelper("Support Dataset Persitence API", capabilities.supportsDataSetPesistenceAPI());
  PrintHelper("Support Dataset Type Persitence API", capabilities.supportsDataSetTypePesistenceAPI());
  PrintHelper("Support Prepared Query API", capabilities.supportsPreparedQueryAPI());
  PrintHelper("Support Batch Executor API", capabilities.supportsBatchExecutorAPI());

  std::cout << ":: DataType Capabilities" << std::endl;
  const te::da::DataTypeCapabilities dataTypeCapabilities = capabilities.getDataTypeCapabilities();
  PrintHelper("Support BIT", dataTypeCapabilities.supportsBit());
  PrintHelper("Support CHAR", dataTypeCapabilities.supportsChar());
  PrintHelper("Support UCHAR", dataTypeCapabilities.supportsUChar());
  PrintHelper("Support INT16", dataTypeCapabilities.supportsInt16());
  PrintHelper("Support UINT16", dataTypeCapabilities.supportsUInt16());
  PrintHelper("Support INT32", dataTypeCapabilities.supportsInt32());
  PrintHelper("Support UINT32", dataTypeCapabilities.supportsUInt32());
  PrintHelper("Support INT64", dataTypeCapabilities.supportsInt64());
  PrintHelper("Support UINT64", dataTypeCapabilities.supportsUInt64());
  PrintHelper("Support BOOLEAN", dataTypeCapabilities.supportsBoolean());
  PrintHelper("Support FLOAT", dataTypeCapabilities.supportsFloat());
  PrintHelper("Support DOUBLE", dataTypeCapabilities.supportsDouble());
  PrintHelper("Support NUMERIC", dataTypeCapabilities.supportsNumeric());
  PrintHelper("Support STRING", dataTypeCapabilities.supportsString());
  PrintHelper("Support BYTE_ARRAY", dataTypeCapabilities.supportsByteArray());
  PrintHelper("Support GEOMETRY", dataTypeCapabilities.supportsGeometry());
  PrintHelper("Support DATE_TIME", dataTypeCapabilities.supportsDateTime());
  PrintHelper("Support ARRAY", dataTypeCapabilities.supportsArray());
  PrintHelper("Support COMPOSITE", dataTypeCapabilities.supportsComposite());
  PrintHelper("Support DATASET", dataTypeCapabilities.supportsDataset());
  PrintHelper("Support RASTER", dataTypeCapabilities.supportsRaster());
  PrintHelper("Support XML", dataTypeCapabilities.supportsXML());
  PrintHelper("Support CINT16", dataTypeCapabilities.supportsCInt16());
  PrintHelper("Support CINT32", dataTypeCapabilities.supportsCInt32());
  PrintHelper("Support CFLOAT", dataTypeCapabilities.supportsCFloat());
  PrintHelper("Support CDOUBLE", dataTypeCapabilities.supportsCDouble());
  //PrintHelper("Support DATASET_ITEM", dataTypeCapabilities.supportsDatasetItem());
  PrintHelper("Support POLYMORPHIC", dataTypeCapabilities.supportsPolymorphic());

  std::cout << ":: DataSetType Capabilities" << std::endl;
  const te::da::DataSetTypeCapabilities dataSetTypeCapabilities = capabilities.getDataSetTypeCapabilities();
  PrintHelper("Support Primary Key", dataSetTypeCapabilities.supportsPrimaryKey());
  PrintHelper("Support Unique Key", dataSetTypeCapabilities.supportsUniqueKey());
  PrintHelper("Support Foreign Key", dataSetTypeCapabilities.supportsForeignKey());
  PrintHelper("Support Sequence", dataSetTypeCapabilities.supportsSequence());
  PrintHelper("Support CheckConstraints", dataSetTypeCapabilities.supportsCheckConstraints());
  PrintHelper("Support Index", dataSetTypeCapabilities.supportsIndex());
  PrintHelper("Support R-Tree Index", dataSetTypeCapabilities.supportsRTreeIndex());
  PrintHelper("Support B-Tree Index", dataSetTypeCapabilities.supportsBTreeIndex());
  PrintHelper("Support Hash Index", dataSetTypeCapabilities.supportsHashIndex());
  PrintHelper("Support Quad-Tree Index", dataSetTypeCapabilities.supportsQuadTreeIndex());

  std::cout << ":: DataSet Capabilities" << std::endl;
  const te::da::DataSetCapabilities dataSetCapabilities = capabilities.getDataSetCapabilities();
  PrintHelper("Support Traversing in a Bidirectional way", dataSetCapabilities.supportsBidirectionalTraversing());
  PrintHelper("Support Traversing in a Random way", dataSetCapabilities.supportsRandomTraversing());
  PrintHelper("Support Traversing using a given key", dataSetCapabilities.supportsIndexedTraversing());
  PrintHelper("Efficient Move Previous", dataSetCapabilities.supportsEfficientMovePrevious());
  PrintHelper("Efficient Move Before First", dataSetCapabilities.supportsEfficientMoveBeforeFirst());
  PrintHelper("Efficient Move Last", dataSetCapabilities.supportsEfficientMoveLast());
  PrintHelper("Efficient Move After Last", dataSetCapabilities.supportsEfficientMoveAfterLast());
  PrintHelper("Efficient Move", dataSetCapabilities.supportsEfficientMove());
  PrintHelper("Efficient Dataset Size", dataSetCapabilities.supportsEfficientDataSetSize());
  PrintHelper("Support Insertion", dataSetCapabilities.supportsInsertion());
  PrintHelper("Support Update", dataSetCapabilities.supportsUpdate());
  PrintHelper("Support Deletion", dataSetCapabilities.supportsDeletion());

  std::cout << ":: Query Capabilities" << std::endl;
  const te::da::QueryCapabilities queryCapabilities = capabilities.getQueryCapabilities();
  PrintHelper("Support SQL Dialect", queryCapabilities.supportsSQLDialect());
  PrintHelper("Support INSERT command", queryCapabilities.supportsInsert());
  PrintHelper("Support UPDATE command", queryCapabilities.supportsUpdate());
  PrintHelper("Support DELETE command", queryCapabilities.supportsDelete());
  PrintHelper("Support CREATE command", queryCapabilities.supportsCreate());
  PrintHelper("Support DROP command", queryCapabilities.supportsDrop());
  PrintHelper("Support ALTER command", queryCapabilities.supportsAlter());
  PrintHelper("Support SELECT command", queryCapabilities.supportsSelect());
  PrintHelper("Support SELECT INTO command", queryCapabilities.supportsSelectInto());
}
