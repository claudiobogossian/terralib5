/*  Copyright (C) 2001-2011 National Institute For Space Research (INPE) - Brazil.

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
  \file DataGrid.h

  \brief It displays the data set contents in a table view.
 */

#ifndef __TERRALIB_EXAMPLE_DATAGRID_H
#define __TERRALIB_EXAMPLE_DATAGRID_H

// TerraLib
#include "terralib/qt/widgets/Config.h"

// Qt
#include <QDialog>
#include <ui_DataGrid.h>

// TerraLib
namespace te
{
  namespace da
  {
    class DataSource;
    class DataSet;
    class DataSetType;
    class DataSourceCatalogLoader;
    class DataSourceTransactor;
  }

  namespace map { class DataGridOperation; }

  namespace qt
  {
    namespace widgets
    {
      class DataGridModel;
      class DataGridView;
    }
  }
}


class DataGrid : public QDialog, public Ui_DataGridDialog
{
  Q_OBJECT

  public:
    DataGrid(QWidget* parent = 0);

    ~DataGrid();

  private slots:
    void serverComboBoxChanged(const QString& server);
    void okPushButtonClicked();

  protected:
    void closeEvent(QCloseEvent* e); 
      
  private:
    te::da::DataSource* m_ds;
    te::da::DataSourceTransactor* m_transactor;
    te::da::DataSourceCatalogLoader* m_catalogLoader;

    te::map::DataGridOperation* m_dataGridOp;

    te::qt::widgets::DataGridView* m_opTableView;

    te::da::DataSetType* m_dataSetType;
    te::da::DataSet* m_dataSet;

    te::qt::widgets::DataGridModel* m_dataGridModel;
};

#endif  // __TERRALIB_EXAMPLE_DATAGRID_H

  
