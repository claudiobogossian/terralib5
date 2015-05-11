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

#ifndef __TERRALIB_EXAMPLE_HIGHLIGHTEDINFO_H
#define __TERRALIB_EXAMPLE_HIGHLIGHTEDINFO_H

//! Qt include files
#include <QWidget>

//! STL include files
#include <set>

//! Forward declarations
class QLineEdit;
class QAction;

namespace te
{
  namespace qt
  {
    namespace widgets
    {
      class TabularViewer;
    }
  }
}

class HighlightedInfo : public QWidget
{
  Q_OBJECT

public:

  HighlightedInfo(te::qt::widgets::TabularViewer* v, QWidget* parent=0);

protected slots:

  void addHighlight(QAction* action);

  void addQueriedClicked();

signals:

  void queryItems(const std::set<std::string>&);

  void updateHighlight(const int&, const std::set<std::string>&);

protected:

  QLineEdit* m_queried;
  QLineEdit* m_starred;
};

#endif //__TERRALIB_EXAMPLE_HIGHLIGHTEDINFO_H
