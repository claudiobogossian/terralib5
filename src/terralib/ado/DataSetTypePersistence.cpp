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
  \file DataSetTypePersistence.cpp

  \brief DataSetTypePersistence implementation for ADO.
*/

// TerraLib
#include "../common/Translator.h"
#include "../dataaccess/dataset/Index.h"
#include "../dataaccess/dataset/Sequence.h"
#include "../dataaccess/dataset/UniqueKey.h"
#include "../dataaccess/dataset/ForeignKey.h"
#include "../dataaccess/dataset/PrimaryKey.h"
#include "../dataaccess/dataset/DataSetType.h"
#include "../dataaccess/dataset/CheckConstraint.h"
#include "../dataaccess/datasource/DataSourceCatalog.h"
#include "../dataaccess/datasource/DataSourceCatalogLoader.h"
#include "../datatype/SimpleProperty.h"
#include "../datatype/StringProperty.h"
#include "../datatype/DataType.h"
#include "Utils.h"
#include "DataSet.h"
#include "Exception.h"
#include "DataSource.h"
#include "DataSourceTransactor.h"
#include "DataSetTypePersistence.h"
#include "DataSourceCatalogLoader.h"

// ADO
#import "msado15.dll" \
    no_namespace rename("EOF", "EndOfFile")
#import "msadox.dll"
#include "icrsint.h"

// Boost
#include <boost/format.hpp>

inline void TESTHR(HRESULT x)
{
  if FAILED(x)
    _com_issue_error(x);
};

te::ado::DataSetTypePersistence::DataSetTypePersistence(DataSourceTransactor* parent)
  : m_t(parent)
{
}

te::ado::DataSetTypePersistence::~DataSetTypePersistence()
{
}

void te::ado::DataSetTypePersistence::create(te::da::DataSetType* dt, const std::map<std::string, std::string>& /*options*/)
{
  try
  {
    m_t->begin();
    
    _ConnectionPtr adoConn = m_t->getADOConnection();
    DataSourceCatalogLoader* loader = dynamic_cast<DataSourceCatalogLoader*>(m_t->getCatalogLoader());

    ADOX::_TablePtr pTable = 0;
    ADOX::_CatalogPtr pCatalog = 0;

    pTable.CreateInstance(__uuidof (ADOX::Table));
    pCatalog.CreateInstance(__uuidof(ADOX::Catalog));

    pCatalog->PutActiveConnection(variant_t((IDispatch *)adoConn));

    pTable->Name = dt->getName().c_str();

    TESTHR(pCatalog->Tables->Append(_variant_t((IDispatch*)pTable)));

    std::size_t ncols = dt->size();
    for(std::size_t i = 0; i < ncols; ++i)
      add(dt, dt->getProperty(i));

    if(dt->getPrimaryKey())
      add(dt, dt->getPrimaryKey());

    /*
    std::size_t nukeys = dt->getNumberOfUniqueKeys();
    for(std::size_t i = 0; i < nukeys; ++i)
      add(dt, dt->getUniqueKey(i));


    std::size_t nidxs = dt->getNumberOfIndexes();
    for(std::size_t i = 0; i < nidxs; ++i)
        add(dt, dt->getIndex(i));

    std::size_t nfks = dt->getNumberOfForeignKeys();
    for(std::size_t i = 0; i < nfks; ++i)
      add(dt, dt->getForeignKey(i));

    std::size_t nccs = dt->getNumberOfCheckConstraints();
    for(std::size_t i = 0; i < nccs; ++i)
      add(dt, dt->getCheckConstraint(i));

    loader->getIndexes(dt);
    loader->getCheckConstraints(dt);
    */

    m_t->commit();

    pCatalog->Tables->Refresh();

  }catch(_com_error &e)
  {
    std::string description(e.Description()); 
    m_t->rollBack();
    throw Exception(TR_ADO("ADO Driver Error: " + description));
  }

}

