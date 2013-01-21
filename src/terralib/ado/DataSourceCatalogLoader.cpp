/*  Copyright (C) 2001-2009 National Institute For Space Research (INPE) - Brazil.

    This file is part of the TerraLib - a Framework for building GIS enabled applications.

    TerraLib is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation, either version 3 of the License,
    or (at your option) any later version.

    TerraLib is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with TerraLib. See COPYING. If not, write to
    TerraLib Team at <terralib-team@terralib.org>.
 */

/*!
  \file DataSourceCatalogLoader.cpp

  \brief DataSourceCatalogLoader class implementation for ADO.
*/

// TerraLib
#include "../common/Translator.h"
#include "../dataaccess/dataset/DataSetType.h"
#include "../dataaccess/datasource/DataSource.h"
#include "../dataaccess/datasource/DataSourceCatalog.h"
#include "../datatype/SimpleProperty.h"
#include "../dataaccess/dataset/DataSetItem.h"
#include "../dataaccess/dataset/DataSetType.h"
#include "../dataaccess/dataset/DataSet.h"
#include "../dataaccess/dataset/Constraint.h"
#include "../dataaccess/dataset/PrimaryKey.h"
#include "../dataaccess/dataset/ForeignKey.h"
#include "../dataaccess/dataset/UniqueKey.h"
#include "../dataaccess/dataset/Index.h"
#include "../datatype.h"
#include "../geometry/Geometry.h"
#include "../geometry/GeometryProperty.h"
#include "../geometry/Envelope.h"
#include "DataSourceCatalogLoader.h"
#include "DataSourceTransactor.h"
#include "Exception.h"
#include "Utils.h"

// ADO
#import "msado15.dll" \
    no_namespace rename("EOF", "EndOfFile")
#import "msadox.dll"

inline void TESTHR(HRESULT x)
{
  if FAILED(x)
    _com_issue_error(x);
};

te::ado::DataSourceCatalogLoader::DataSourceCatalogLoader(DataSourceTransactor* t)
  : m_t(t)
{
}

te::ado::DataSourceCatalogLoader::~DataSourceCatalogLoader()
{
}

void te::ado::DataSourceCatalogLoader::getDataSets(boost::ptr_vector<std::string>& datasets)
{
  _ConnectionPtr adoConn = m_t->getADOConnection();

  ADOX::_CatalogPtr pCatalog = 0;

  TESTHR(pCatalog.CreateInstance(__uuidof(ADOX::Catalog)));

  try
  {
    pCatalog->PutActiveConnection(variant_t((IDispatch *)adoConn));

    ADOX::TablesPtr tables = pCatalog->GetTables();
    
    for(long i = 0; i < tables->GetCount(); i++)
    {
      ADOX::_TablePtr table = tables->GetItem(i);
      std::string tableName = table->GetName();

      std::string tabletype = table->GetType();

      if(table->GetType() == _bstr_t("ACCESS TABLE") || 
         table->GetType() == _bstr_t("LINK") || 
         table->GetType() == _bstr_t("PASS-THROUGH") ||
         table->GetType() == _bstr_t("SYSTEM TABLE") ||
         table->GetType() == _bstr_t("VIEW") ||
         tableName == "geometry_columns")
         continue;

      datasets.push_back(new std::string(table->GetName()));
    }
  }
  catch(_com_error &e)
  {
    throw Exception(TR_ADO(e.ErrorMessage()));
  }
}

te::da::DataSetType* te::ado::DataSourceCatalogLoader::getDataSetType(const std::string& datasetName,
                                                                      const bool full)
{
  if(!datasetExists(datasetName))
    return 0;
  
  std::auto_ptr<te::da::DataSetType> dt(new te::da::DataSetType(datasetName));
  dt->setTitle(datasetName);

  getProperties(dt.get());

  if(full)
  {
    getPrimaryKey(dt.get());
    getUniqueKeys(dt.get());
    getIndexes(dt.get());
  }

  dt->setFullLoaded(full);
  return dt.release();
}

