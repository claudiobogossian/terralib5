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
  \file terralib/qt/widgets/se/PointPlacementPropertyItem.h

  \brief A widget used to define the PointPlacement se object.
*/

#ifndef __TERRALIB_QT_WIDGETS_SE_INTERNAL_POINTPLACEMENTPROPERTYITEM_H
#define __TERRALIB_QT_WIDGETS_SE_INTERNAL_POINTPLACEMENTPROPERTYITEM_H

// TerraLib
#include "../Config.h"
#include "../propertybrowser/AbstractPropertyItem.h"

namespace te
{
// Forward declarations
  namespace se
  {
    class PointPlacement;
  }

  namespace qt
  {
    namespace widgets
    {
      /*!
        \class PointPlacementPropertyItem

        \brief A widget used to define the PointPlacement se object.

      */

      class TEQTWIDGETSEXPORT PointPlacementPropertyItem : public te::qt::widgets::AbstractPropertyItem
      {
        Q_OBJECT

        public:
          /** @name Initializer Methods
            *  Methods related to instantiation and destruction.
            */
          //@{

          /*! \brief Constructor */
          PointPlacementPropertyItem(QtTreePropertyBrowser* pb, QColor c = QColor());

          /*! \brief Destructor. */
          ~PointPlacementPropertyItem();

          //@}

          /** @name Re-implementation of Pure Virtual Method
          *   AbstractFillWidget methods.
          */
          //@{

          void setPointPlacement(const te::se::PointPlacement* pp);

          te::se::PointPlacement* getPointPlacement() const;

          void resetPointPlacement();

          /*! \brief Updates the widget form based on internal fill element. */
          void updateUi();

        public slots:

          virtual void valueChanged(QtProperty* p, int value);

          virtual void valueChanged(QtProperty* p, const QSize &value);

        signals:

          /*! This signal is emitted when the internal fill element is changed. */
          void pointPlacementChanged();

        protected:

          QtProperty* m_anchorProperty;
          QtProperty* m_displacementProperty;
          QtProperty* m_rotationProperty;

          te::se::PointPlacement* m_pointPlacement;         //!< Fill element that will be configured by this widget.

          bool m_update;
      };
    }
  }
}

#endif //__TERRALIB_QT_WIDGETS_SE_INTERNAL_POINTPLACEMENTPROPERTYITEM_H