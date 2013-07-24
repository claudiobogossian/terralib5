/*  Copyright (C) 2008-2013 National Institute For Space Research (INPE) - Brazil.

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
  \file terralib/ado2/Connection.cpp

  \brief A class that implements a connection to a ADO database.  
*/

// TerraLib
#include "../common/Translator.h"
#include "Connection.h"
#include "Exception.h"

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

void te::ado::Connection::execute(const std::string& command)
{
  try
  {
    m_conn->Execute(_bstr_t(command.c_str()),0, adCmdText);
  }
  catch(_com_error& e)
  {
    throw Exception(TR_ADO(e.Description()));
  }
}

te::ado::Connection::~Connection()
{
  if(m_conn)
    m_conn->Close();
}

te::ado::Connection::Connection(ConnectionPool* pool, const std::string& conninfo, const std::string& cencoding, bool inuse)
  : m_pool(pool),
    m_conn(0),
    m_inuse(inuse),
    m_lastuse(boost::posix_time::second_clock::local_time())
{
  if(conninfo.empty())
    return;

  try
  {
    m_conn.CreateInstance(__uuidof(::Connection));
    TESTHR(m_conn->Open(conninfo.c_str(), "", "", -1));
  }
  catch(_com_error& e)
  {
    throw Exception(TR_ADO(e.Description()));
  }

  long status = m_conn->GetState();

  if(status != adStateOpen)
  {
    boost::format errmsg(TR_ADO("It was not possible to create a connection to the given data source due to the following error: %1%."));

    errmsg = errmsg % m_conn->GetErrors()->GetItem(0)->GetDescription();

    m_conn->Close();

    m_conn = 0;

    throw Exception(errmsg.str());
  }
}
