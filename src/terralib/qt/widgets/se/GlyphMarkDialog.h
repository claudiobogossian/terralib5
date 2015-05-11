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
  \file terralib/qt/widgets/se/GlyphMarkDialog.h

  \brief A dialog used to build a glyph mark element.
*/

#ifndef __TERRALIB_QT_WIDGETS_SE_INTERNAL_GLYPHMARKDIALOG_H
#define __TERRALIB_QT_WIDGETS_SE_INTERNAL_GLYPHMARKDIALOG_H

// TerraLib
#include "../Config.h"

// Qt
#include <QDialog>

// STL
#include <memory>
#include <string>
#include <vector>

// Forward declaraion
namespace Ui { class GlyphMarkDialogForm; }

namespace te
{
// Forward declarations
  namespace se
  {
    class Mark;
  }

  namespace qt
  {
    namespace widgets
    {
// Forward declarations
      class GlyphMarkWidget;

      /*!
        \class GlyphMarkDialog

        \brief A dialog used to build a glyph mark element.
               If you want to use this dialog, you can use commands like:
               <code>
               te::se::Mark* m = te::qt::widgets::GlyphMarkWidget::getMark(0, parent, "Title");
               te::se::Mark* m = te::qt::widgets::GlyphMarkWidget::getMark(initialMark, parent, "Title");
               ...
               delete m;
               </code>
      */
      class TEQTWIDGETSEXPORT GlyphMarkDialog : public QDialog
      {
        public:

          /** @name Initializer Methods
           *  Methods related to instantiation and destruction.
           */
          //@{

          /*! \brief Constructs a glyph mark dialog which is a child of parent, with widget flags set to f. */
          GlyphMarkDialog(QWidget* parent = 0, Qt::WindowFlags f = 0);

          /*! \brief Destructor. */
          ~GlyphMarkDialog();

          //@}

        public:

          /*!
            \brief Pops up a modal mark dialog with the given window title, lets the user configure the mark,
                   and returns that mark. The mark is initially set to initial. The dialog is a child of parent.

            \param initial A initial mark element that will be used.
                           The dialog form will be update based on mark element parameters. It can be NULL.
            \param parent Dialog parent.
            \param title  Dialog title.

            \note The dialog will NOT take the ownership of the given initial mark.
            \note The caller will take the ownership of the returned mark.
            \note It returns a NULL mark element if the user cancels the dialog.
          */
          static te::se::Mark* getMark(const te::se::Mark* initial, QWidget* parent = 0, const QString& title = "");

          /*!
            \brief Gets the configured mark element.

            \return The configured mark element.
            
            \note The caller will take the ownership of the returned mark.
          */
          te::se::Mark* getMark() const;

        private:

          std::auto_ptr<Ui::GlyphMarkDialogForm> m_ui;         //!< Dialog form.
          te::qt::widgets::GlyphMarkWidget* m_glyphMarkWidget; //!< Glyph Mark Widget used to configure the mak element.
      };

    } // end namespace widgets
  }   // end namespace qt
}     // end namespace te

#endif  // __TERRALIB_QT_WIDGETS_SE_INTERNAL_GLYPHMARKDIALOG_H
