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
#include "DataSetPersistence.h"
#include "DataSourceTransactor.h"
#include "Exception.h"

te::ado::DataSetPersistence::DataSetPersistence(DataSourceTransactor* parent)
  : m_t(parent)
{
}

te::ado::DataSetPersistence::~DataSetPersistence()
{
}

void te::ado::DataSetPersistence::create(te::da::DataSetType* /*dt*/, te::da::DataSet* /*d*/, const std::map<std::string, std::string>& /*options*/)
{
  throw Exception(TR_ADO("Not implemented yet!"));
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

void te::ado::DataSetPersistence::add(const te::da::DataSetType* /*dt*/, te::da::DataSet* /*d*/)
{
  throw Exception(TR_ADO("Not implemented yet!"));
}

void te::ado::DataSetPersistence::add(const te::da::DataSetType* /*dt*/, te::da::DataSetItem* /*item*/)
{
  throw Exception(TR_ADO("Not implemented yet!"));
}

void te::ado::DataSetPersistence::update(const te::da::DataSetType* /*dt*/,
                                              te::da::DataSet* /*dataset*/,
                                              const std::vector<te::dt::Property*>& /*properties*/)
{
  throw Exception(TR_ADO("Not implemented yet!"));
}

void te::ado::DataSetPersistence::update(const te::da::DataSetType* /*dt*/,
                                              te::da::DataSet* /*oldD*/,
                                              te::da::DataSet* /*newD*/,
                                              const std::vector<te::dt::Property*>& /*properties*/)
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
                 
