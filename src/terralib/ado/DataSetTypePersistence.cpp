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
#include "../geometry/GeometryProperty.h"
#include "../geometry/Enums.h"
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

  _ConnectionPtr adoConn = m_t->getADOConnection();
  DataSourceCatalogLoader* loader = dynamic_cast<DataSourceCatalogLoader*>(m_t->getCatalogLoader());

  ADOX::_TablePtr pTable = 0;
  ADOX::_CatalogPtr pCatalog = 0;
    
  TESTHR(pTable.CreateInstance(__uuidof (ADOX::Table)));
  TESTHR(pCatalog.CreateInstance(__uuidof(ADOX::Catalog)));

  try
  {
    pCatalog->PutActiveConnection(variant_t((IDispatch *)adoConn));

    pTable->Name = dt->getName().c_str();

    TESTHR(pCatalog->Tables->Append(_variant_t((IDispatch*)pTable)));

  }
  catch(_com_error &e)
  {
    throw Exception(TR_ADO(e.ErrorMessage()));
  }

  std::size_t ncols = dt->size();
  for(std::size_t i = 0; i < ncols; ++i)
    add(dt, dt->getProperty(i));

  if(dt->getPrimaryKey())
    add(dt, dt->getPrimaryKey());
  
  te::gm::GeometryProperty* geomProp = 0;
  geomProp = dt->getDefaultGeomProperty();
  if(geomProp)
    te::ado::insertInGeometryColumns(adoConn, dt);

    /* METODO NA UTILS (insertInGeometryColumns)
    _RecordsetPtr recset;
    TESTHR(recset.CreateInstance(__uuidof(Recordset)));
  
    TESTHR(recset->Open(_bstr_t("geometry_columns"),
    _variant_t((IDispatch*)adoConn,true), adOpenKeyset, adLockOptimistic, adCmdTable));
      
    TESTHR(recset->AddNew());

    recset->GetFields()->GetItem("f_table_catalog")->Value = (_bstr_t)std::string("''").c_str();
    recset->GetFields()->GetItem("f_table_schema")->Value = (_bstr_t)std::string("public").c_str();
    recset->GetFields()->GetItem("f_table_name")->Value = (_bstr_t)dt->getName().c_str();
    recset->GetFields()->GetItem("f_geometry_column")->Value = (_bstr_t)geomProp->getName().c_str();
    recset->GetFields()->GetItem("coord_dimension")->Value = (_variant_t)coord_dimension;
    recset->GetFields()->GetItem("srid")->Value = (_variant_t)geomProp->getSRID();
    recset->GetFields()->GetItem("type")->Value = (_bstr_t)te::ado::GetGeometryName(geomProp->getGeometryType()).c_str();

    recset->Update();*/
  

}

void te::ado::DataSetTypePersistence::drop(te::da::DataSetType* dt)
{

  _ConnectionPtr adoConn = m_t->getADOConnection();

  ADOX::_CatalogPtr pCatalog = 0;

  TESTHR(pCatalog.CreateInstance(__uuidof(ADOX::Catalog)));

  try
  {
    pCatalog->PutActiveConnection(variant_t((IDispatch *)adoConn));
    
    TESTHR(pCatalog->Tables->Delete(dt->getName().c_str()));
  }
  catch(_com_error& e)
  {
    throw Exception(TR_ADO(e.Description()));
  }
}

void te::ado::DataSetTypePersistence::rename(te::da::DataSetType* dt, const std::string& newName)
{

  _ConnectionPtr adoConn = m_t->getADOConnection();

  ADOX::_CatalogPtr pCatalog = 0;

  TESTHR(pCatalog.CreateInstance(__uuidof(ADOX::Catalog)));

  try
  {
    pCatalog->Tables->GetItem(dt->getName().c_str())->PutName(newName.c_str());
  }
  catch(_com_error& e)
  {
    throw Exception(TR_ADO(e.Description()));
  }

}

void te::ado::DataSetTypePersistence::add(te::da::DataSetType* dt, te::dt::Property* p)
{

  _ConnectionPtr adoConn = m_t->getADOConnection();

  ADOX::_CatalogPtr pCatalog = 0;
  ADOX::_TablePtr pTable = 0;

  TESTHR(pCatalog.CreateInstance(__uuidof(ADOX::Catalog)));

  try
  {
    pCatalog->PutActiveConnection(variant_t((IDispatch *)adoConn));

    pTable = pCatalog->Tables->GetItem(dt->getName().c_str());
  }
  catch(_com_error& e)
  {
    throw Exception(TR_ADO(e.Description()));
  }

  te::ado::addAdoPropertyFromTerralib(pTable, p);

}