//te::da::DataSetType* te::ado::DataSourceCatalogLoader::getDataSetType(const std::string& datasetName,
//                                                                      const bool full)
//{
//  if(!datasetExists(datasetName))
//    return 0;
//
//  _ConnectionPtr adoConn = m_t->getADOConnection();
//
//  ADOX::_CatalogPtr pCatalog = 0;
//  
//  TESTHR(pCatalog.CreateInstance(__uuidof(ADOX::Catalog)));
//
//  try
//  {
//    pCatalog->PutActiveConnection(variant_t((IDispatch *)adoConn));
//  }
//  catch(_com_error& e)
//  {
//    throw Exception(TR_ADO(e.Description()));
//  }
//
//  ADOX::TablesPtr tables = pCatalog->GetTables();
//
//  ADOX::_TablePtr t = tables->GetItem(datasetName.c_str());
//
//  ADOX::ColumnsPtr cols = t->GetColumns();
//  
//  std::auto_ptr<te::da::DataSetType> dt(new te::da::DataSetType(datasetName));
//  dt->setTitle(datasetName);
//
//  std::vector<te::dt::Property*> properties;
//  
//  _variant_t vIndex;
//  for(long i = 0; i < cols->Count; i++)
//  {
//    vIndex = i;
//    ADOX::_ColumnPtr c = cols->GetItem(vIndex);
//
//    te::dt::Property* prop = te::ado::Convert2Terralib(c);
//
//    dt->add(prop);
//    
//    if(te::ado::isGeomProperty(adoConn, dt->getName(), prop->getName()))
//    {
//      te::gm::GeometryProperty* geop = (te::gm::GeometryProperty*)prop;
//      geop->setSRID(getSRID(adoConn, geop));
//      te::gm::GeomType t = getType(adoConn, geop);
//      geop->setGeometryType(t);
//      dt->setDefaultGeomProperty(geop);
//    }
//  }
//  
//  getPrimaryKey(dt.get());
//  getIndexes(dt.get());
//  getUniqueKeys(dt.get());
//
//  return dt.release();
//}

void te::ado::DataSourceCatalogLoader::getProperties(te::da::DataSetType* dt)
{
  std::string dsName = dt->getName();
  int numCols = 0;
  te::dt::Property* p = 0;

  ADOX::DataTypeEnum colType;
  std::map<int, std::string> colNamesMap;
  std::map<int, ADOX::DataTypeEnum> colTypesMap;
  std::map<int, int> charLengthMap;
  std::map<int, bool> isRequiredMap;
  std::map<int, bool> hasDefaultMap;
  std::map<int, std::string> defaultValueMap;

  try
  {
    HRESULT hr = S_OK;

    _ConnectionPtr conn = m_t->getADOConnection();

    _RecordsetPtr rs = NULL;

    TESTHR(rs.CreateInstance(__uuidof(Recordset)));

    // Create a safearray which takes three elements,and pass it as 
    // the second parameter in the OpenSchema method.
    SAFEARRAY FAR* psa = NULL;
    SAFEARRAYBOUND rgsabound;
    _variant_t var[3];

    _variant_t  Array;
    rgsabound.lLbound = 0;
    rgsabound.cElements = 3;
    psa = SafeArrayCreate(VT_VARIANT, 1, &rgsabound);

    var[0].vt = VT_EMPTY;
    var[1].vt = VT_EMPTY;
    var[2] = dsName.c_str();

    // Fill the safe array.
    for(LONG i = 0; i < 3; ++i)
      hr = SafeArrayPutElement(psa, &i, &var[i]);

    Array.vt = VT_ARRAY | VT_VARIANT;
    Array.parray = psa;  

    rs = conn->OpenSchema(adSchemaColumns, &Array, vtMissing);

    int pos;
    while (!(rs->EndOfFile))
    {
      // Get the column name
      _bstr_t columnName = rs->Fields->GetItem("COLUMN_NAME")->Value;
      pos = rs->Fields->GetItem("ORDINAL_POSITION")->Value;
      pos = pos - 1;
      colNamesMap[pos] = (LPCSTR)columnName;

      // Get the data type of the column
      colType = ADOX::DataTypeEnum(int(rs->Fields->GetItem("DATA_TYPE")->Value));
      colTypesMap[pos] = colType;

      // Get the length of the column
      _variant_t length = rs->Fields->GetItem("CHARACTER_MAXIMUM_LENGTH")->Value;
      int charLength = 0;
      if(length.vt != VT_NULL)
        charLength = (int)length.dblVal;
      charLengthMap[pos] = charLength;

      // Get the columns that accept null values
      bool nullVal = rs->Fields->GetItem("IS_NULLABLE")->Value;
      isRequiredMap[pos] = !nullVal;

      // Get the columns that has default values
      bool hasDefault = rs->Fields->GetItem("COLUMN_HASDEFAULT")->Value;
      isRequiredMap[pos] = !hasDefault;

      // Get the default value
      std::string defaultStr;
      if(hasDefault)
      {
        _bstr_t defaultValue = rs->Fields->GetItem("COLUMN_DEFAULT")->Value;
         defaultStr = (LPSTR)defaultValue;
      }
        
      defaultValueMap[pos] = defaultStr;

      rs->MoveNext();
      ++numCols;
    }
  }
  catch (_com_error& e)
  {
    std::cout << "Error = " << (char*) e.ErrorMessage() << std::endl;
  }

  // Create the dataset properties
  for(int i = 0; i < numCols; ++i)
  {
    ADOX::DataTypeEnum colType = colTypesMap[i];
    std::string colName = colNamesMap[i];

    switch(colType)
    {
      case ::adBoolean:
        p = new te::dt::SimpleProperty(colName, Convert2Terralib(colType));
        break;

      case ::adVarWChar:
      case ::adWChar:
      case ::adVarChar:
      case ::adLongVarChar:
      case ::adLongVarWChar:
      case ::adBSTR:
      case ::adChar:
        p = new te::dt::StringProperty(colName, (te::dt::StringType)Convert2Terralib(colType), charLengthMap[i]);
        break;

      case ADOX::adTinyInt:
      case ADOX::adSmallInt:
      case ADOX::adInteger:
      case ADOX::adBigInt:
      case ADOX::adSingle:
      case ADOX::adDouble:
      case ::adUnsignedBigInt:
      case ::adUnsignedInt:
      case ::adUnsignedSmallInt:
      case ::adUnsignedTinyInt:
      case ADOX::adLongVarBinary:
      case ADOX::adBinary:
        p = new te::dt::SimpleProperty(colName, Convert2Terralib(colType));
        break;

      case ADOX::adDate:
      case ADOX::adDBDate:
      case ADOX::adDBTime:
      case ADOX::adDBTimeStamp:
        p = new te::dt::DateTimeProperty(colName, te::dt::TIME_INSTANT);
        break;
          
      default:
        p = new te::dt::SimpleProperty(colName, te::dt::UNKNOWN_TYPE);
        break;
    }

    dt->add(p);
  }
}

