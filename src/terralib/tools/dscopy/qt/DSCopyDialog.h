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
  \file terralib/tools/dscopy/qt/DSCopyDialog.h

  \brief Data Source Copy GUI
 */

#ifndef __TERRALIB_TOOLS_DSCOPY_DSCOPYDIALOG_H
#define __TERRALIB_TOOLS_DSCOPY_DSCOPYDIALOG_H

// Qt
#include <QtGui/QDialog>
#include <Ui_DSCopyDialog.h>

namespace te
{
  namespace tools
  {
    namespace dscopy
    {

      // Forward declarations
      class DSCopyDialogController;

      /*!
        \class DSCopyDialog

        \brief TerraLib Data Source Copy GUI
      */
      class DSCopyDialog : public QDialog, public Ui_DSCopyDialog
      {
        Q_OBJECT

        public:


          DSCopyDialog(QWidget * parent = 0, Qt::WindowFlags f = 0);

          ~DSCopyDialog();

        private slots:

          void helpPushButton_clicked();
          void backPushButton_clicked();
          void nextPushButton_clicked();
          void cancelPushButton_clicked();

          void inComboBox_activated(const QString&);
          void outComboBox_activated(const QString&);

          void inPathPushButton_clicked();
          void outPathPushButton_clicked();
          
        protected:
          void closeEvent(QCloseEvent* e);

        private:
          DSCopyDialogController* m_controller;
      };
    }
  }
}

#endif // __TERRALIB_TOOLS_DSCOPY_DSCOPYDIALOG_H