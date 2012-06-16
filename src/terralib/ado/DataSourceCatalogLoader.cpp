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
#include "../datatype/SimpleProperty.h"
#include "../dataaccess/dataset/Constraint.h"
#include "../dataaccess/dataset/PrimaryKey.h"
#include "../dataaccess/dataset/UniqueKey.h"
#include "../dataaccess/dataset/Index.h"
#include "DataSourceCatalogLoader.h"
#include "DataSourceTransactor.h"
#include "Exception.h"
#include "Utils.h"

// ADO
#import "msado15.dll" \
    no_namespace rename("EOF", "EndOfFile")
#import "msadox.dll"

te::ado::DataSourceCatalogLoader::DataSourceCatalogLoader(DataSourceTransactor* parent)
  : m_t(parent)
{
}

te::ado::DataSourceCatalogLoader::~DataSourceCatalogLoader()
{
}

void te::ado::DataSourceCatalogLoader::getDataSets(std::vector<std::string*>& datasets)
{
  _ConnectionPtr adoConn = m_t->getADOConnection();

  _RecordsetPtr pRstSchema = 0;

  pRstSchema = adoConn->OpenSchema(adSchemaTables);

  while ( !(pRstSchema->EndOfFile) )
  {
    _bstr_t table_name = pRstSchema->Fields->GetItem("TABLE_NAME")->Value;
    datasets.push_back(new std::string(table_name));

    pRstSchema->MoveNext();
  }
}

te::da::DataSetType* te::ado::DataSourceCatalogLoader::getDataSetType(const std::string& datasetName,
                                                                           const bool full)
{
  _ConnectionPtr adoConn = m_t->getADOConnection();

  ADOX::_CatalogPtr pCatalog = 0;

  pCatalog.CreateInstance(__uuidof(ADOX::Catalog));

  pCatalog->PutActiveConnection(variant_t((IDispatch *)adoConn));

  ADOX::TablesPtr tables = pCatalog->GetTables();

  ADOX::_TablePtr t = tables->GetItem(datasetName.c_str());

  ADOX::ColumnsPtr cols = t->GetColumns();

  std::auto_ptr<te::da::DataSetType> dt(new te::da::DataSetType(datasetName));
  dt->setTitle(datasetName);

  std::vector<te::dt::Property*> properties;
  
  for(long i = 0; i < cols->Count; i++)
  {
    ADOX::_ColumnPtr c = cols->GetItem(i);

    te::dt::Property* prop = te::ado::Convert2Terralib(c);

    dt->add(prop);
  }

  getPrimaryKey(dt.get());
  getUniqueKeys(dt.get());

  return dt.release();
}

void te::ado::DataSourceCatalogLoader::getPrimaryKey(te::da::DataSetType* dt)
{
  _ConnectionPtr adoConn = m_t->getADOConnection();

  ADOX::_CatalogPtr pCatalog = 0;

  pCatalog.CreateInstance(__uuidof(ADOX::Catalog));

  pCatalog->PutActiveConnection(variant_t((IDispatch *)adoConn));

  ADOX::TablesPtr tables = pCatalog->GetTables();

  ADOX::_TablePtr t = tables->GetItem(dt->getName().c_str());

  ADOX::KeysPtr keys = t->GetKeys();

  ADOX::_KeyPtr pk = 0;

  for(long i = 0; i < keys->Count; i++)
  {
    if(keys->GetItem(i)->GetType() == ADOX::adKeyPrimary)
      pk = keys->GetItem(i);
  }

  ADOX::ColumnsPtr cols = pk->GetColumns();

  std::auto_ptr<te::da::PrimaryKey> tlPk( new te::da::PrimaryKey(std::string(pk->GetName()), dt));

  for(long i = 0; i < cols->GetCount(); i++)
  {
    tlPk->add(dt->getProperty(std::string(cols->GetItem(i)->GetName())));
  }

  dt->add(tlPk.release());

}

void te::ado::DataSourceCatalogLoader::getUniqueKeys(te::da::DataSetType* dt)
{
  _ConnectionPtr adoConn = m_t->getADOConnection();

  ADOX::_CatalogPtr pCatalog = 0;

  pCatalog.CreateInstance(__uuidof(ADOX::Catalog));

  pCatalog->PutActiveConnection(variant_t((IDispatch *)adoConn));

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

	  dt->add(tlUk.release());

    }
  }
}

void te::ado::DataSourceCatalogLoader::getForeignKeys(te::da::DataSetType* dt, std::vector<std::string>& fkNames, std::vector<std::string>& rdts)
{
  throw Exception(TR_ADO("Not implemented yet!"));
}

te::da::ForeignKey* te::ado::DataSourceCatalogLoader::getForeignKey(const std::string& fkName, te::da::DataSetType* dt, te::da::DataSetType* rdt)
{
  throw Exception(TR_ADO("Not implemented yet!"));
}

void te::ado::DataSourceCatalogLoader::getIndexes(te::da::DataSetType* dt)
{
  _ConnectionPtr adoConn = m_t->getADOConnection();

  ADOX::_CatalogPtr pCatalog = 0;

  pCatalog.CreateInstance(__uuidof(ADOX::Catalog));

  pCatalog->PutActiveConnection(variant_t((IDispatch *)adoConn));

  ADOX::TablesPtr tables = pCatalog->GetTables();

  ADOX::_TablePtr t = tables->GetItem(dt->getName().c_str());

  ADOX::IndexesPtr idxs = t->GetIndexes();

  //te::da::Index* tlIdx = new te::da::Index();
  
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
  throw Exception(TR_ADO("Not implemented yet!"));
}

void te::ado::DataSourceCatalogLoader::loadCatalog(const bool full)
{
  throw Exception(TR_ADO("Not implemented yet!"));
}

bool te::ado::DataSourceCatalogLoader::datasetExists(const std::string& name)
{
  _ConnectionPtr adoConn = m_t->getADOConnection();

  ADOX::_CatalogPtr pCatalog = 0;

  pCatalog.CreateInstance(__uuidof(ADOX::Catalog));

  pCatalog->PutActiveConnection(variant_t((IDispatch *)adoConn));

  ADOX::TablesPtr tables = pCatalog->GetTables();

  for(long i = 0; i < tables->Count; i++)
  {
    ADOX::_TablePtr tab = tables->GetItem(i);

    if(std::string(tab->GetName()) == name)
      return true;
  }

  return false;
}

bool te::ado::DataSourceCatalogLoader::primarykeyExists(const std::string& name)
{
  _ConnectionPtr adoConn = m_t->getADOConnection();

  ADOX::_CatalogPtr pCatalog = 0;

  pCatalog.CreateInstance(__uuidof(ADOX::Catalog));

  pCatalog->PutActiveConnection(variant_t((IDispatch *)adoConn));

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

  pCatalog.CreateInstance(__uuidof(ADOX::Catalog));

  pCatalog->PutActiveConnection(variant_t((IDispatch *)adoConn));

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

  pCatalog.CreateInstance(__uuidof(ADOX::Catalog));

  pCatalog->PutActiveConnection(variant_t((IDispatch *)adoConn));

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

  pCatalog.CreateInstance(__uuidof(ADOX::Catalog));

  pCatalog->PutActiveConnection(variant_t((IDispatch *)adoConn));

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