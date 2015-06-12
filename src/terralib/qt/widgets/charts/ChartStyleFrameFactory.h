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
  \file terralib/qt/widgets/charts/ChartStyleFrameFactory.h

  \brief A factory to build the Chart frame object.
*/

#ifndef __TERRALIB_QT_WIDGETS_INTERNAL_CHARTSTYLEFRAMEFACTORY_H
#define __TERRALIB_QT_WIDGETS_INTERNAL_CHARTSTYLEFRAMEFACTORY_H

// QT
#include <QWidget>

// TerraLib
#include "../../../common/AbstractFactory.h"
#include "../Config.h"
#include "ChartWidget.h"
#include "ChartWidgetFactory.h"
#include "ChartStyleFrame.h"

namespace te
{
  namespace qt
  {   
    namespace widgets
    {
      /*!
        \class ChartFrameFactory

        \brief A factory to build the Chart frame object.
      */
      class TEQTWIDGETSEXPORT ChartStyleFrameFactory : public te::qt::widgets::ChartWidgetFactory
      {
        public:

          static void initialize();

          static void finalize();

          ~ChartStyleFrameFactory();

        protected:

          te::qt::widgets::ChartWidget* build();

          ChartStyleFrameFactory();

          virtual te::qt::widgets::ChartWidget* create(QWidget* parent = 0);

        private:

          static ChartStyleFrameFactory* sm_factory;

      };
    } // end namespace widgets
  }   // end namespace qt
}     // end namespace te
#endif // __TERRALIB_QT_WIDGETS_INTERNAL_CHARTSTYLEFRAMEFACTORY_H