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
  \file CustomizeToolBarsDialog.h

  \brief Defines a widget for customize the tool bars of the system.
*/

#ifndef __TERRALIB_QT_WIDGETS_INTERNAL_CUSTOMIZETOOLBARSDIALOG_H
#define __TERRALIB_QT_WIDGETS_INTERNAL_CUSTOMIZETOOLBARSDIALOG_H

#include "../Config.h"

//Qt
#include <QtGui/QDialog>

// Forward declaration
class QToolBar;
class QMenuBar;
class QListWidgetItem;

namespace Ui
{
  class CustomizeToolBarsDialogForm;
}

namespace te
{
  namespace qt
  {
    namespace widgets
    {
      /*!
        \class CustomizeToolBarsDialog

        \brief Widget to customize toolbars of the system.
      */
      class TEQTWIDGETSEXPORT CustomizeToolBarsDialog : public QDialog
      {
        Q_OBJECT

        public:

          /*!
            \brief Constructor.

            \param mbar Menu bar containing all buttons of the system.

            \param tbars List of the toolbar of the system.

            \param parent Widget parent.
          */
          CustomizeToolBarsDialog(QMenuBar* mbar, std::vector<QToolBar*> tbars, QWidget* parent=0);

          /*!
            \brief Destructor.
          */
          ~CustomizeToolBarsDialog();

          /*!
            \brief

            \return
          */
          std::vector<QToolBar*> getCreatedToolBars()const ;

          /*!
            \brief

            \return
          */
          std::vector<QString> getRemovedBars() const;

        protected slots:

          /*!
            \brief
          */
          void onAddToolButtonClicked();

          /*!
            \brief
          */
          void onRemoveToolButtonClicked();

          /*!
            \brief Called when another toolbar is selected.
          */
          void onToolbarChanged (QListWidgetItem * current, QListWidgetItem * previous);

        protected:

          void closeEvent (QCloseEvent * event);

          std::vector<QToolBar*> m_bars;           //!< List of toolbars of the system.

          std::vector<QToolBar*> m_createdBars;    //!< List of toolbars of the system.

          std::vector<QString> m_removedBars;

          bool m_removing;

        private:

          Ui::CustomizeToolBarsDialogForm* m_ui;
      };
    }
  }
}

#endif //__TERRALIB_QT_WIDGETS_INTERNAL_CUSTOMIZETOOLBARSDIALOG_H
