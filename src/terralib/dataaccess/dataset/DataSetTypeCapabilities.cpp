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
  \file terralib/dataaccess/dataset/DataSetTypeCapabilities.cpp

  \brief A class that informs what kind of constraint and index is supported by a given data source.
*/

// TerraLib
#include "DataSetTypeCapabilities.h"

te::da::DataSetTypeCapabilities::DataSetTypeCapabilities()
  : m_supportPrimaryKey(false),
    m_supportUniqueKey(false),
    m_supportForeignKey(false),
    m_supportSequence(false),
    m_supportCheckConstraints(false),
    m_supportIndex(false),
    m_supportRTreeIndex(false),
    m_supportBTreeIndex(false),
    m_supportHashIndex(false),
    m_supportQuadTreeIndex(false),
    m_supportAddColumn(false),
    m_supportRemoveColumn(false),
    m_supportDataEdition(false)
{
}

te::da::DataSetTypeCapabilities::~DataSetTypeCapabilities()
{
}

bool te::da::DataSetTypeCapabilities::supportsPrimaryKey() const
{
  return m_supportPrimaryKey;
}

void te::da::DataSetTypeCapabilities::setSupportPrimaryKey(const bool& support)
{
  m_supportPrimaryKey = support;
}

bool te::da::DataSetTypeCapabilities::supportsUniqueKey() const
{
  return m_supportUniqueKey;
}

void te::da::DataSetTypeCapabilities::setSupportUniqueKey(const bool& support)
{
  m_supportUniqueKey = support;
}

bool te::da::DataSetTypeCapabilities::supportsForeignKey() const
{
  return m_supportForeignKey;
}

void te::da::DataSetTypeCapabilities::setSupportForeingKey(const bool& support)
{
  m_supportForeignKey = support;
}

bool te::da::DataSetTypeCapabilities::supportsSequence() const
{
  return m_supportSequence;
}

void te::da::DataSetTypeCapabilities::setSupportSequence(const bool& support)
{
  m_supportSequence = support;
}

bool te::da::DataSetTypeCapabilities::supportsCheckConstraints() const
{
  return m_supportCheckConstraints;
}

void te::da::DataSetTypeCapabilities::setSupportCheckConstraints(const bool& support)
{
  m_supportCheckConstraints = support;
}

bool te::da::DataSetTypeCapabilities::supportsIndex() const
{
  return m_supportIndex;
}

void te::da::DataSetTypeCapabilities::setSupportIndex(const bool& support)
{
  m_supportIndex = support;
}

bool te::da::DataSetTypeCapabilities::supportsRTreeIndex() const
{
  return m_supportRTreeIndex;
}

void te::da::DataSetTypeCapabilities::setSupportRTreeIndex(const bool& support)
{
  m_supportRTreeIndex = support;
}

bool te::da::DataSetTypeCapabilities::supportsBTreeIndex() const
{
  return m_supportBTreeIndex;
}

void te::da::DataSetTypeCapabilities::setSupportBTreeIndex(const bool& support)
{
  m_supportBTreeIndex = support;
}

bool te::da::DataSetTypeCapabilities::supportsHashIndex() const
{
  return m_supportHashIndex;
}

void te::da::DataSetTypeCapabilities::setSupportHashIndex(const bool& support)
{
  m_supportHashIndex = support;
}

bool te::da::DataSetTypeCapabilities::supportsQuadTreeIndex() const
{
  return m_supportQuadTreeIndex;
}

void te::da::DataSetTypeCapabilities::setSupportQuadTreeIndex(const bool& support)
{
  m_supportQuadTreeIndex = support;
}

void te::da::DataSetTypeCapabilities::setSupportAll()
{
  m_supportPrimaryKey = true;
  m_supportUniqueKey = true;
  m_supportForeignKey = true;
  m_supportSequence = true;
  m_supportCheckConstraints = true;
  m_supportIndex = true;
  m_supportRTreeIndex = true;
  m_supportBTreeIndex = true;
  m_supportHashIndex = true;
  m_supportQuadTreeIndex = true;
  m_supportAddColumn = true;
  m_supportRemoveColumn = true;
  m_supportDataEdition = true;
}

bool te::da::DataSetTypeCapabilities::supportsAddColumn() const
{
  return m_supportAddColumn;
}

void te::da::DataSetTypeCapabilities::setSupportAddColumn(const bool &support)
{
  m_supportAddColumn = support;
}

bool te::da::DataSetTypeCapabilities::supportsRemoveColumn() const
{
  return m_supportRemoveColumn;
}

void te::da::DataSetTypeCapabilities::setSupportRemoveColumn(const bool &support)
{
  m_supportRemoveColumn = support;
}

bool te::da::DataSetTypeCapabilities::supportsDataEdition() const
{
  return m_supportDataEdition;
}

void te::da::DataSetTypeCapabilities::setSupportDataEdition(const bool &support)
{
  m_supportDataEdition = support;
}
