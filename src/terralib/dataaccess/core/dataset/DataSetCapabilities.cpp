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
  \file terralib/dataaccess/core/dataset/DataSetCapabilities.cpp

  \brief A class that informs what the dataset implementation of a given data source can perform.
*/

// TerraLib
#include "DataSetCapabilities.h"

te::da::core::DataSetCapabilities::DataSetCapabilities()
  : m_bidirectional(false),
    m_random(false),
    m_indexed(false),
    m_efficientMovePrevious(false),
    m_efficientMoveBeforeFirst(false),
    m_efficientMoveLast(false),
    m_efficientMoveAfterLast(false),
    m_efficientMove(false),
    m_efficientSize(false),
    m_insertion(false),
    m_update(false),
    m_deletion(false)
{
}

te::da::core::DataSetCapabilities::~DataSetCapabilities()
{
}

bool te::da::core::DataSetCapabilities::supportsBidirectionalTraversing() const
{
  return m_bidirectional;
}

void te::da::core::DataSetCapabilities::setSupportBidirectionalTraversing(const bool support)
{
  m_bidirectional = support;
}

bool te::da::core::DataSetCapabilities::supportsRandomTraversing() const
{
  return m_random;
}

void te::da::core::DataSetCapabilities::setSupportRandomTraversing(const bool support)
{
  m_random = support;
}

bool te::da::core::DataSetCapabilities::supportsIndexedTraversing() const
{
  return m_indexed;
}

void te::da::core::DataSetCapabilities::setSupportIndexedTraversing(const bool support)
{
  m_indexed = support;
}

bool te::da::core::DataSetCapabilities::supportsEfficientMovePrevious() const
{
  return m_efficientMovePrevious;
}

void te::da::core::DataSetCapabilities::setSupportEfficientMovePrevious(const bool support)
{
  m_efficientMovePrevious = support;
}

bool te::da::core::DataSetCapabilities::supportsEfficientMoveBeforeFirst() const
{
  return m_efficientMoveBeforeFirst;
}

void te::da::core::DataSetCapabilities::setSupportEfficientMoveBeforeFirst(const bool support)
{
  m_efficientMoveBeforeFirst = support;
}

bool te::da::core::DataSetCapabilities::supportsEfficientMoveLast() const
{
  return m_efficientMoveLast;
}

void te::da::core::DataSetCapabilities::setSupportEfficientMoveLast(const bool support)
{
  m_efficientMoveLast = support;
}

bool te::da::core::DataSetCapabilities::supportsEfficientMoveAfterLast() const
{
  return m_efficientMoveAfterLast;
}

void te::da::core::DataSetCapabilities::setSupportEfficientMoveAfterLast(const bool support)
{
  m_efficientMoveAfterLast = support;
}

bool te::da::core::DataSetCapabilities::supportsEfficientMove() const
{
  return m_efficientMove;
}

void te::da::core::DataSetCapabilities::setSupportEfficientMove(const bool support)
{
  m_efficientMove = support;
}

bool te::da::core::DataSetCapabilities::supportsEfficientDataSetSize() const
{
  return m_efficientSize;
}

void te::da::core::DataSetCapabilities::setSupportEfficientDataSetSize(const bool support)
{
  m_efficientSize = support;
}

bool te::da::core::DataSetCapabilities::supportsInsertion() const
{
  return m_insertion;
}

void te::da::core::DataSetCapabilities::setSupportInsertion(const bool support)
{
  m_insertion = support;
}

bool te::da::core::DataSetCapabilities::supportsUpdate() const
{
  return m_update;
}

void te::da::core::DataSetCapabilities::setSupportUpdate(const bool support)
{
  m_update = support;
}

bool te::da::core::DataSetCapabilities::supportsDeletion() const
{
  return m_deletion;
}

void te::da::core::DataSetCapabilities::setSupportDeletion(const bool support)
{
  m_deletion = support;
}

void te::da::core::DataSetCapabilities::setSupportAll()
{
  m_bidirectional = true;
  m_random = true;
  m_indexed = true;
  m_efficientMovePrevious = true;
  m_efficientMoveBeforeFirst = true;
  m_efficientMoveLast = true;
  m_efficientMoveAfterLast = true;
  m_efficientMove = true;
  m_efficientSize = true;
  m_insertion = true;
  m_update = true;
  m_deletion = true;
}
