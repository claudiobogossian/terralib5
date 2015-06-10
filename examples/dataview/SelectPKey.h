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

#ifndef __TERRALIB_EXAMPLE_SELECTPKEY_H
#define __TERRALIB_EXAMPLE_SELECTPKEY_H

//Qt inclue files
#include <QWidget>

//Forward declarations
namespace Ui
{
  class SelectPKey;
}

namespace te
{
  namespace da
  {
    class DataSet;
  }
}

class SelectPKey : public QWidget
{
  Q_OBJECT
   
public:
  SelectPKey(QWidget* parent=0);

  ~SelectPKey();

  void updateColumns(te::da::DataSet* dset);

protected slots:

  void on_m_selectAllPushButton_clicked();

  void on_m_unselectAllPushButton_clicked();

  void on_m_updateKeysPushButton_clicked();

signals:

  void pkeysChanged(const std::vector<size_t>&);

private:
  Ui::SelectPKey* m_ui;
};

#endif //__TERRALIB_EXAMPLE_SELECTPKEY_H
