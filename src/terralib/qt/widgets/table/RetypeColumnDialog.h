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
  \file terralib/qt/widgets/table/RenameColumnDialog.h

  \brief A Qt dialog for renaming columns into a table.
*/

#ifndef __TERRALIB_QT_WIDGETS_INTERNAL_RETYPECOLUMNDIALOG_H
#define __TERRALIB_QT_WIDGETS_INTERNAL_RETYPECOLUMNDIALOG_H

#include "../Config.h"

// Qt
#include <QDialog>

// STL
#include <memory>

// Forward declarations
namespace Ui
{
  class RetypeColumnDialogForm;
}

namespace te
{
  namespace dt
  {
    class Property;
  }

  namespace qt
  {
    namespace widgets
    {
      class TEQTWIDGETSEXPORT RetypeColumnDialog : public QDialog
      {
        Q_OBJECT

        public :

          RetypeColumnDialog(QWidget* parent=0);

          ~RetypeColumnDialog();

          void setTableName(const QString& name);

          void setType(const int& type);

          void setColumnName(const QString& name);

          void setColummnSize(const int& size);

          int getColumnType() const;

          int getColumnSize() const;

          std::auto_ptr<te::dt::Property> getProperty();

        private :

          std::auto_ptr<Ui::RetypeColumnDialogForm> m_ui; 
      };
    }
  }
}

#endif //__TERRALIB_QT_WIDGETS_INTERNAL_RETYPECOLUMNDIALOG_H
