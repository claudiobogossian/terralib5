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
  \file terralib/dataaccess/dataset/DataSetCapabilities.cpp

  \brief A class that informs what the dataset implementation of a given data source can perform.
*/

// TerraLib
#include "DataSetCapabilities.h"

te::da::DataSetCapabilities::DataSetCapabilities()
  : m_bidirectional(false),
    m_random(false),
    m_indexed(false),
    m_efficientMovePrevious(false),
    m_efficientMoveBeforeFirst(false),
    m_efficientMoveLast(false),
    m_efficientMoveAfterLast(false),
    m_efficientMove(false),
    m_efficientSize(false)
{
}

te::da::DataSetCapabilities::~DataSetCapabilities()
{
}

bool te::da::DataSetCapabilities::supportsBidirectionalTraversing() const
{
  return m_bidirectional;
}

void te::da::DataSetCapabilities::setSupportBidirectionalTraversing(const bool& support)
{
  m_bidirectional = support;
}

bool te::da::DataSetCapabilities::supportsRandomTraversing() const
{
  return m_random;
}

void te::da::DataSetCapabilities::setSupportRandomTraversing(const bool& support)
{
  m_random = support;
}

bool te::da::DataSetCapabilities::supportsIndexedTraversing() const
{
  return m_indexed;
}

void te::da::DataSetCapabilities::setSupportIndexedTraversing(const bool& support)
{
  m_indexed = support;
}

bool te::da::DataSetCapabilities::supportsEfficientMovePrevious() const
{
  return m_efficientMovePrevious;
}

void te::da::DataSetCapabilities::setSupportEfficientMovePrevious(const bool& support)
{
  m_efficientMovePrevious = support;
}

bool te::da::DataSetCapabilities::supportsEfficientMoveBeforeFirst() const
{
  return m_efficientMoveBeforeFirst;
}

void te::da::DataSetCapabilities::setSupportEfficientMoveBeforeFirst(const bool& support)
{
  m_efficientMoveBeforeFirst = support;
}

bool te::da::DataSetCapabilities::supportsEfficientMoveLast() const
{
  return m_efficientMoveLast;
}

void te::da::DataSetCapabilities::setSupportEfficientMoveLast(const bool& support)
{
  m_efficientMoveLast = support;
}

bool te::da::DataSetCapabilities::supportsEfficientMoveAfterLast() const
{
  return m_efficientMoveAfterLast;
}

void te::da::DataSetCapabilities::setSupportEfficientMoveAfterLast(const bool& support)
{
  m_efficientMoveAfterLast = support;
}

bool te::da::DataSetCapabilities::supportsEfficientMove() const
{
  return m_efficientMove;
}

void te::da::DataSetCapabilities::setSupportEfficientMove(const bool& support)
{
  m_efficientMove = support;
}

bool te::da::DataSetCapabilities::supportsEfficientDataSetSize() const
{
  return m_efficientSize;
}

void te::da::DataSetCapabilities::setSupportEfficientDataSetSize(const bool& support)
{
  m_efficientSize = support;
}

void te::da::DataSetCapabilities::setSupportAll()
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
}