//void te::ado::DataSourceCatalogLoader::getProperties(te::da::DataSetType* dt)
//{
//  _ConnectionPtr adoConn = m_t->getADOConnection();
//
//  ADOX::_CatalogPtr pCatalog = 0;
//
//  TESTHR(pCatalog.CreateInstance(__uuidof(ADOX::Catalog)));
//
//  try
//  {
//    pCatalog->PutActiveConnection(variant_t((IDispatch *)adoConn));
//  }
//  catch(_com_error& e)
//  {
//    throw Exception(TR_ADO(e.Description()));
//  }
//
//  ADOX::TablesPtr tables = pCatalog->GetTables();
//
//  ADOX::_TablePtr t = tables->GetItem(dt->getName().c_str());
//
//  ADOX::ColumnsPtr cols = t->GetColumns();
//
//  std::vector<te::dt::Property*> properties;
//
//  for(long i = 0; i < cols->Count; i++)
//  {
//    ADOX::_ColumnPtr c = cols->GetItem(i);
//
//    te::dt::Property* prop = te::ado::Convert2Terralib(c);
//
//    dt->add(prop);
//
//    if(te::ado::isGeomProperty(adoConn, dt->getName(), prop->getName()))
//    {
//      te::gm::GeometryProperty* geop = (te::gm::GeometryProperty*)prop;
//      geop->setSRID(getSRID(adoConn, geop));
//      te::gm::GeomType t = getType(adoConn, geop);
//      geop->setGeometryType(t);
//      dt->setDefaultGeomProperty(geop);
//    }
//  }
//
//  //getPrimaryKey(dt);
//  //getIndexes(dt);
//  //getUniqueKeys(dt);
//}

te::dt::Property* te::ado::DataSourceCatalogLoader::getProperty(const std::string& /*datasetName*/, const std::string& /*propertyName*/)
{
  throw Exception(TR_ADO("Not implemented yet!"));
}