void te::ado::DataSetTypePersistence::drop(te::da::DataSetType* dt)
{
  try
  {
    _ConnectionPtr adoConn = m_t->getADOConnection();

    ADOX::_CatalogPtr pCatalog = 0;

    pCatalog.CreateInstance(__uuidof(ADOX::Catalog));

    pCatalog->Tables->Delete(dt->getName().c_str());

  }
  catch(_com_error &e)
  {
    std::string description(e.Description()); 
    throw Exception(TR_ADO("ADO Driver Error: " + description));
  }
}

void te::ado::DataSetTypePersistence::rename(te::da::DataSetType* dt, const std::string& newName)
{
  _ConnectionPtr adoConn = m_t->getADOConnection();

  ADOX::_CatalogPtr pCatalog = 0;

  pCatalog.CreateInstance(__uuidof(ADOX::Catalog));

  pCatalog->Tables->GetItem(dt->getName().c_str())->PutName(newName.c_str());
}

void te::ado::DataSetTypePersistence::add(te::da::DataSetType* dt, te::dt::Property* p)
{
  _ConnectionPtr adoConn = m_t->getADOConnection();

  ADOX::_CatalogPtr pCatalog = 0;
  ADOX::_TablePtr pTable = 0;

  pCatalog.CreateInstance(__uuidof(ADOX::Catalog));
  pCatalog->PutActiveConnection(variant_t((IDispatch *)adoConn));

  pTable = pCatalog->Tables->GetItem(dt->getName().c_str());

  addAdoPropertyFromTerralib(pTable, p);

}

void te::ado::DataSetTypePersistence::drop(te::dt::Property* p)
{
  throw Exception(TR_ADO("Not implemented yet!"));

  _ConnectionPtr adoConn = m_t->getADOConnection();

  ADOX::_CatalogPtr pCatalog = 0;

  pCatalog.CreateInstance(__uuidof(ADOX::Catalog));

  
}

void te::ado::DataSetTypePersistence::rename(te::dt::Property* /*p*/, const std::string& /*newName*/) 
{
  throw Exception(TR_ADO("Not implemented yet!"));
}

void te::ado::DataSetTypePersistence::update(te::dt::Property* /*oldP*/, te::dt::Property* /*newP*/)
{
  throw Exception(TR_ADO("Not implemented yet!"));
}

void te::ado::DataSetTypePersistence::add(te::da::DataSetType* dt, te::da::PrimaryKey* pk) 
{
  _variant_t vOptional;
  vOptional.vt = VT_ERROR;
  vOptional.scode = DISP_E_PARAMNOTFOUND;

  _ConnectionPtr adoConn = m_t->getADOConnection();

  ADOX::_KeyPtr pKey = 0;
  ADOX::_TablePtr pTable = 0;
  ADOX::_CatalogPtr pCatalog = 0;

  pCatalog.CreateInstance(__uuidof(ADOX::Catalog));
  pKey.CreateInstance(__uuidof(ADOX::Key));
  
  pCatalog->PutActiveConnection(variant_t((IDispatch *)adoConn));

  pTable = pCatalog->Tables->GetItem(dt->getName().c_str());

  pKey->Name = pTable->Name + "_pk";
  pKey->Type = ADOX::adKeyPrimary;

  for(size_t i = 0; i < pk->getProperties().size(); i++)
  {
    te::dt::Property* p = pk->getProperties()[i];
    pKey->Columns->Append(p->getName().c_str(), te::ado::terralib2Ado(p->getType()), 256);

  }

  pTable->Keys->Append(_variant_t((IDispatch *)pKey),ADOX::adKeyPrimary,vOptional,L"",L"");
  pCatalog->Tables->Refresh();

}

void te::ado::DataSetTypePersistence::drop(te::da::PrimaryKey* pk)
{
  throw Exception(TR_ADO("Not implemented yet!"));
}

