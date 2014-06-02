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
  \file terralib/qt/widgets/se/SymbolTableWidget.h

  \brief A widget used to preview a symbol as separated layers.
*/

#ifndef __TERRALIB_QT_WIDGETS_SE_INTERNAL_SYMBOLTABLEWIDGET_H
#define __TERRALIB_QT_WIDGETS_SE_INTERNAL_SYMBOLTABLEWIDGET_H

// TerraLib
#include "../Config.h"

// Qt
#include <QWidget>

// Forward declarations
class QTableWidget;

namespace te
{
  namespace qt
  {
    namespace widgets
    {
// Forward declaration
      class Symbol;

      /*!
        \class SymbolTableWidget

        \brief A widget used to preview a symbol as separated layers.
      */
      class TEQTWIDGETSEXPORT SymbolTableWidget : public QWidget
      {
        Q_OBJECT

        public:

          /** @name Initializer Methods
           *  Methods related to instantiation and destruction.
           */
          //@{

          /*!
            \brief Constructs a symbol table widget with fixed size, which is a child of parent, with widget flags set to f. 

            \param size The preview size.
            \param parent The widget's parent.
          */
          SymbolTableWidget(const QSize& size, QWidget* parent = 0);

          /*! \brief Destructor. */
          ~SymbolTableWidget();

          //@}

        public:

          /*!
            \brief Preview a symbol element.

            \param symb A valid symbol element.

            \note The widget will NOT take the ownership of the given symbol.
          */
          void updatePreview(Symbol* symbol);

          /*!
            \brief Selects the given index on symbol table layer.

            \param index The symbolizer index of symbol.
          */
          void selectSymbolizer(const int& index);

          /*!
            \brief Return the size hint to this widget.

            \return The size hint.
          */
          QSize sizeHint() const;

        protected slots:

          void onPreviewTableItemSelectionChanged();

        signals:

          /*!
            \brief This signal is emitted when a symbolizer of the symbol is clicked.

            \param index The symbolizer index.
          */
          void symbolizerClicked(int index);

        private:

          QTableWidget* m_previewTable; //!< Qt element that will be used to visualize preview results.
          QSize m_size;                 //!< Preview size.
      };

    } // end namespace widgets
  }   // end namespace qt
}     // end namespace te

#endif  // __TERRALIB_QT_WIDGETS_SE_INTERNAL_SYMBOLTABLEWIDGET_H