void te::ado::DataSourceCatalogLoader::getPrimaryKey(te::da::DataSetType* dt)
{
  _ConnectionPtr adoConn = m_t->getADOConnection();

  ADOX::_CatalogPtr pCatalog = 0;

  TESTHR(pCatalog.CreateInstance(__uuidof(ADOX::Catalog)));

  try
  {
    pCatalog->PutActiveConnection(variant_t((IDispatch *)adoConn));
  }
  catch(_com_error& e)
  {
    throw Exception(TR_ADO(e.Description()));
  }
  ADOX::TablesPtr tables = pCatalog->GetTables();

  ADOX::_TablePtr t = tables->GetItem(dt->getName().c_str());

  ADOX::KeysPtr keys = t->GetKeys();

  ADOX::_KeyPtr pk = 0;

  for(long i = 0; i < keys->Count; i++)
  {
    if(keys->GetItem(i)->GetType() == ADOX::adKeyPrimary)
      pk = keys->GetItem(i);
  }

  if(pk == 0)
  {
    dt->setPrimaryKey(0);
    return;
  }

  ADOX::ColumnsPtr cols = pk->GetColumns();

  std::auto_ptr<te::da::PrimaryKey> tlPk( new te::da::PrimaryKey(std::string(pk->GetName()), dt));

  for(long i = 0; i < cols->GetCount(); i++)
    tlPk->add(dt->getProperty(std::string(cols->GetItem(i)->GetName())));

  dt->add(tlPk.release());
}

void te::ado::DataSourceCatalogLoader::getUniqueKeys(te::da::DataSetType* dt)
{
  _ConnectionPtr adoConn = m_t->getADOConnection();

  ADOX::_CatalogPtr pCatalog = 0;

  TESTHR(pCatalog.CreateInstance(__uuidof(ADOX::Catalog)));

  try
  {
    pCatalog->PutActiveConnection(variant_t((IDispatch *)adoConn));
  }
  catch(_com_error& e)
  {
    throw Exception(TR_ADO(e.Description()));
  }

  ADOX::TablesPtr tables = pCatalog->GetTables();

  ADOX::_TablePtr t = tables->GetItem(dt->getName().c_str());

  ADOX::KeysPtr keys = t->GetKeys();  

  for(long i = 0; i < keys->Count; i++)
  {
    if(keys->GetItem(i)->GetType() == ADOX::adKeyUnique)
    {
      ADOX::_KeyPtr uk = keys->GetItem(i);
      
      std::auto_ptr<te::da::UniqueKey> tlUk( new te::da::UniqueKey(std::string(uk->GetName()), dt));

      ADOX::ColumnsPtr cols = uk->GetColumns();

      for(long j = 0; j < cols->Count; j++)
        tlUk->add(dt->getProperty(std::string(cols->GetItem(i)->GetName())));

      //dt->add(tlUk.release());  já está adicionado
    }
  } 
}

void te::ado::DataSourceCatalogLoader::getForeignKeys(te::da::DataSetType* dt, std::vector<std::string>& fkNames, std::vector<std::string>& rdts)
{
  _ConnectionPtr adoConn = m_t->getADOConnection();

  ADOX::_CatalogPtr pCatalog = 0;

  TESTHR(pCatalog.CreateInstance(__uuidof(ADOX::Catalog)));

  try
  {
    pCatalog->PutActiveConnection(variant_t((IDispatch *)adoConn));
  }
  catch(_com_error& e)
  {
    throw Exception(TR_ADO(e.Description()));
  }

  ADOX::TablesPtr tables = pCatalog->GetTables();

  ADOX::_TablePtr t = tables->GetItem(dt->getName().c_str());

  ADOX::KeysPtr keys = t->GetKeys();  

  for(long i = 0; i < keys->Count; i++)
  {
    if(keys->GetItem(i)->GetType() == ADOX::adKeyForeign)
    {
      ADOX::_KeyPtr fk = keys->GetItem(i);

      fkNames.push_back(std::string(fk->GetName()));
      rdts.push_back(std::string(fk->GetRelatedTable()));
    }
  }
}

