/*  Copyright (C) 2011-2012 National Institute For Space Research (INPE) - Brazil.

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
  \file terralib/qt/widgets/dataset/explorer/DataSetExplorer.cpp

  \brief A widget designed to explore the datasets of a data source.
*/

// TerraLib
#include "DataSetExplorer.h"
#include "DataSetTreeView.h"

// Qt
#include <QScrollArea>
#include <QVBoxLayout>

te::qt::widgets::DataSetExplorer::DataSetExplorer(QWidget* parent, Qt::WindowFlags flags)
  : QDockWidget(tr("Dataset Explorer"), parent, flags)
{
  //QScrollArea* scrollArea = new QScrollArea(this);

  m_treeView.reset(new DataSetTreeView(this));

  //scrollArea->setWidget(dtv);

  //scrollArea->setWidgetResizable(true);

  //setWidget(scrollArea);

  setWidget(m_treeView.get());
}

te::qt::widgets::DataSetExplorer::~DataSetExplorer()
{
  //QWidget* w = widget();

  //if(w == 0)
    //return;

  //QScrollArea* scrollArea = dynamic_cast<QScrollArea*>(w);

  //if(w == 0)
    //return;

  //w = scrollArea->widget();

  //if(w == 0)
    //return;

  //DataSetTreeView* tview = dynamic_cast<DataSetTreeView*>(w);

  //if(tview == 0)
    //return;

  //QAbstractItemModel* model = tview->model();

  //tview->setModel(0);

  //delete model;
}

void te::qt::widgets::DataSetExplorer::set(const te::da::DataSourceInfoPtr& ds)
{
  m_treeView->set(ds);
}

te::qt::widgets::DataSetTreeView* te::qt::widgets::DataSetExplorer::getTreeView() const
{
  return m_treeView.get();
}

