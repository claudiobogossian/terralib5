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

#ifndef __TERRALIB_EXAMPLE_TABULARVIEWEREX_H
#define __TERRALIB_EXAMPLE_TABULARVIEWEREX_H

//!Qt include files
#include <QMainWindow>

namespace te
{
  namespace da
  {
    //! Forward declarations
    class DataSource;
  }

  namespace qt
  {
    namespace widgets
    {
    //! Forward declarations
      class TabularViewer;
      class FileChooser;
    }
  }
}
class SelectPKey;
class QTabWidget;

class TabularViewerEx : public QMainWindow
{
  Q_OBJECT

public:
  TabularViewerEx(QWidget* parent=0);

  ~TabularViewerEx();

protected slots:
  
  void updateViewer();

  void pkeysChanged(const std::vector<size_t>& ids);

  void setThemeName(const QString& tName);

  void changeThemeName();

protected:
  te::qt::widgets::FileChooser* m_fchooser;
  te::qt::widgets::TabularViewer* m_viewer;
  te::da::DataSource* m_dsrc;

  SelectPKey* m_pkey_sel;

  QTabWidget* m_tab;
};

#endif //__TERRALIB_EXAMPLE_TABULARVIEWEREX_H