te::da::ForeignKey* te::ado::DataSourceCatalogLoader::getForeignKey(const std::string& fkName, te::da::DataSetType* dt, te::da::DataSetType* rdt)
{
  _ConnectionPtr adoConn = m_t->getADOConnection();

  ADOX::_CatalogPtr pCatalog = 0;

  TESTHR(pCatalog.CreateInstance(__uuidof(ADOX::Catalog)));

  try
  {
    pCatalog->PutActiveConnection(variant_t((IDispatch *)adoConn));
  }
  catch(_com_error& e)
  {
    throw Exception(TR_ADO(e.Description()));
  }

  ADOX::TablesPtr tables = pCatalog->GetTables();

  ADOX::_TablePtr t = tables->GetItem(dt->getName().c_str());

  ADOX::KeysPtr keys = t->GetKeys();

  ADOX::_KeyPtr adofk = keys->GetItem(fkName.c_str());
  
  if(std::string(adofk->GetRelatedTable()) == rdt->getName())
    throw Exception(TR_ADO("Foreign Key not found!"));

  te::da::ForeignKey* fk = new te::da::ForeignKey(fkName);
  fk->setDataSetType(dt);
  fk->setReferencedDataSetType(rdt);

  std::vector<te::dt::Property*> props;
  std::vector<te::dt::Property*> rProps;

  for(long i = 0; i < adofk->GetColumns()->GetCount(); i++)
  {
    ADOX::_ColumnPtr col = adofk->GetColumns()->GetItem(i);

    props.push_back(dt->getProperty(std::string(col->GetName())));
    rProps.push_back(rdt->getProperty(std::string(col->GetRelatedColumn())));
  }

  fk->setProperties(props);

  fk->setReferencedProperties(rProps);

  return fk;
}

void te::ado::DataSourceCatalogLoader::getIndexes(te::da::DataSetType* dt)
{
  _ConnectionPtr adoConn = m_t->getADOConnection();

  ADOX::_CatalogPtr pCatalog = 0;

  TESTHR((pCatalog.CreateInstance(__uuidof(ADOX::Catalog))));

  try
  {
    pCatalog->PutActiveConnection(variant_t((IDispatch *)adoConn));
  }
  catch(_com_error& e)
  {
    throw Exception(TR_ADO(e.Description()));
  }

  ADOX::TablesPtr tables = pCatalog->GetTables();

  ADOX::_TablePtr t = tables->GetItem(dt->getName().c_str());

  ADOX::IndexesPtr idxs = t->GetIndexes();

  for(long i = 0; i < idxs->GetCount(); i++)
  {
    ADOX::_IndexPtr idx = idxs->GetItem(i);

    te::da::Index* tlIdx = new te::da::Index();
    tlIdx->setName(std::string(idx->GetName()));

    std::vector<te::dt::Property*> props;

    ADOX::ColumnsPtr cols = idx->GetColumns();
    for(long i = 0; i < cols->GetCount(); i++)
      props.push_back(dt->getProperty(std::string(cols->GetItem(i)->GetName())));
    
    tlIdx->setProperties(props);

    dt->add(tlIdx);
  } 
}

void te::ado::DataSourceCatalogLoader::getCheckConstraints(te::da::DataSetType* dt)
{
  throw Exception(TR_ADO("Not implemented yet!"));
}

void te::ado::DataSourceCatalogLoader::getSequences(std::vector<std::string*>& sequences)
{
  throw Exception(TR_ADO("Not implemented yet!"));
}

te::da::Sequence* te::ado::DataSourceCatalogLoader::getSequence(const std::string& seqName)
{
  throw Exception(TR_ADO("Not implemented yet!"));
}

te::gm::Envelope* te::ado::DataSourceCatalogLoader::getExtent(const te::gm::GeometryProperty* gp)
{
  if(!datasetExists(gp->getParent()->getName()))
    throw Exception(TR_ADO("The Data Set Type doesn't exist!"));

  std::string tableName = gp->getParent()->getName();

  te::da::DataSetType* dt = getDataSetType(tableName);

  te::da::DataSet* ds = m_t->getDataSet(dt->getName());

  te::gm::Envelope* env = new te::gm::Envelope();

  bool first = true;
  while(ds->moveNext())
  {
    te::gm::Geometry* geo = ds->getGeometry(dt->getDefaultGeomProperty()->getName());
    std::string aaaaaa = geo->asText();
    geo->computeMBR(true);
    
    if(first)
    {
      env->m_llx = geo->getMBR()->getLowerLeftX();
      env->m_lly = geo->getMBR()->getLowerLeftY();
      env->m_urx = geo->getMBR()->getUpperRightX();
      env->m_ury = geo->getMBR()->getUpperRightY();
      first = false;
      continue;
    }

    if(geo->getMBR()->getLowerLeftX() < env->m_llx) env->m_llx = geo->getMBR()->getLowerLeftX();
    if(geo->getMBR()->getLowerLeftY() < env->m_lly) env->m_lly = geo->getMBR()->getLowerLeftY();
    if(geo->getMBR()->getUpperRightX() > env->m_urx) env->m_urx = geo->getMBR()->getUpperRightX();
    if(geo->getMBR()->getUpperRightY() > env->m_ury) env->m_ury = geo->getMBR()->getUpperRightY();
  }

  return env;
}

