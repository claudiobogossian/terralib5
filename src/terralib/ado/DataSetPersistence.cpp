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
  \file terralib/ado/DataSetPersistence.cpp

  \brief DataSetPersistence implementation for ADO.
*/

// TerraLib
#include "../common/Translator.h"
#include "../dataaccess/dataset/DataSet.h"
#include "../dataaccess/dataset/DataSetType.h"
#include "../dataaccess/dataset/DataSetTypePersistence.h"
#include "../datatype/Property.h"
#include "../datatype/ByteArray.h"
#include "../geometry/Envelope.h"
#include "DataSetPersistence.h"
#include "DataSourceTransactor.h"
#include "Exception.h"
#include "Utils.h"

// ADO
#import "msado15.dll" \
  no_namespace rename("EOF", "EndOfFile")
#import "msadox.dll"

// STL
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

  add(dt->getName(), d, options, limit);
}

void te::ado::DataSetPersistence::remove(const std::string& datasetName)
{
  _ConnectionPtr adoConn = m_t->getADOConnection();

  _RecordsetPtr recset;
  TESTHR(recset.CreateInstance(__uuidof(Recordset)));

  recset->CursorLocation = adUseClient;
  recset->CursorType = adOpenStatic;
  recset->LockType = adLockBatchOptimistic;

  try
  {
    TESTHR(recset->Open(_bstr_t(datasetName.c_str()),
    _variant_t((IDispatch*)adoConn,true), adOpenKeyset, adLockOptimistic, adCmdTable));
  }
  catch(_com_error& e)
  {
    throw Exception(TR_ADO(e.Description()));
  }

  TESTHR(recset->Delete(adAffectCurrent));
}

void te::ado::DataSetPersistence::remove(const std::string& /*datasetName*/, const te::da::ObjectIdSet* /*oids*/)
{
  throw Exception(TR_ADO("Not implemented yet!"));
}

void te::ado::DataSetPersistence::add(const std::string& datasetName, te::da::DataSet* d, const std::map<std::string, std::string>& /*options*/, std::size_t /*limit*/)
{
  _ConnectionPtr adoConn = m_t->getADOConnection();

  _RecordsetPtr recset;
  TESTHR(recset.CreateInstance(__uuidof(Recordset)));

  try
  {
    TESTHR(recset->Open(_bstr_t(datasetName.c_str()),
    _variant_t((IDispatch*)adoConn,true), adOpenKeyset, adLockOptimistic, adCmdTable));
    
    do
    {
      TESTHR(recset->AddNew());

      for(std::size_t i = 0; i < d->getNumProperties(); ++i)
      {
      
        std::string pname = d->getPropertyName(i);
        int pType = d->getPropertyDataType(i);

        switch(pType)
        {
          case te::dt::CHAR_TYPE:
            recset->GetFields()->GetItem(pname.c_str())->Value = (_bstr_t)d->getChar(pname.c_str());
            break;

          //case te::dt::UCHAR_TYPE:

          case te::dt::INT16_TYPE:
            recset->GetFields()->GetItem(pname.c_str())->Value = (_variant_t)d->getInt16(pname.c_str());
            break;

          case te::dt::INT32_TYPE:
            recset->GetFields()->GetItem(pname.c_str())->Value = (_variant_t)d->getInt32(pname.c_str());
            break;

          case te::dt::INT64_TYPE:
            recset->GetFields()->GetItem(pname.c_str())->Value = (_variant_t)d->getInt64(pname.c_str());
            break;

          //case te::dt::NUMERIC_TYPE:
          //case te::dt::DATETIME_TYPE:
          case te::dt::FLOAT_TYPE:
            recset->GetFields()->GetItem(pname.c_str())->Value = (_variant_t)d->getFloat(pname.c_str());
            break;

          case te::dt::DOUBLE_TYPE:
            recset->GetFields()->GetItem(pname.c_str())->Value = (_variant_t)d->getDouble(pname.c_str());
            break;

          case te::dt::STRING_TYPE:
            recset->GetFields()->GetItem(pname.c_str())->Value = (_bstr_t)d->getString(pname.c_str()).c_str();
            break;
          
          case te::dt::BOOLEAN_TYPE:
            recset->GetFields()->GetItem(pname.c_str())->Value = (_variant_t)d->getBool(pname.c_str());
            break;

          case te::dt::BYTE_ARRAY_TYPE:
          {
            /*char * data = ((te::dt::ByteArray*)props[i])->getData();

            _variant_t var;
            te::ado::Blob2Variant(data, ((te::dt::ByteArray*)props[i])->bytesUsed(), var);

            recset->Fields->GetItem(props[i]->getName().c_str())->AppendChunk (var);

            break;*/
          }

          //case te::dt::ARRAY_TYPE:
          case te::dt::GEOMETRY_TYPE:
          {
            te::gm::Geometry* geometry = d->getGeometry(pname);
            const te::gm::Envelope* env = geometry->getMBR();
            
            recset->GetFields()->GetItem("lower_x")->Value = (_variant_t)env->m_llx;
            recset->GetFields()->GetItem("lower_y")->Value = (_variant_t)env->m_lly;
            recset->GetFields()->GetItem("upper_x")->Value = (_variant_t)env->m_urx;
            recset->GetFields()->GetItem("upper_y")->Value = (_variant_t)env->m_ury;

            _variant_t var;
            Convert2Ado(d->getGeometry(pname), var);

            recset->Fields->GetItem(pname.c_str())->AppendChunk (var);
            
            break;
          }

          default:
            throw te::ado::Exception(TR_ADO("The informed type could not be mapped to ADO type system!"));
            break;
          }
      }

      TESTHR(recset->Update());

    }
    while(d->moveNext());
  }
  catch(_com_error& e)
  {
    throw Exception(TR_ADO(e.Description()));
  }
}

void te::ado::DataSetPersistence::update(const std::string& /*datasetName*/,
                                         te::da::DataSet* /*dataset*/,
                                         const std::vector<std::size_t>& /*properties*/,
                                         const te::da::ObjectIdSet* /*oids*/,
                                         const std::map<std::string, std::string>& /*options*/,
                                         std::size_t /*limit*/)
{
  throw Exception(TR_ADO("Not implemented yet!"));
}

te::da::DataSourceTransactor* te::ado::DataSetPersistence::getTransactor() const
{
  return m_t;
}
