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
  \file LegendItem.cpp

  \brief A widget designed to show the information of a Legend in a tree oriented way.
 */

// TerraLib
#include "LegendItem.h"

// Qt
#include <QtGui/QMenu>
#include <QtGui/QWidget>

te::qt::widgets::LegendItem::LegendItem(QObject* parent)
  : QObject(parent)
{
}

te::qt::widgets::LegendItem::~LegendItem()
{
}

QVariant te::qt::widgets::LegendItem::data(int role) const
{
  /*if(role == Qt::DisplayRole)
  {
    if(m_refLayer->getTitle().empty())
      return QVariant("");

    return QVariant(m_refLayer->getTitle().c_str());
  }
  else if(role == Qt::CheckStateRole)
  {
    if(m_refLayer->isVisible() == true)
      return QVariant(Qt::Checked);
    else
      return QVariant(Qt::Unchecked);
  }*/

  return QVariant("Legenda");
}

QMenu* te::qt::widgets::LegendItem::getMenu(QWidget* parent) const
{
  return 0;
}

