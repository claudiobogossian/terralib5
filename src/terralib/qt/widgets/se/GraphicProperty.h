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
  \file terralib/qt/widgets/se/GraphicPropertyItem.h

  \brief A widget used to define the general properties of a se object.
*/

#ifndef __TERRALIB_QT_WIDGETS_SE_INTERNAL_GRAPHICPROPERTY_H
#define __TERRALIB_QT_WIDGETS_SE_INTERNAL_GRAPHICPROPERTY_H

// TerraLib
#include "../Config.h"


// Qt
#include <QWidget>

namespace te
{
  //forward declaration
  namespace se
  {
    class Graphic;
  }

  namespace qt
  {
    namespace widgets
    {
      class GraphicPropertyItem;
      class WellKnownMarkPropertyItem;
      class GlyphMarkPropertyItem;
      class LocalImagePropertyItem;

      /*!
        \class GraphicProperty

        \brief A widget used to define the general properties of a se object.

      */

      class TEQTWIDGETSEXPORT GraphicProperty : public QWidget
      {
        Q_OBJECT

        public:
          /** @name Initializer Methods
            *  Methods related to instantiation and destruction.
            */
          //@{

          /*! \brief Constructor */
          GraphicProperty(QWidget* parent);

          /*! \brief Destructor. */
          ~GraphicProperty();

          //@}

        public:

          /** @name Re-implementation of Pure Virtual Method
          *   AbstractGraphicWidget methods.
          */
          //@{

          bool setGraphic(const te::se::Graphic* graphic);

          te::se::Graphic* getGraphic() const;

          //@}

        public slots:

          void onGraphicPropertyChanged();

          void onWellKnownMarkChanged();

          void onGlyphMarkChanged();

          void onLocalImageChanged();

        signals:

          /*! This signal is emitted when the internal graphic element is changed. */
          void graphicChanged();

        protected:

          te::qt::widgets::GraphicPropertyItem* m_graphp;
          te::qt::widgets::WellKnownMarkPropertyItem* m_mp;
          te::qt::widgets::GlyphMarkPropertyItem* m_gp;
          te::qt::widgets::LocalImagePropertyItem* m_li;

          bool m_setLocalGraphic;

          te::se::Graphic* m_graphic;
      };
    }
  }
}

#endif //__TERRALIB_QT_WIDGETS_SE_INTERNAL_GRAPHICPROPERTY_H