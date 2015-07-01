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
  \file terralib/qt/widgets/se/BasicFillDialog.h

  \brief A dialog used to build a basic fill element.
*/

#ifndef __TERRALIB_QT_WIDGETS_SE_INTERNAL_BASICFILLDIALOG_H
#define __TERRALIB_QT_WIDGETS_SE_INTERNAL_BASICFILLDIALOG_H

// TerraLib
#include "../Config.h"

// Qt
#include <QDialog>

// STL
#include <memory>

// Forward declaraion
namespace Ui { class BasicFillDialogForm; }

namespace te
{
// Forward declarations
  namespace se
  {
    class Fill;
  }

  namespace qt
  {
    namespace widgets
    {
// Forward declarations
      class BasicFillWidget;

      /*!
        \class BasicFillDialog

        \brief A dialog used to build a basic fill element.
               If you want to use this dialog, you can use commands like:
               <code>
               te::se::Fill* f = te::qt::widgets::BasicFillDialog::getFill(0, parent, "Title");
               te::se::Fill* f = te::qt::widgets::BasicFillDialog::getFill(initialFill, parent, "Title");
               ...
               delete f;
               </code>
      */
      class TEQTWIDGETSEXPORT BasicFillDialog : public QDialog
      {
        public:

          /** @name Initializer Methods
           *  Methods related to instantiation and destruction.
           */
          //@{

          /*! \brief Constructs a basic fill dialog which is a child of parent, with widget flags set to f. */
          BasicFillDialog(QWidget* parent = 0, Qt::WindowFlags f = 0);

          /*! \brief Destructor. */
          ~BasicFillDialog();

          //@}

        public:

          /*!
            \brief Pops up a modal basic fill dialog with the given window title, lets the user configure the fill, 
                   and returns that fill. The fill is initially set to initial. The dialog is a child of parent.

            \param initial A initial fill element that will be used. The dialog form will be update based on
                           fill element parameters. It can be NULL.
            \param parent Dialog parent.
            \param title  Dialog title.

            \note The dialog will NOT take the ownership of the given initial fill.
            \note The caller will take the ownership of the returned fill.
            \note It returns a NULL fill element if the user cancels the dialog.
          */
          static te::se::Fill* getFill(const te::se::Fill* initial, QWidget* parent = 0, const QString& title = "");

          /*!
            \brief Gets the configured fill element.

            \return The configured fill element.
            
            \note The caller will take the ownership of the returned fill.
          */
          te::se::Fill* getFill() const;

        private:

          std::auto_ptr<Ui::BasicFillDialogForm> m_ui;    //!< Dialog form.
          te::qt::widgets::BasicFillWidget* m_fillWidget; //!< Basic Fill Widget used to configure the fill element.
      }; 

    } // end namespace widgets
  }   // end namespace qt
}     // end namespace te

#endif  // __TERRALIB_QT_WIDGETS_SE_INTERNAL_BASICFILLDIALOG_H
