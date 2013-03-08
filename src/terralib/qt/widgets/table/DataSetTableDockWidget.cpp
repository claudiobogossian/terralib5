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
  \file DataSetTableDockWidget.cpp

  \brief A dock widget for table view widgets.
*/

// TerraLib
#include "DataSetTableDockWidget.h"
#include "DataSetTableModel.h"
#include "DataSetTableView.h"

te::qt::widgets::DataSetTableDockWidget::DataSetTableDockWidget(te::da::DataSet* dataset, QWidget* parent, Qt::WindowFlags flags)
  : QDockWidget(tr("Table"), parent, flags),
    m_view(0),
    m_model(0)
{
  m_view.reset(new DataSetTableView(this));
  m_model.reset(new DataSetTableModel(dataset, this));
  m_view->setModel(m_model.get());
  setWidget(m_view.get());
}

te::qt::widgets::DataSetTableDockWidget::~DataSetTableDockWidget()
{
  if(m_view.get())
    m_view->setModel(0);
}

te::qt::widgets::DataSetTableView* te::qt::widgets::DataSetTableDockWidget::getView() const
{
  return m_view.get();
}

te::qt::widgets::DataSetTableModel* te::qt::widgets::DataSetTableDockWidget::getModel() const
{
  return m_model.get();
}