void te::ado::DataSetTypePersistence::drop(te::dt::Property* p)
{

  _ConnectionPtr adoConn = m_t->getADOConnection();

  ADOX::_CatalogPtr pCatalog = 0;
  ADOX::_TablePtr pTable = 0;

  TESTHR(pCatalog.CreateInstance(__uuidof(ADOX::Catalog)));

  try
  {
    pCatalog->PutActiveConnection(variant_t((IDispatch *)adoConn));

    pTable = pCatalog->Tables->GetItem(p->getParent()->getName().c_str());
    TESTHR(pTable->GetColumns()->Delete(p->getName().c_str()));
  }
  catch(_com_error& e)
  {
    throw Exception(TR_ADO(e.Description()));
  }

}

void te::ado::DataSetTypePersistence::rename(te::dt::Property* p, const std::string& newName) 
{
  _ConnectionPtr adoConn = m_t->getADOConnection();

  ADOX::_CatalogPtr pCatalog = 0;
  ADOX::_TablePtr pTable = 0;

  TESTHR(pCatalog.CreateInstance(__uuidof(ADOX::Catalog)));

  try
  {
    pCatalog->PutActiveConnection(variant_t((IDispatch *)adoConn));

    pTable = pCatalog->Tables->GetItem(p->getParent()->getName().c_str());
    ADOX::_ColumnPtr col = pTable->GetColumns()->GetItem(p->getName().c_str());
  
    col->PutName(newName.c_str());

    pCatalog->GetTables()->Refresh();
  }
  catch(_com_error& e)
  {
    throw Exception(TR_ADO(e.Description()));
  }
}

void te::ado::DataSetTypePersistence::update(te::dt::Property* oldP, te::dt::Property* newP)
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

  TESTHR(pCatalog.CreateInstance(__uuidof(ADOX::Catalog)));
  TESTHR(pKey.CreateInstance(__uuidof(ADOX::Key)));
  
  try
  {
    pCatalog->PutActiveConnection(variant_t((IDispatch *)adoConn));

    pTable = pCatalog->Tables->GetItem(dt->getName().c_str());

    pKey->Name = pTable->Name + "_pk";
    pKey->Type = ADOX::adKeyPrimary;

    for(size_t i = 0; i < pk->getProperties().size(); i++)
    {
      te::dt::Property* p = pk->getProperties()[i];
      TESTHR(pKey->Columns->Append(p->getName().c_str(), te::ado::Convert2Ado(p->getType()), 256));

    }

    TESTHR(pTable->Keys->Append(_variant_t((IDispatch *)pKey),ADOX::adKeyPrimary,vOptional,L"",L""));
    pCatalog->Tables->Refresh();
  }
  catch(_com_error& e)
  {
    throw Exception(TR_ADO(e.Description()));
  }

}

void te::ado::DataSetTypePersistence::drop(te::da::PrimaryKey* pk)
{
  _ConnectionPtr adoConn = m_t->getADOConnection();

  ADOX::_KeyPtr pKey = 0;
  ADOX::_TablePtr pTable = 0;
  ADOX::_CatalogPtr pCatalog = 0;

  try
  {
    pCatalog->PutActiveConnection(variant_t((IDispatch *)adoConn));

    pTable = pCatalog->Tables->GetItem(pk->getDataSetType()->getName().c_str());

    pKey = pTable->Keys->GetItem(pk->getName().c_str());

    TESTHR(pTable->GetKeys()->Delete(_variant_t((IDispatch *)pKey)));
  }
  catch(_com_error& e)
  {
    throw Exception(TR_ADO(e.Description()));
  }
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

  TESTHR(pCatalog.CreateInstance(__uuidof(ADOX::Catalog)));
  TESTHR(pKey.CreateInstance(__uuidof(ADOX::Key)));

  try
  {
    pCatalog->PutActiveConnection(variant_t((IDispatch *)adoConn));

    pTable = pCatalog->Tables->GetItem(dt->getName().c_str());

    pKey->Name = pTable->Name + "_uk";
    pKey->Type = ADOX::adKeyUnique;

    for(size_t i = 0; i < uk->getProperties().size(); i++)
    {
      te::dt::Property* p = uk->getProperties()[i];

      TESTHR(pKey->Columns->Append(p->getName().c_str(), te::ado::Convert2Ado(p->getType()), 256));
    }

    TESTHR(pTable->Keys->Append(_variant_t((IDispatch *)pKey),ADOX::adKeyUnique,vOptional,L"",L""));

    pCatalog->Tables->Refresh();
  }
  catch(_com_error& e)
  {
    throw Exception(TR_ADO(e.Description()));
  }
}

void te::ado::DataSetTypePersistence::drop(te::da::UniqueKey* uk)
{
  _ConnectionPtr adoConn = m_t->getADOConnection();

  ADOX::_KeyPtr pKey = 0;
  ADOX::_TablePtr pTable = 0;
  ADOX::_CatalogPtr pCatalog = 0;

  try
  {
    pCatalog->PutActiveConnection(variant_t((IDispatch *)adoConn));

    pTable = pCatalog->Tables->GetItem(uk->getDataSetType()->getName().c_str());

    pKey = pTable->Keys->GetItem(uk->getName().c_str());

    TESTHR(pTable->GetKeys()->Delete(_variant_t((IDispatch *)pKey)));
  }
  catch(_com_error& e)
  {
    throw Exception(TR_ADO(e.Description()));
  }
}

