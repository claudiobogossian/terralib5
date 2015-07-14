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

/*!
  \file terralib/qt/widgets/table/AlterDataDialog.h

  \brief A Qt dialog for reset data of a column in the table.
*/

#ifndef __TERRALIB_QT_WIDGETS_INTERNAL_ALTERDATADIALOG_H
#define __TERRALIB_QT_WIDGETS_INTERNAL_ALTERDATADIALOG_H

#include "../Config.h"

// Qt
#include <QDialog>

// STL
#include <memory>

// Forward declarations
namespace Ui
{
  class AlterDataDialogForm;
}

namespace te
{
  namespace qt
  {
    namespace widgets
    {
      class TEQTWIDGETSEXPORT AlterDataDialog : public QDialog
      {
        Q_OBJECT

        public :

          AlterDataDialog(QWidget* parent=0);

          ~AlterDataDialog();

          void setSelectedColumn(const QString& colName);

          void setDataColumns(const std::vector<QString>& cols);

          void setHelpLabelText(const QString& text);

          bool alterAllData();

          QString getExpression();

        protected slots:

          void plusPressed();

          void minusPressed();

          void multiplyPressed();

          void divPressed();

          void columnChanged(const QString& col);

        private :

          std::auto_ptr<Ui::AlterDataDialogForm> m_ui; 
      };
    }
  }
}

#endif //__TERRALIB_QT_WIDGETS_INTERNAL_ALTERDATADIALOG_H
