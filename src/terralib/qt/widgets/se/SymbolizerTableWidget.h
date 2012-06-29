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
  \file terralib/qt/widgets/se/SymbolizerTableWidget.h

  \brief A widget used to preview a set of symbolizer elements as separated visual layers.
*/

#ifndef __TERRALIB_QT_WIDGETS_SE_INTERNAL_SYMBOLIZERTABLEWIDGET_H
#define __TERRALIB_QT_WIDGETS_SE_INTERNAL_SYMBOLIZERTABLEWIDGET_H

// TerraLib
#include "../../../geometry/Enums.h"
#include "../../../se/Enums.h"
#include "../Config.h"

// Qt
#include <QtGui/QWidget>

// STL
#include <vector>

// Forward declarations
class QTableWidget;

namespace te
{
// Forward declarations
  namespace gm
  {
    class Geometry;
  }

  namespace se
  {
    class Symbolizer;
  }

  namespace qt
  {
    namespace widgets
    {
      /*!
        \class SymbolizerTableWidget

        \brief A widget used to preview a set of symbolizer elements as separated visual layers.
      */
      class TEQTWIDGETSEXPORT SymbolizerTableWidget : public QWidget
      {
        public:

          /** @name Initializer Methods
           *  Methods related to instantiation and destruction.
           */
          //@{

          /*!
            \brief Constructs a symbolizer table widget with fixed size, which is a child of parent, with widget flags set to f. 

            \param size The preview size.
            \param type The symbolizer type that will be visualized on preview.
            \param parent The widget's parent.
          */
          SymbolizerTableWidget(const QSize& size, const te::se::SymbolizerType& type, QWidget* parent = 0);

          /*! \brief Destructor. */
          ~SymbolizerTableWidget();

          //@}

        public:

          /*!
            \brief Preview a set of symbolizer elements.

            \param symbs A valid set of symbolizer elements.

            \note The widget will NOT take the ownership of the given symbolizers.
          */
          void updatePreview(const std::vector<te::se::Symbolizer*>& symbs);

          /*!
            \brief Sets the symbolizer type that will be visualized on preview.

            \param type The symbolizer type.
          */
          void setSymbolizerType(const te::se::SymbolizerType& type);

          QSize sizeHint() const;

        private:

          QTableWidget* m_previewTable; //!< Qt element that will be used to visualize preview results.
          te::gm::Geometry* m_geom;     //!< Geometry used to draw the preview.
          QSize m_size;                 //!< Preview size.
      };

    } // end namespace widgets
  }   // end namespace qt
}     // end namespace te

#endif  // __TERRALIB_QT_WIDGETS_SE_INTERNAL_SYMBOLIZERTABLEWIDGET_H
