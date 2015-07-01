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
  \file terralib/qt/widgets/se/BasicStrokeDialog.h

  \brief A dialog used to build a basic stroke element.
*/

#ifndef __TERRALIB_QT_WIDGETS_SE_INTERNAL_BASICSTROKEDIALOG_H
#define __TERRALIB_QT_WIDGETS_SE_INTERNAL_BASICSTROKEDIALOG_H

// TerraLib
#include "../Config.h"

// Qt
#include <QDialog>

// STL
#include <memory>

// Forward declaraion
namespace Ui { class BasicStrokeDialogForm; }

namespace te
{
// Forward declarations
  namespace se
  {
    class Stroke;
  }

  namespace qt
  {
    namespace widgets
    {
// Forward declarations
      class BasicStrokeWidget;

      /*!
        \class BasicStrokeDialog

        \brief A dialog used to build a basic stroke element.
               If you want to use this dialog, you can use commands like:
               <code>
               te::se::Stroke* s = te::qt::widgets::BasicStrokeDialog::getStroke(0, parent, "Title");
               te::se::Stroke* s = te::qt::widgets::BasicStrokeDialog::getStroke(initialStroke, parent, "Title");
               ...
               delete s;
               </code>
      */
      class TEQTWIDGETSEXPORT BasicStrokeDialog : public QDialog
      {
        public:

          /** @name Initializer Methods
           *  Methods related to instantiation and destruction.
           */
          //@{

          /*! \brief Constructs a basic stroke dialog which is a child of parent, with widget flags set to f. */
          BasicStrokeDialog(QWidget* parent = 0, Qt::WindowFlags f = 0);

          /*! \brief Destructor. */
          ~BasicStrokeDialog();

          //@}

        public:

          /*!
            \brief Pops up a modal basic stroke dialog with the given window title, lets the user configure the stroke, 
                   and returns that stroke. The stroke is initially set to initial. The dialog is a child of parent. 

            \param initial A initial stroke element that will be used. The dialog form will be update based on 
                           stroke element parameters. It can be NULL.
            \param parent Dialog parent.
            \param title  Dialog title.

            \note The dialog will NOT take the ownership of the given initial stroke.
            \note The caller will take the ownership of the returned stroke.
            \note It returns a NULL stroke element if the user cancels the dialog.
          */
          static te::se::Stroke* getStroke(const te::se::Stroke* initial, QWidget* parent = 0, const QString& title = "");

          /*!
            \brief Gets the configured stroke element.

            \return The configured stroke element.
            
            \note The caller will take the ownership of the returned stroke.
          */
          te::se::Stroke* getStroke() const;

        private:

          std::auto_ptr<Ui::BasicStrokeDialogForm> m_ui;      //!< Dialog form.
          te::qt::widgets::BasicStrokeWidget* m_strokeWidget; //!< Basic Stroke Widget used to configure the stroke element.
      }; 

    } // end namespace widgets
  }   // end namespace qt
}     // end namespace te

#endif  // __TERRALIB_QT_WIDGETS_SE_INTERNAL_BASICSTROKEDIALOG_H