void te::ado::DataSourceCatalogLoader::getGeometryColumn(te::da::DataSetType* dt, te::gm::GeometryProperty* geomp)
{
  //geomp = getDefaultGeomProperty(dt, m_t->getADOConnection());
  
  //_ConnectionPtr adoConn = m_t->getADOConnection();

  //geomp->getParent()->setName(dt->getName());
  //geomp->setSRID(te::ado::getSRID(adoConn, geomp));
  //geomp->setGeometryType(te::ado::getType(adoConn, geomp));
  dt->setDefaultGeomProperty(geomp);
}

void te::ado::DataSourceCatalogLoader::loadCatalog(const bool full)
{
  te::da::DataSourceCatalog* catalog = m_t->getDataSource()->getCatalog();
  catalog->clear();

  // get the list of tables in the database and create a DataSetType for each table or view  
  boost::ptr_vector<std::string> datasets;
  getDataSets(datasets);

  for(size_t i = 0; i < datasets.size(); i++)
  {
    te::da::DataSetTypePtr dt(new te::da::DataSetType(datasets[i]));
    dt->setTitle(datasets[i]);
    dt->setFullLoaded(full);

    catalog->add(dt);

    getProperties(dt.get());
  }

  if(!full)
    return;

  // for each table in the catalog, finish loading its information: pk, fk, uk, indexes
  // note: this will avoid the need to load tables in the right order to insert into the catalog.
  std::size_t ndsets = catalog->getNumberOfDataSets();

  for(std::size_t i = 0; i < ndsets; ++i)
  {
    te::da::DataSetTypePtr dt = catalog->getDataSetType(i);

    getCheckConstraints(dt.get());
    getIndexes(dt.get());
  }
}

bool te::ado::DataSourceCatalogLoader::hasDataSets()
{
  _ConnectionPtr adoConn = m_t->getADOConnection();

  ADOX::_CatalogPtr pCatalog = 0;

  TESTHR(pCatalog.CreateInstance(__uuidof(ADOX::Catalog)));

  try
  {
    pCatalog->PutActiveConnection(variant_t((IDispatch *)adoConn));

    ADOX::TablesPtr tables = pCatalog->GetTables();

    if(tables->GetCount() > 0)
      return true;
  }
  catch(_com_error &e)
  {
    throw Exception(TR_ADO(e.ErrorMessage()));
  }

  return false;
}

bool te::ado::DataSourceCatalogLoader::datasetExists(const std::string& name)
{
  _ConnectionPtr adoConn = m_t->getADOConnection();

  ADOX::_CatalogPtr pCatalog = 0;

  TESTHR(pCatalog.CreateInstance(__uuidof(ADOX::Catalog)));

  try
  {
    pCatalog->PutActiveConnection(variant_t((IDispatch *)adoConn));
  }
  catch(_com_error& e)
  {
    throw Exception(TR_ADO(e.Description()));
  }

  ADOX::_TablePtr table = 0;

  for(long i = 0; i < pCatalog->GetTables()->Count; i++)
    if(std::string(pCatalog->GetTables()->GetItem(i)->GetName()) == name)
      return true;

  return false;
}

