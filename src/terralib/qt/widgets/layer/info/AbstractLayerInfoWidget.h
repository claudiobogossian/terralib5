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
  \file terralib/qt/widgets/info/AbstractLayerInfoWidget.h

  \brief 
*/

#ifndef __TERRALIB_QT_WIDGETS_LAYER_INTERNAL_ABSTRACTLAYERINFOWIDGET_H
#define __TERRALIB_QT_WIDGETS_LAYER_INTERNAL_ABSTRACTLAYERINFOWIDGET_H

#include "../../Config.h"
#include "../../widgets/propertybrowser/AbstractPropertyItem.h"

namespace te
{
  // Forward declaration
  namespace map
  {
     class AbstractLayer;
  }

  namespace qt
  {
    namespace widgets
    {
      class TEQTWIDGETSEXPORT AbstractLayerInfoWidget : public AbstractPropertyItem
      {
        Q_OBJECT

        public:

          /*! \brief Constructor */
          AbstractLayerInfoWidget(QtTreePropertyBrowser* pb, te::map::AbstractLayer* layer);

          /*! \brief Destructor. */
          ~AbstractLayerInfoWidget();

        public slots:

          virtual void valueChanged(QtProperty* p, int value);

          virtual void valueChanged(QtProperty* p, QString value);

        protected:

          te::map::AbstractLayer* m_layer;
      };
    }
  }
}

#endif //__TERRALIB_QT_WIDGETS_LAYER_INTERNAL_ABSTRACTLAYERINFOWIDGET_H