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
  \file Legend.h

  \brief It edits the legend associated to a layer.
 */

#ifndef __TERRALIB_EXAMPLE_LEGEND_H
#define __TERRALIB_EXAMPLE_LEGEND_H

// TerraLib
#include "terralib/qt/widgets/Config.h"

// Qt
#include <QDialog>
#include <QModelIndex>
#include <QPoint>

class QMenu;

// TerraLib
namespace te
{
  namespace da
  {
    class DataSource;
    class DataSourceTransactor;
  }

  namespace map
  {
    class FolderLayer;
  }

  namespace qt
  {
    namespace widgets
    {
      class LayerExplorer;
      class LayerExplorerModel;
    }
  }
}

class Legend : public QDialog
{
  Q_OBJECT

  public:
    Legend(QWidget* parent = 0);

    ~Legend();

  private slots:
    void contextMenuActivated(const QModelIndex& index, const QPoint& pos);
    void editLegendSlot();

  protected:
    void closeEvent(QCloseEvent* e); 
      
  private:
    te::da::DataSource* m_ds;
    te::da::DataSourceTransactor* m_t;

    te::map::FolderLayer* m_rootLayer;
    te::qt::widgets::LayerExplorerModel* m_layerModel;
    te::qt::widgets::LayerExplorer* m_layerView;

    QMenu* m_popupMenu;
};

#endif  // __TERRALIB_EXAMPLE_LEGEND_H
