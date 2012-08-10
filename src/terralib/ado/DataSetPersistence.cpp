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
  \file DataSetPersistence.cpp

  \brief DataSetPersistence implementation for ADO.
*/

// TerraLib
#include "../common/Translator.h"
#include "../dataaccess/dataset/DataSet.h"
#include "../dataaccess/dataset/DataSetType.h"
#include "../dataaccess/dataset/DataSetItem.h"
#include "../dataaccess/dataset/PrimaryKey.h"
#include "../dataaccess/dataset/UniqueKey.h"
#include "../geometry/WKBReader.h"
#include "../geometry/WKBWriter.h"
#include "../geometry/Geometry.h"
#include "../geometry/GeometryProperty.h"
#include "../datatype/ByteArray.h"
#include "../memory/DataSetItem.h"
#include "../dataaccess/dataset/DataSetTypePersistence.h"
#include "../datatype/Property.h"
#include "DataSetPersistence.h"
#include "DataSourceTransactor.h"
#include "Exception.h"

#include "Utils.h"

// ADO
#import "msado15.dll" \
    no_namespace rename("EOF", "EndOfFile")
#import "msadox.dll"

#include <vector>

inline void TESTHR(HRESULT x)
{
  if FAILED(x)
    _com_issue_error(x);
};

te::ado::DataSetPersistence::DataSetPersistence(DataSourceTransactor* parent)
  : m_t(parent)
{
}

te::ado::DataSetPersistence::~DataSetPersistence()
{
}

void te::ado::DataSetPersistence::create(te::da::DataSetType* dt, te::da::DataSet* d, const std::map<std::string, std::string>& options, std::size_t limit)
{
  te::da::DataSetTypePersistence* dtp = m_t->getDataSetTypePersistence();

  dtp->create(dt);

  add(dt, d);
}

void te::ado::DataSetPersistence::remove(const te::da::DataSetType* dt)
{

  _ConnectionPtr adoConn = m_t->getADOConnection();

  _RecordsetPtr recset;
  TESTHR(recset.CreateInstance(__uuidof(Recordset)));

  recset->CursorLocation = adUseClient;
  recset->CursorType = adOpenStatic;
  recset->LockType = adLockBatchOptimistic;

  TESTHR(recset->Open(_bstr_t(dt->getName().c_str()),
  _variant_t((IDispatch*)adoConn,true), adOpenKeyset, adLockOptimistic, adCmdTable));

  TESTHR(recset->Delete(adAffectCurrent));

  te::da::DataSet* ds = m_t->getDataSet(dt->getName());

}

void te::ado::DataSetPersistence::remove(const te::da::DataSetType* /*dt*/, te::da::DataSet* /*d*/)
{
  throw Exception(TR_ADO("Not implemented yet!"));
}

void te::ado::DataSetPersistence::remove(const te::da::DataSetType* /*dt*/, te::da::DataSetItem* /*item*/)
{
  throw Exception(TR_ADO("Not implemented yet!"));
}

void te::ado::DataSetPersistence::remove(const te::da::DataSetType* /*dt*/, te::da::DataSet* /*d*/, std::size_t /*limit*/)
{
  throw Exception(TR_ADO("Not implemented yet!"));
}

void te::ado::DataSetPersistence::add(const te::da::DataSetType* dt, te::da::DataSet* d, std::size_t /*limit*/)
{
  do
  {
    add(dt, d->getItem());
  }
  while(d->moveNext());
}

