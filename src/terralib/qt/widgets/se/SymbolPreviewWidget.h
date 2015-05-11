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
  \file terralib/qt/widgets/se/SymbolPreviewWidget.h

  \brief A widget used to preview symbol elements.
*/

#ifndef __TERRALIB_QT_WIDGETS_SE_INTERNAL_SYMBOLPREVIEWWIDGET_H
#define __TERRALIB_QT_WIDGETS_SE_INTERNAL_SYMBOLPREVIEWWIDGET_H

// TerraLib
#include "../Config.h"

// Qt
#include <QWidget>

// STL
#include <vector>

// Forward declarations
class QLabel;

namespace te
{
// Forward declaration
  namespace gm
  {
    class Geometry;
  }

  namespace qt
  {
    namespace widgets
    {
// Forward declaration
      class Symbol;

      /*!
        \class SymbolPreviewWidget

        \brief A widget used to preview symbol elements.
      */
      class TEQTWIDGETSEXPORT SymbolPreviewWidget : public QWidget
      {
        public:

          /** @name Initializer Methods
           *  Methods related to instantiation and destruction.
           */
          //@{

          /*!
            \brief Constructs a symbol preview widget with fixed size, which is a child of parent, with widget flags set to f.

            \param size The preview size.
            \param parent The widget's parent.
          */
          SymbolPreviewWidget(const QSize& size, QWidget* parent = 0);

          /*! \brief Destructor. */
          ~SymbolPreviewWidget();

          //@}

        public:

          /*!
            \brief Preview a symbol element.

            \param symb A valid symbol element.

            \note The widget will NOT take the ownership of the given symbol.
          */
          void updatePreview(Symbol* symbol);

          /*!
            \brief Preview a set of symbolizer elements.

            \param symbolizers The set of symbolizer elements.

            \note The widget will NOT take the ownership of the given symbolizers.
          */
          void updatePreview(const std::vector<te::se::Symbolizer*>& symbolizers);

          /*!
            \brief Preview a symbolizer element.

            \param symb A valid symbolizer element.

            \note The widget will NOT take the ownership of the given symbolizer.
          */
          void updatePreview(te::se::Symbolizer* symbolizer);

        private:

          QLabel* m_previewLabel;       //!< Qt element that will be used to visualize preview result.
          te::gm::Geometry* m_geom;     //!< Geometry used to draw the preview.
          QSize m_size;                 //!< Preview size.
      };

    } // end namespace widgets
  }   // end namespace qt
}     // end namespace te

#endif  // __TERRALIB_QT_WIDGETS_SE_INTERNAL_SYMBOLPREVIEWWIDGET_H
