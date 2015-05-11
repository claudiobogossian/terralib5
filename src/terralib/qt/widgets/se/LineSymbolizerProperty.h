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
  \file terralib/qt/widgets/se/LineSymbolizerProperty.h

  \brief A widget used to define the line symbolizer
         properties, such as:
           - basic stroke
*/

#ifndef __TERRALIB_QT_WIDGETS_SE_INTERNAL_LINESYMBOLIZERPROPERTY_H
#define __TERRALIB_QT_WIDGETS_SE_INTERNAL_LINESYMBOLIZERPROPERTY_H

// TerraLib
#include "../Config.h"

// Qt
#include <QWidget>


namespace te
{
// Forward declarations
  namespace se
  {
    class LineSymbolizer;
    class Symbolizer;
  }

  namespace qt
  {
    namespace widgets
    {
      class BasicStrokePropertyItem;
      /*!
        \class LineSymbolizerProperty

        \brief A widget used to define the polygon symbolizer
               properties, such as:
                 - basic stroke

      */

      class TEQTWIDGETSEXPORT LineSymbolizerProperty : public QWidget
      {
        Q_OBJECT

        public:
          /** @name Initializer Methods
            *  Methods related to instantiation and destruction.
            */
          //@{

          /*! \brief Constructor */
          LineSymbolizerProperty(QWidget* parent);

          /*! \brief Destructor. */
          ~LineSymbolizerProperty();

          //@}

        public:

          /*!
            \brief Sets a line symbolizer element to this widget.

            \param symb A valid line symbolizer element.

            \note The widget will NOT take the ownership of the given symbolizer.
            \note The widget form will be update based on given symbolizer parameters.
          */
          void setSymbolizer(te::se::LineSymbolizer* symb);

          /*!
            \brief Gets the configured line symbolizer element.

            \return The configured line symbolizer element.
            
            \note The caller will take the ownership of the returned symbolizer.
          */
          te::se::Symbolizer* getSymbolizer() const;

        protected slots:

          void onStrokeChanged();

        signals:

          /*! This signal is emitted when the internal line symbolizer element is changed. */
          void symbolizerChanged();


        protected:

          te::qt::widgets::BasicStrokePropertyItem* m_bs;

          te::se::LineSymbolizer* m_symb;                      //!< Line symbolizer element that will be configured by this widget.
      };
    }
  }
}

#endif //__TERRALIB_QT_WIDGETS_SE_INTERNAL_LINESYMBOLIZERPROPERTY_H