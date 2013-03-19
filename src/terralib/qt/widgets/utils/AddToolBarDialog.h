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
  \file AddToolBarDialog.h

  \brief Defines an API for creating customized tool bars.
*/

#ifndef __TERRALIB_QT_WIDGETS_INTERNAL_ADDTOOLBARDIALOG_H
#define __TERRALIB_QT_WIDGETS_INTERNAL_ADDTOOLBARDIALOG_H

#include "../Config.h"

// Qt
#include <QtGui/QDialog>

//Forward declaration
class QToolBar;
class QMenuBar;

namespace Ui
{
  class AddToolBarDialogForm;
}

namespace te
{
  namespace qt
  {
    namespace widgets
    {
      //Forward declaration
      class MenuBarModel;

      class TEQTWIDGETSEXPORT AddToolBarDialog : public QDialog
      {
        Q_OBJECT

        public:

          AddToolBarDialog (QWidget* parent=0);

          ~AddToolBarDialog();

          void setAvailableActions(QMenuBar* bar);

          QToolBar* getCreatedToolBar() const;

        protected slots:

          void onOkButtonPressed();

        protected:

          MenuBarModel* m_model;
  
        private:

          Ui::AddToolBarDialogForm* m_ui;
      };
    }
  }
}


#endif //__TERRALIB_QT_WIDGETS_INTERNAL_ADDTOOLBARDIALOG_H
