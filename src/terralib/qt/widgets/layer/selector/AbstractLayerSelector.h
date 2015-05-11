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
  \file terralib/qt/widgets/layer/selector/AbstractLayerSelector.h

  \brief An abstract class for layer selection widgets.
*/

#ifndef __TERRALIB_QT_WIDGETS_LAYER_SELECTOR_INTERNAL_ABSTRACTLAYERSELECTOR_H
#define __TERRALIB_QT_WIDGETS_LAYER_SELECTOR_INTERNAL_ABSTRACTLAYERSELECTOR_H

// TerraLib
#include "../../../../dataaccess/datasource/DataSourceInfo.h"
#include "../../../../maptools/AbstractLayer.h"
#include "../../Config.h"

// STL
#include <list>

// Qt
#include <QWidget>

namespace te
{
  namespace qt
  {
    namespace widgets
    {
      /*!
        \class AbstractLayerSelector

        \brief An abstract class for layer selection widgets.

        \ingroup widgets
      */
      class TEQTWIDGETSEXPORT AbstractLayerSelector : public QWidget
      {
        Q_OBJECT

        public:

          AbstractLayerSelector(QWidget* parent = 0, Qt::WindowFlags f = 0);

          ~AbstractLayerSelector();

          virtual void set(const std::list<te::da::DataSourceInfoPtr>& datasources) = 0;

          virtual std::list<te::map::AbstractLayerPtr> getLayers() = 0;
      }; 

    } // end namespace widgets
  }   // end namespace qt
}     // end namespace te

#endif  // __TERRALIB_QT_WIDGETS_LAYER_SELECTOR_INTERNAL_ABSTRACTLAYERSELECTOR_H

