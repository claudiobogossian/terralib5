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
  \file terralib/qt/widgets/se/GraphicDialog.h

  \brief A dialog used to build a graphic element.
*/

#ifndef __TERRALIB_QT_WIDGETS_SE_INTERNAL_GRAPHICDIALOG_H
#define __TERRALIB_QT_WIDGETS_SE_INTERNAL_GRAPHICDIALOG_H

// TerraLib
#include "../Config.h"

// Qt
#include <QDialog>

// STL
#include <memory>
#include <string>
#include <vector>

// Forward declaraion
namespace Ui { class GraphicDialogForm; }

namespace te
{
// Forward declarations
  namespace se
  {
    class Graphic;
  }

  namespace qt
  {
    namespace widgets
    {
// Forward declarations
      class GraphicSelectorWidget;

      /*!
        \class GraphicDialog

        \brief A dialog used to build a graphic element.
               If you want to use this dialog, you can use commands like:
               <code>
               te::se::Graphic* g = te::qt::widgets::GraphicDialog::getGraphic(0, parent, "Title");
               ...
               delete g;
               </code>
      */
      class TEQTWIDGETSEXPORT GraphicDialog : public QDialog
      {
        public:

          /** @name Initializer Methods
           *  Methods related to instantiation and destruction.
           */
          //@{

          /*! \brief Constructs a graphic dialog which is a child of parent, with widget flags set to f. */
          GraphicDialog(QWidget* parent = 0, Qt::WindowFlags f = 0);

          /*! \brief Destructor. */
          ~GraphicDialog();

          //@}

        public:

          /*!
            \brief Pops up a modal graphic dialog with the given window title, lets the user configure the graphic, 
                   and returns that graphic. The graphic is initially set to initial. The dialog is a child of parent. 

            \param initial A initial graphic element that will be used.
                           The dialog form will be update based on graphic element parameters. It can be NULL.
            \param parent Dialog parent.
            \param title  Dialog title.

            \note The dialog will NOT take the ownership of the given initial graphic.
            \note The caller will take the ownership of the returned graphic.
            \note It returns a NULL graphic element if the user cancels the dialog.
          */
          static te::se::Graphic* getGraphic(const te::se::Graphic* initial, QWidget* parent = 0, const QString& title = "");

          /*!
            \brief Sets the graphic element.

            \return The graphic element.
            
            \note The dialog will NOT take the ownership of the given graphic.
          */
          void setGraphic(const te::se::Graphic* graphic);

          /*!
            \brief Gets the configured graphic element.

            \return The configured graphic element.
            
            \note The caller will take the ownership of the returned graphic.
          */
          te::se::Graphic* getGraphic() const;

          /*!
            \brief Gets the current graphic icon.

            \param size The icon size that will be generated.

            \return An icon that represents the graphic. i.e. a simple preview.
          */
          QIcon getGraphicIcon(const QSize& size);

        private:

          std::auto_ptr<Ui::GraphicDialogForm> m_ui; //!< Dialog form.
          GraphicSelectorWidget* m_graphicSelector;  //!< Graphic selector widget used to select the graphic.
      };

    } // end namespace widgets
  }   // end namespace qt
}     // end namespace te

#endif  // __TERRALIB_QT_WIDGETS_SE_INTERNAL_GRAPHICDIALOG_H
