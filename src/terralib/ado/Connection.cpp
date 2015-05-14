/*  Copyright (C) 2008 National Institute For Space Research (INPE) - Brazil.

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
  \file terralib/ado/Connection.cpp

  \brief A class that implements a connection to a ADO database.  
*/

// TerraLib
#include "../common/Exception.h"
#include "../common/Translator.h"
#include "Connection.h"

// STL
#include <cassert>
#include <string>

// Boost
#include <boost/format.hpp>

inline void TESTHR(HRESULT hr)
{
  if(FAILED(hr))
    _com_issue_error(hr);
}

te::ado::Connection::Connection(const std::string& conninfo)
  : m_conn(0)
{
  if(conninfo.empty())
    return;

  _bstr_t connStr = conninfo.c_str();

  try
  {
    TESTHR(::CoInitialize(0));

    TESTHR(m_conn.CreateInstance(__uuidof(::Connection)));
    TESTHR(m_conn->Open(connStr, "", "", -1));
  }
  catch(_com_error& e)
  {
    std::string err = TE_TR("ADO Connection: ");
    err += (LPCSTR)e.ErrorMessage();
    throw te::common::Exception(err);
  }

  long status = m_conn->GetState();

  if(status != adStateOpen)
  {
    boost::format errmsg(TE_TR("It was not possible to create a connection to the given data source due to the following error: %1%."));

    errmsg = errmsg % m_conn->GetErrors()->GetItem(0)->GetDescription();

    m_conn->Close();

    m_conn = 0;

    throw te::common::Exception(errmsg.str());
  }
}

te::ado::Connection::~Connection()
{
  if (m_conn)
    if (m_conn->GetState() == ::adStateOpen)
      m_conn->Close();

  ::CoUninitialize();
}

_RecordsetPtr te::ado::Connection::query(const std::string& query, bool connected)
{
  _RecordsetPtr recordset;

  TESTHR(recordset.CreateInstance(__uuidof(Recordset)));
  
  try
  {
    //if(connected)
      //recordset->Open(query.c_str(), _variant_t((IDispatch *)m_conn), adOpenDynamic, adLockReadOnly, adCmdText);
    //else
      recordset->Open(query.c_str(), _variant_t((IDispatch *)m_conn), adOpenStatic, adLockReadOnly, adCmdText);
  }
  catch(_com_error& e)
  {
    std::string err = TE_TR("ADO Connection: ");
    err += (LPCSTR)e.ErrorMessage();
    throw te::common::Exception(err);
  }

  return recordset;
}

void te::ado::Connection::execute(const std::string& command)
{
  try
  {
    m_conn->Execute(_bstr_t(command.c_str()),0, adCmdText);
  }
  catch(_com_error& e)
  {
    std::string err = TE_TR("ADO Connection: ");
    err += (LPCSTR)e.ErrorMessage();
    throw te::common::Exception(err);
  }
}

bool te::ado::Connection::isValid()
{
  return m_conn != 0;
}