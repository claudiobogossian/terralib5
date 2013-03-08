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
  \file DataSetTableView.cpp

  \brief A table view for a dataset.
*/

// TerraLib
#include "DataSetTableModel.h"
#include "DataSetTableView.h"

te::qt::widgets::DataSetTableView::DataSetTableView(QWidget* parent)
  : QTableView(parent)
{
  setAlternatingRowColors(true);
}

te::qt::widgets::DataSetTableView::~DataSetTableView()
{
}

void te::qt::widgets::DataSetTableView::set(te::da::DataSet* dataset)
{
  DataSetTableModel* m = new DataSetTableModel(dataset, this);
 
  this->setModel(m);
}

