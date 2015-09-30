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
    \file FileEdit.cpp

    \brief

    \ingroup layout
*/

// TerraLib
#include "FilePathManager.h"

te::layout::FilePathManager::FilePathManager(QObject *parent /*= 0*/)
  :QtAbstractPropertyManager(parent)
{

}

te::layout::FilePathManager::~FilePathManager()
{

}

QString te::layout::FilePathManager::value(const QtProperty *property) const
{
  if (!m_values.contains(property))
    return QString();
  return m_values[property].value;
}

QString te::layout::FilePathManager::filter(const QtProperty *property) const
{
  if (!m_values.contains(property))
    return QString();
  return m_values[property].filter;
}

void te::layout::FilePathManager::setValue(QtProperty *property, const QString &val)
{
  if (!m_values.contains(property))
    return;

  Data data = m_values[property];

  if (data.value == val)
    return;

  data.value = val;

  m_values[property] = data;

  emit propertyChanged(property);
  emit valueChanged(property, data.value);
}

void te::layout::FilePathManager::setFilter(QtProperty *property, const QString &fil)
{
  if (!m_values.contains(property))
        return;

  Data data = m_values[property];

  if (data.filter == fil)
    return;

  data.filter = fil;

  m_values[property] = data;

  emit filterChanged(property, data.filter);
}

QString te::layout::FilePathManager::valueText(const QtProperty *property) const
{
  return value(property);
}

void te::layout::FilePathManager::initializeProperty(QtProperty *property)
{
  m_values[property] = Data();
}

void te::layout::FilePathManager::uninitializeProperty(QtProperty *property)
{
  m_values.remove(property);
}