void te::ado::DataSetTypePersistence::add(te::da::DataSetType* dt, te::da::Index* index) 
{
  _ConnectionPtr adoConn = m_t->getADOConnection();

  ADOX::_IndexPtr pIndex = 0;
  ADOX::_TablePtr pTable = 0;
  ADOX::_CatalogPtr pCatalog = 0;

  TESTHR(pCatalog.CreateInstance(__uuidof(ADOX::Catalog)));
  TESTHR(pIndex.CreateInstance(__uuidof(ADOX::Index)));

  try
  {
    pCatalog->PutActiveConnection(variant_t((IDispatch *)adoConn));

    pTable = pCatalog->Tables->GetItem(dt->getName().c_str());

    pIndex->Name = index->getName().c_str();

    std::vector<te::dt::Property*> idxProps = index->getProperties();
    for(size_t i = 0; i < idxProps.size(); i++)
    {
      long size = 0;
      if(idxProps[i]->getType() == te::dt::STRING_TYPE)
        size = (long)((te::dt::StringProperty*)idxProps[i])->size();

      TESTHR(pIndex->Columns->Append(idxProps[i]->getName().c_str(), te::ado::Convert2Ado(idxProps[i]->getType()), size));
    }

    TESTHR(pTable->Indexes->Append(_variant_t((IDispatch *)pIndex)));
  }
  catch(_com_error& e)
  {
    throw Exception(TR_ADO(e.Description()));
  }

}

void te::ado::DataSetTypePersistence::add(te::da::DataSetType* /*dt*/, te::da::Index* /*index*/, 
  const std::map<std::string, std::string>& /*options*/)
{
  throw Exception(TR_ADO("Not implemented yet!"));
}

void te::ado::DataSetTypePersistence::drop(te::da::Index* index)
{
  _ConnectionPtr adoConn = m_t->getADOConnection();

  ADOX::_IndexPtr pIndex = 0;
  ADOX::_TablePtr pTable = 0;
  ADOX::_CatalogPtr pCatalog = 0;

  TESTHR(pCatalog.CreateInstance(__uuidof(ADOX::Catalog)));
  TESTHR(pIndex.CreateInstance(__uuidof(ADOX::Index)));

  try
  {
    pCatalog->PutActiveConnection(variant_t((IDispatch *)adoConn));

    pTable = pCatalog->Tables->GetItem(index->getDataSetType()->getName().c_str());

    pIndex = pTable->GetIndexes()->GetItem(index->getName().c_str());

    TESTHR(pTable->Indexes->Delete(_variant_t((IDispatch *)pIndex)));
  }
  catch(_com_error& e)
  {
    throw Exception(TR_ADO(e.Description()));
  }
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

  try
  {
    pCatalog->PutActiveConnection(variant_t((IDispatch *)adoConn));

    pTable = pCatalog->Tables->GetItem(dt->getName().c_str());

    TESTHR(pKey.CreateInstance(__uuidof(ADOX::Key)));

    pKey->Name = pTable->Name + "_fk";
    pKey->Type = ADOX::adKeyForeign;
    pKey->RelatedTable = fk->getReferencedDataSetType()->getName().c_str();  

    for(size_t i = 0; i < fk->getProperties().size(); i++)
    {

      te::dt::Property* p = fk->getProperties()[i];

      TESTHR(pKey->Columns->Append(p->getName().c_str(), te::ado::Convert2Ado(p->getType()), 256));
      pKey->Columns->GetItem(p->getName().c_str())->RelatedColumn = fk->getReferencedProperties()[i]->getName().c_str();

    }

    TESTHR(pTable->Keys->Append(_variant_t((IDispatch *)pKey),ADOX::adKeyPrimary,vOptional,L"",L""));
  
    pCatalog->Tables->Refresh();
  }
  catch(_com_error& e)
  {
    throw Exception(TR_ADO(e.Description()));
  }
}

void te::ado::DataSetTypePersistence::drop(te::da::ForeignKey* fk)
{
  _ConnectionPtr adoConn = m_t->getADOConnection();

  ADOX::_KeyPtr pKey = 0;
  ADOX::_TablePtr pTable = 0;
  ADOX::_CatalogPtr pCatalog = 0;

  try
  {

    pCatalog->PutActiveConnection(variant_t((IDispatch *)adoConn));

    pTable = pCatalog->Tables->GetItem(fk->getDataSetType()->getName().c_str());

    pKey = pTable->Keys->GetItem(fk->getName().c_str());

    TESTHR(pTable->GetKeys()->Delete(_variant_t((IDispatch *)pKey)));
  }
  catch(_com_error& e)
  {
    throw Exception(TR_ADO(e.Description()));
  }
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
