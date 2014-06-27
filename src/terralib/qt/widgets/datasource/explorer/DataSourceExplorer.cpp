/*  Copyright (C) 2011-2011 National Institute For Space Research (INPE) - Brazil.

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
  \file terralib/qt/widgets/datasource/explorer/DataSourceExplorer.cpp

  \brief A widget designed to explore the data sources of an application.
*/

// TerraLib
#include "DataSourceExplorer.h"
#include "DataSourceToolBox.h"

// Qt
#include <QScrollArea>
#include <QVBoxLayout>

te::qt::widgets::DataSourceExplorer::DataSourceExplorer(QWidget* parent, Qt::WindowFlags flags)
  : QDockWidget(tr("Data Source Explorer"), parent, flags),
    m_toolBox(0)
{
  QScrollArea* scrollArea = new QScrollArea(this);

  m_toolBox = new DataSourceToolBox(this);

  scrollArea->setWidget(m_toolBox);

  scrollArea->setWidgetResizable(true);

  setWidget(scrollArea);
}

te::qt::widgets::DataSourceExplorer::~DataSourceExplorer()
{
}

te::qt::widgets::DataSourceToolBox* te::qt::widgets::DataSourceExplorer::getToolBox() const
{
  return m_toolBox;
}

