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
  \file terralib/qt/widgets/rp/RasterHistogramWidget.h

  \brief This file has the RasterHistogramWidget class.
*/

#ifndef __TERRALIB_QT_WIDGETS_CANVAS_INTERNAL_RASTERHISTOGRAMWIDGET_H
#define __TERRALIB_QT_WIDGETS_CANVAS_INTERNAL_RASTERHISTOGRAMWIDGET_H

// TerraLib
#include "../../../maptools/AbstractLayer.h"
#include "../Config.h"

// STL
#include <memory>

// Qt
#include <QtGui/QWidget>

namespace Ui { class RasterHistogramWidgetForm; }

namespace te
{
  namespace qt
  {
    namespace widgets
    {
      class ChartStyle;
      class ChartDisplay;
      /*!
        \class RasterHistogramWidget

        \brief This class is used to define a component for viewing overlaid layers.
      */
      class TEQTWIDGETSEXPORT RasterHistogramWidget : public QWidget
      {
        Q_OBJECT

        public:

          RasterHistogramWidget(QWidget* parent = 0, Qt::WindowFlags f = 0);

          ~RasterHistogramWidget();

          Ui::RasterHistogramWidgetForm* getForm() const;

          void set(te::map::AbstractLayerPtr layer);


        protected slots:

          void onRedToolButtonToggled(bool flag);

          void onGreenToolButtonToggled(bool flag);

          void onBlueToolButtonToggled(bool flag);

          void onMonoToolButtonToggled(bool flag);

        private:

         std::auto_ptr<Ui::RasterHistogramWidgetForm> m_ui;

         te::qt::widgets::ChartStyle* m_chartStyle;
         te::qt::widgets::ChartDisplay* m_chartDisplay;

         te::map::AbstractLayerPtr m_layer;
      }; 

    } // end namespace widgets
  }   // end namespace qt
}     // end namespace te

#endif  // __TERRALIB_QT_WIDGETS_CANVAS_INTERNAL_RASTERHISTOGRAMWIDGET_H