void te::ado::DataSetTypePersistence::add(te::da::DataSetType* dt, te::da::UniqueKey* uk) 
{
  _variant_t vOptional;
  vOptional.vt = VT_ERROR;
  vOptional.scode = DISP_E_PARAMNOTFOUND;

  _ConnectionPtr adoConn = m_t->getADOConnection();
  
  ADOX::_KeyPtr pKey = 0;
  ADOX::_TablePtr pTable = 0;
  ADOX::_CatalogPtr pCatalog = 0;

  pCatalog.CreateInstance(__uuidof(ADOX::Catalog));
  pKey.CreateInstance(__uuidof(ADOX::Key));

  pCatalog->PutActiveConnection(variant_t((IDispatch *)adoConn));

  pTable = pCatalog->Tables->GetItem(dt->getName().c_str());

  pKey->Name = pTable->Name + "_uk";
  pKey->Type = ADOX::adKeyUnique;

  for(size_t i = 0; i < uk->getProperties().size(); i++)
  {
    te::dt::Property* p = uk->getProperties()[i];

    pKey->Columns->Append(p->getName().c_str(), te::ado::terralib2Ado(p->getType()), 256);
  }

  pTable->Keys->Append(_variant_t((IDispatch *)pKey),ADOX::adKeyPrimary,vOptional,L"",L"");

  pCatalog->Tables->Refresh();
}

void te::ado::DataSetTypePersistence::drop(te::da::UniqueKey* /*uk*/)
{
  throw Exception(TR_ADO("Not implemented yet!"));
}

void te::ado::DataSetTypePersistence::add(te::da::DataSetType* /*dt*/, te::da::Index* /*index*/) 
{
  throw Exception(TR_ADO("Not implemented yet!"));
}

void te::ado::DataSetTypePersistence::add(te::da::DataSetType* /*dt*/, te::da::Index* /*index*/, 
  const std::map<std::string, std::string>& /*options*/)
{
  throw Exception(TR_ADO("Not implemented yet!"));
}

void te::ado::DataSetTypePersistence::drop(te::da::Index* /*index*/)
{
  throw Exception(TR_ADO("Not implemented yet!"));
}

void te::ado::DataSetTypePersistence::add(te::da::DataSetType* dt, te::da::ForeignKey* fk)
{
  _variant_t vOptional;
  vOptional.vt = VT_ERROR;
  vOptional.scode = DISP_E_PARAMNOTFOUND;

  _ConnectionPtr adoConn = m_t->getADOConnection();

  ADOX::_KeyPtr pKey = 0;
  ADOX::_TablePtr pTable = 0;
  ADOX::_CatalogPtr pCatalog = 0;

  pCatalog->PutActiveConnection(variant_t((IDispatch *)adoConn));

  pTable = pCatalog->Tables->GetItem(dt->getName().c_str());

  pKey.CreateInstance(__uuidof(ADOX::Key));

  pKey->Name = pTable->Name + "_uk";
  pKey->Type = ADOX::adKeyForeign;
  pKey->RelatedTable = fk->getDataSetType()->getName().c_str();
  
  for(size_t i = 0; i < fk->getProperties().size(); i++)
  {

    te::dt::Property* p = fk->getProperties()[i];

    pKey->Columns->Append(p->getName().c_str(), te::ado::terralib2Ado(p->getType()), 256);
    //pKey->Columns->GetItem(p->getName().c_str())->RelatedColumn = 

  }

  pTable->Keys->Append(_variant_t((IDispatch *)pKey),ADOX::adKeyPrimary,vOptional,L"",L"");

  pCatalog->Tables->Refresh();
}

void te::ado::DataSetTypePersistence::drop(te::da::ForeignKey* /*fk*/)
{
  throw Exception(TR_ADO("Not implemented yet!"));
}

void te::ado::DataSetTypePersistence::add(te::da::DataSetType* /*dt*/, te::da::CheckConstraint* /*cc*/)
{
  throw Exception(TR_ADO("Not implemented yet!"));
}

void te::ado::DataSetTypePersistence::drop(te::da::CheckConstraint* /*cc*/)
{
  throw Exception(TR_ADO("Not implemented yet!"));
}

void te::ado::DataSetTypePersistence::create(te::da::Sequence* /*sequence*/)
{
  throw Exception(TR_ADO("Not implemented yet!"));
}

void te::ado::DataSetTypePersistence::drop(te::da::Sequence* /*sequence*/)
{
  throw Exception(TR_ADO("Not implemented yet!"));
}

te::da::DataSourceTransactor* te::ado::DataSetTypePersistence::getTransactor() const
{
  return m_t;
}
