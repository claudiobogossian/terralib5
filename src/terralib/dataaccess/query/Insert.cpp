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
  \file terralib/dataaccess/query/Insert.cpp

  \brief A Insert can be used to add information in a table.
*/

// TerraLib
#include "Insert.h"

te::da::Insert::Insert(DataSetName* d, Fields* f, Select* s)
  : m_dsName(d),
    m_fields(f),
    m_select(s)
{
}

te::da::Insert::Insert(const DataSetName& d, const Fields& f, const Select& s)
  : m_dsName(static_cast<DataSetName*>(d.clone())),
    m_fields(0),
    m_select(static_cast<Select*>(s.clone()))
{
  m_fields.reset(new Fields(f));
}

te::da::Insert::Insert(DataSetName* d, Select* s)
  : m_dsName(d),
    m_select(s)
{
}

te::da::Insert::Insert(const DataSetName& d, const Select& s)
  : m_dsName(static_cast<DataSetName*>(d.clone())),
    m_select(static_cast<Select*>(s.clone()))
{
}

te::da::Insert::Insert(const Insert& rhs)
  :
    m_dsName(0),
    m_fields(0),
    m_select(0)
{
  m_dsName.reset(rhs.m_dsName.get() ? new DataSetName(*rhs.m_dsName) : 0);
  m_fields.reset(rhs.m_fields.get() ? new Fields(*rhs.m_fields) : 0);
  m_select.reset(rhs.m_select.get() ? new Select(*rhs.m_select) : 0);
}

te::da::Insert::~Insert()
{
}

te::da::Insert& te::da::Insert::operator=(const Insert& rhs)
{
  if(this != &rhs)
  {
    m_dsName.reset(rhs.m_dsName.get() ? new DataSetName(*rhs.m_dsName) : 0);
    m_fields.reset(rhs.m_fields.get() ? new Fields(*rhs.m_fields) : 0);
    m_select.reset(rhs.m_select.get() ? new Select(*rhs.m_select) : 0);
  }

  return *this;
}

te::da::Query* te::da::Insert::clone() const
{
  return new Insert(*this);
}

te::da::DataSetName* te::da::Insert::getDataSetName() const
{
  return m_dsName.get();
}

void te::da::Insert::setDataSetName(DataSetName* d)
{
  m_dsName.reset(d);
}

te::da::Fields* te::da::Insert::getFields() const
{
  return m_fields.get();
}

void te::da::Insert::setFields(Fields* f)
{
  m_fields.reset(f);
}

te::da::Select* te::da::Insert::getSelect() const
{
  return m_select.get();
}

void te::da::Insert::setSelect(Select* s)
{
  m_select.reset(s);
}

