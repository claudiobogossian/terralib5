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
  \file terralib/qt/widgets/se/PolygonSymbolizerProperty.h

  \brief A widget used to define the polygon symbolizer
         properties, such as:
           - basic fill
           - basic stroke
*/

#ifndef __TERRALIB_QT_WIDGETS_SE_INTERNAL_POLYGONSYMBOLIZERPROPERTY_H
#define __TERRALIB_QT_WIDGETS_SE_INTERNAL_POLYGONSYMBOLIZERPROPERTY_H

// TerraLib
#include "../Config.h"

// Qt
#include <QWidget>


namespace te
{
// Forward declarations
  namespace se
  {
    class PolygonSymbolizer;
    class Symbolizer;
  }

  namespace qt
  {
    namespace widgets
    {
      class BasicFillPropertyItem;
      class BasicStrokePropertyItem;
      class GraphicProperty;
      /*!
        \class PolygonSymbolizerProperty

        \brief A widget used to define the polygon symbolizer
               properties, such as:
                 - basic fill
                 - basic stroke

      */

      class TEQTWIDGETSEXPORT PolygonSymbolizerProperty : public QWidget
      {
        Q_OBJECT

        public:
          /** @name Initializer Methods
            *  Methods related to instantiation and destruction.
            */
          //@{

          /*! \brief Constructor */
          PolygonSymbolizerProperty(QWidget* parent);

          /*! \brief Destructor. */
          ~PolygonSymbolizerProperty();

          //@}

        public:

          /*!
            \brief Sets a polygon symbolizer element to this widget.

            \param symb A valid polygon symbolizer element.

            \note The widget will NOT take the ownership of the given symbolizer.
            \note The widget form will be update based on given symbolizer parameters.
          */
          void setSymbolizer(te::se::PolygonSymbolizer* symb);

          /*!
            \brief Gets the configured polygon symbolizer element.

            \return The configured polygon symbolizer element.
            
            \note The caller will take the ownership of the returned symbolizer.
          */
          te::se::Symbolizer* getSymbolizer() const;

        protected slots:

          void onStrokeChanged();

          void onFillChanged();

          void onPolyGraphicChanged();

        signals:

          /*! This signal is emitted when the internal polygon symbolizer element is changed. */
          void symbolizerChanged();


        protected:
          te::qt::widgets::BasicFillPropertyItem* m_bf;
          te::qt::widgets::BasicStrokePropertyItem* m_bs;
          te::qt::widgets::GraphicProperty* m_graph;

          bool m_setLocalSymbol;

          te::se::PolygonSymbolizer* m_symb;                   //!< Polygon symbolizer element that will be configured by this widget.
      };
    }
  }
}

#endif //__TERRALIB_QT_WIDGETS_SE_INTERNAL_POLYGONSYMBOLIZERPROPERTY_H