bool te::ado::DataSourceCatalogLoader::primarykeyExists(const std::string& name)
{
  _ConnectionPtr adoConn = m_t->getADOConnection();

  ADOX::_CatalogPtr pCatalog = 0;

  TESTHR(pCatalog.CreateInstance(__uuidof(ADOX::Catalog)));

  try
  {
    pCatalog->PutActiveConnection(variant_t((IDispatch *)adoConn));
  }
  catch(_com_error& e)
  {
    throw Exception(TR_ADO(e.Description()));
}

  ADOX::TablesPtr tables = pCatalog->GetTables();

  for(long i = 0; i < tables->Count; i++)
  {
    ADOX::_TablePtr tab = tables->GetItem(i);

    ADOX::KeysPtr keys = tab->GetKeys();

    for(long j = 0; j < keys->Count; j++)
    {
      ADOX::_KeyPtr key = keys->GetItem(j);
      if(key->GetType() == ADOX::adKeyPrimary && std::string(key->GetName()) == name.c_str())
        return true;
    }
  }

  return false;
}

bool te::ado::DataSourceCatalogLoader::uniquekeyExists(const std::string& name)
{
  _ConnectionPtr adoConn = m_t->getADOConnection();

  ADOX::_CatalogPtr pCatalog = 0;

  TESTHR(pCatalog.CreateInstance(__uuidof(ADOX::Catalog)));

  try
  {
    pCatalog->PutActiveConnection(variant_t((IDispatch *)adoConn));
  }
  catch(_com_error& e)
  {
    throw Exception(TR_ADO(e.Description()));
  }

  ADOX::TablesPtr tables = pCatalog->GetTables();

  for(long i = 0; i < tables->Count; i++)
  {
    ADOX::_TablePtr tab = tables->GetItem(i);

    ADOX::KeysPtr keys = tab->GetKeys();

    for(long j = 0; j < keys->Count; j++)
    {
      ADOX::_KeyPtr key = keys->GetItem(j);
      if(key->GetType() == ADOX::adKeyUnique && std::string(key->GetName()) == name.c_str())
        return true;
    }
  }

  return false;
}

bool te::ado::DataSourceCatalogLoader::foreignkeyExists(const std::string& name)
{
  _ConnectionPtr adoConn = m_t->getADOConnection();

  ADOX::_CatalogPtr pCatalog = 0;

  TESTHR(pCatalog.CreateInstance(__uuidof(ADOX::Catalog)));

  try
  {
    pCatalog->PutActiveConnection(variant_t((IDispatch *)adoConn));
  }
  catch(_com_error& e)
  {
    throw Exception(TR_ADO(e.Description()));
  }

  ADOX::TablesPtr tables = pCatalog->GetTables();

  for(long i = 0; i < tables->Count; i++)
  {
    ADOX::_TablePtr tab = tables->GetItem(i);

    ADOX::KeysPtr keys = tab->GetKeys();

    for(long j = 0; j < keys->Count; j++)
    {
      ADOX::_KeyPtr key = keys->GetItem(j);
      if(key->GetType() == ADOX::adKeyForeign && std::string(key->GetName()) == name.c_str())
        return true;
    }
  }

  return false;
}

bool te::ado::DataSourceCatalogLoader::checkConstraintExists(const std::string& name)
{
  throw Exception(TR_ADO("Not implemented yet!"));
}

bool te::ado::DataSourceCatalogLoader::indexExists(const std::string& name)
{
  _ConnectionPtr adoConn = m_t->getADOConnection();

  ADOX::_CatalogPtr pCatalog = 0;

  TESTHR(pCatalog.CreateInstance(__uuidof(ADOX::Catalog)));

  try
  {
    pCatalog->PutActiveConnection(variant_t((IDispatch *)adoConn));
  }
  catch(_com_error& e)
  {
    throw Exception(TR_ADO(e.Description()));
}

  ADOX::TablesPtr tables = pCatalog->GetTables();

  for(long i = 0; i < tables->Count; i++)
  {
    ADOX::_TablePtr tab = tables->GetItem(i);

    ADOX::IndexesPtr idxs = tab->GetIndexes();

    for(long j = 0; j < idxs->Count; j++)
    {
      ADOX::_IndexPtr idx = idxs->GetItem(j);
      if(std::string(idx->GetName()) == name.c_str())
        return true;
    }
  }

  return false;
}

bool te::ado::DataSourceCatalogLoader::sequenceExists(const std::string& name)
{
  throw Exception(TR_ADO("Not implemented yet!"));
}

te::da::DataSourceTransactor* te::ado::DataSourceCatalogLoader::getTransactor() const
{
  return m_t;
}
