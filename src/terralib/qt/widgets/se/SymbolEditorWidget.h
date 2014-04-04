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
  \file terralib/qt/widgets/se/SymbolEditorWidget.h

  \brief A widget used to edit symbols.
*/

#ifndef __TERRALIB_QT_WIDGETS_SE_INTERNAL_SYMBOLEDITORWIDGET_H
#define __TERRALIB_QT_WIDGETS_SE_INTERNAL_SYMBOLEDITORWIDGET_H

// TerraLib
#include "../../../se/Enums.h"
#include "../Config.h"
#include "Symbol.h"

// Qt
#include <QWidget>

// STL
#include <memory>
#include <vector>

// Forward declarations
class QStackedWidget;
namespace Ui { class SymbolEditorWidgetForm; }

namespace te
{
// Forward declarations
  namespace se
  {
    class Symbolizer;
  }

  namespace qt
  {
    namespace widgets
    {
// Forward declarations
      class SymbolPreviewWidget;
      class SymbolTableWidget;

      /*!
        \class SymbolEditorWidget

        \brief A widget used to edit symbols.
      */
      class TEQTWIDGETSEXPORT SymbolEditorWidget : public QWidget
      {
        Q_OBJECT

        public:

          /** @name Initializer Methods
           *  Methods related to instantiation and destruction.
           */
          //@{

          /*!
            \brief Constructs a symbol editor, which is a child of parent, with widget flags set to f. 

            \type The Symbol type that will be handled by the widget.
            \param parent The widget's parent.
          */
          SymbolEditorWidget(const SymbolType& type, QWidget* parent = 0);

          /*! \brief Destructor. */
          ~SymbolEditorWidget();

          //@}

        protected:

          /*! \brief Internal method to initialize the widget. */
          void initialize();

          /*! \brief Updates the widget form based on internal symbolizers elements. */
          void updateUi();

          /*! \brief Creates a new symbolizer element. */
          void createNewSymbolizer();

          /*! \brief Swap two internal symbolizers. */
          void swapSymbolizers(const int& indexFirst, const int& indexSecond);

        protected slots:

          void onAddToolButtonClicked();

          void onRemoveToolButtonClicked();

          void onUpToolButtonClicked();

          void onDownToolButtonClicked();

          void onSymbolizerChanged();

        private:

          std::auto_ptr<Ui::SymbolEditorWidgetForm> m_ui;    //!< Widget form.
          SymbolPreviewWidget* m_preview;                    //!< Preview Widget used to visualize the symbol.
          SymbolTableWidget* m_symbolTable;                  //!< Table Widget used to visualize the symbol as separated visual layers.
          QStackedWidget* m_symbolizersStackedWidget;        //!< Set of symbolizers widgets.
          Symbol* m_symbol;                                  //!< Symbol handled by this widget.
          SymbolType m_type;                                 //!< The Symbol type handled by this widget.
      };

    } // end namespace widgets
  }   // end namespace qt
}     // end namespace te

#endif  // __TERRALIB_QT_WIDGETS_SE_INTERNAL_SYMBOLEDITORWIDGET_H
