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
  \file terralib/qt/widgets/se/ScalePropertyItem.h

  \brief A widget used to define the scale values.
*/

#ifndef __TERRALIB_QT_WIDGETS_SE_INTERNAL_SCALEPROPERTYITEM_H
#define __TERRALIB_QT_WIDGETS_SE_INTERNAL_SCALEPROPERTYITEM_H

// TerraLib
#include "../Config.h"
#include "../propertybrowser/AbstractPropertyItem.h"

namespace te
{
// Forward declarations
  namespace qt
  {
    namespace widgets
    {
      /*!
        \class ScalePropertyItem

        \brief A widget used to define the scale values.

      */

      class TEQTWIDGETSEXPORT ScalePropertyItem : public te::qt::widgets::AbstractPropertyItem
      {
        Q_OBJECT

        public:
          /** @name Initializer Methods
            *  Methods related to instantiation and destruction.
            */
          //@{

          /*! \brief Constructor */
          ScalePropertyItem(QtTreePropertyBrowser* pb, QColor c = QColor());

          /*! \brief Destructor. */
          ~ScalePropertyItem();

          //@}

          /** @name Re-implementation of Pure Virtual Method
          *   AbstractPropertyItem methods.
          */
          //@{

          void setMinScaleDenominator(const double& value);

          double getMinScaleDenominator();

          void setMaxScaleDenominator(const double& value);

          double getMaxScaleDenominator();


        public slots:

          virtual void valueChanged(QtProperty* p, double value);


        signals:

          /*! This signal is emitted when the internal fill element is changed. */
          void scaleMinChanged();

          void scaleMaxChanged();

        protected:

          QtProperty* m_minScaleProperty;
          QtProperty* m_maxScaleProperty;

          double m_minScale;
          double m_maxScale;
      };
    }
  }
}

#endif //__TERRALIB_QT_WIDGETS_SE_INTERNAL_SCALEPROPERTYITEM_H