void te::ado::DataSetPersistence::add(const te::da::DataSetType* dt, te::da::DataSetItem* item)
{
  _ConnectionPtr adoConn = m_t->getADOConnection();

  std::vector<te::dt::Property*> props = dt->getProperties();

  _RecordsetPtr recset;
  TESTHR(recset.CreateInstance(__uuidof(Recordset)));
  
  TESTHR(recset->Open(_bstr_t(dt->getName().c_str()),
  _variant_t((IDispatch*)adoConn,true), adOpenKeyset, adLockOptimistic, adCmdTable));

  TESTHR(recset->AddNew());

  for(size_t i = 0; i < props.size(); i++)
  {

    te::gm::GeometryProperty* geomProp = 0;
    geomProp = dt->getDefaultGeomProperty();

    if(geomProp && (geomProp->getName() == props[i]->getName()))
    {
      te::gm::Geometry* geo = item->getGeometry(props[i]->getName());

      long size = geo->getWkbSize();

      char* wkb = new char[size];

      te::gm::WKBWriter::write(geo, wkb);

      unsigned int newWkbSize = size+4;

      char* newWkb = new char[newWkbSize];

      memcpy(newWkb, wkb, size);

      unsigned int srid = geo->getSRID();

      memcpy(newWkb+size, &srid, 4);

      VARIANT var;
      BYTE *pByte;

      SAFEARRAY FAR* psa;
      SAFEARRAYBOUND rgsabound[1];
      rgsabound[0].lLbound = 0;
      rgsabound[0].cElements = newWkbSize;

      psa = SafeArrayCreate(VT_I1, 1, rgsabound);

      if(SafeArrayAccessData(psa,(void **)&pByte) == NOERROR)
        memcpy((LPVOID)pByte,(LPVOID)newWkb,newWkbSize);
      SafeArrayUnaccessData(psa);

      var.vt = VT_ARRAY | VT_UI1;
      var.parray = psa;

      recset->Fields->GetItem(props[i]->getName().c_str())->AppendChunk (var);

      continue;
    }

    int pType = props[i]->getType();

    switch(pType)
    {
      case te::dt::CHAR_TYPE:
        recset->GetFields()->GetItem(props[i]->getName().c_str())->Value = (_bstr_t)item->getChar(props[i]->getName().c_str());
        break;

      //case te::dt::UCHAR_TYPE:

      case te::dt::INT16_TYPE:
        recset->GetFields()->GetItem(props[i]->getName().c_str())->Value = (_variant_t)item->getInt16(props[i]->getName().c_str());
        break;

      case te::dt::INT32_TYPE:
        recset->GetFields()->GetItem(props[i]->getName().c_str())->Value = (_variant_t)item->getInt32(props[i]->getName().c_str());
        break;

      case te::dt::INT64_TYPE:
        recset->GetFields()->GetItem(props[i]->getName().c_str())->Value = (_variant_t)item->getInt64(props[i]->getName().c_str());
        break;

      //case te::dt::NUMERIC_TYPE:
      //case te::dt::DATETIME_TYPE:
      case te::dt::FLOAT_TYPE:
        recset->GetFields()->GetItem(props[i]->getName().c_str())->Value = (_variant_t)item->getFloat(props[i]->getName().c_str());
        break;

      case te::dt::DOUBLE_TYPE:
        recset->GetFields()->GetItem(props[i]->getName().c_str())->Value = (_variant_t)item->getDouble(props[i]->getName().c_str());
        break;

      case te::dt::STRING_TYPE:
        recset->GetFields()->GetItem(props[i]->getName().c_str())->Value = (_bstr_t)item->getString(props[i]->getName().c_str()).c_str();
        break;

      case te::dt::BOOLEAN_TYPE:
        recset->GetFields()->GetItem(props[i]->getName().c_str())->Value = (_variant_t)item->getBool(props[i]->getName().c_str());
        break;

      case te::dt::BYTE_ARRAY_TYPE:
      {
        char * data = ((te::dt::ByteArray*)props[i])->getData();

        VARIANT var;
        BYTE *pByte;
        long size = sizeof(data);

        SAFEARRAY FAR* psa;
        SAFEARRAYBOUND rgsabound[1];
        rgsabound[0].lLbound = 0;
        rgsabound[0].cElements = size;

        psa = SafeArrayCreate(VT_I1, 1, rgsabound);

        if(SafeArrayAccessData(psa,(void **)&pByte) == NOERROR)
          memcpy((LPVOID)pByte,(LPVOID)data,size);
        SafeArrayUnaccessData(psa);

        var.vt = VT_ARRAY | VT_UI1;
        var.parray = psa;

        recset->Fields->GetItem(props[i]->getName().c_str())->AppendChunk (var);

        break;
      }

      //case te::dt::ARRAY_TYPE:
     
      default:
        throw te::ado::Exception(TR_ADO("The informed type could not be mapped to ADO type system!"));
      break;
    }

  }

  recset->Update();

}

void te::ado::DataSetPersistence::update(const te::da::DataSetType* /*dt*/,
                      te::da::DataSet* /*dataset*/,
                      const std::vector<te::dt::Property*>& /*properties*/,
                      std::size_t /*limit*/)
{
  throw Exception(TR_ADO("Not implemented yet!"));
}

void te::ado::DataSetPersistence::update(const te::da::DataSetType* /*dt*/,
                    te::da::DataSet* /*oldD*/,
                    te::da::DataSet* /*newD*/,
                    const std::vector<te::dt::Property*>& /*properties*/,
                    std::size_t /*limit*/)
{
  throw Exception(TR_ADO("Not implemented yet!"));
}

void te::ado::DataSetPersistence::update(const te::da::DataSetType* /*dt*/,
                    te::da::DataSetItem* /*item*/,
                    const std::vector<te::dt::Property*>& /*properties*/)
{
  throw Exception(TR_ADO("Not implemented yet!"));
}

void te::ado::DataSetPersistence::update(const te::da::DataSetType* /*dt*/,
                    te::da::DataSetItem* /*oldItem*/,
                    te::da::DataSetItem* /*newItem*/,
                    const std::vector<te::dt::Property*>& /*properties*/)
{
  throw Exception(TR_ADO("Not implemented yet!"));
}

te::da::DataSourceTransactor* te::ado::DataSetPersistence::getTransactor() const
{
  return m_t;
}
                 
