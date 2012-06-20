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

void te::ado::DataSetPersistence::remove(const te::da::DataSetType* /*dt*/)
{
  throw Exception(TR_ADO("Not implemented yet!"));
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
  try
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
          std::string value = "";
          value = ((te::dt::ByteArray*)props[i])->toString();

          BYTE *pByte;
          VARIANT var;

          SAFEARRAY FAR* psa;
          SAFEARRAYBOUND rgsabound[1];
          rgsabound[0].lLbound = 0;	
          rgsabound[0].cElements =  value.size();

          // create a single dimensional byte array
          psa = SafeArrayCreate(VT_I1, 1, rgsabound);

          // set the data of the array with data in the edit box
          if(SafeArrayAccessData(psa,(void **)&pByte) == NOERROR)
            memcpy((LPVOID)pByte,(LPVOID)(char*)value.c_str(), value.size() * sizeof(char));
          SafeArrayUnaccessData(psa);
          var.vt = VT_ARRAY | VT_UI1;
          var.parray = psa;

          recset->Fields->GetItem(props[i]->getName().c_str())->AppendChunk (var);
          SafeArrayDestroy (var.parray);
        
          break;
        }

        //case te::dt::GEOMETRY_TYPE:
        //case te::dt::ARRAY_TYPE:
     
        default:
          throw te::ado::Exception(TR_ADO("The informed type could not be mapped to ADO type system!"));
        break;
      }

    }

    recset->Update();
  }
  catch(_com_error &e)
  {
    std::string description(e.Description());
    throw Exception(TR_ADO("ADO Driver Error: " + description));
  }
  catch(const std::exception& e)
  {
    std::string description(e.what());
    throw Exception(TR_ADO("ADO Driver Error: " +  description));
  }
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
                 
