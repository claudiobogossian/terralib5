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
#include <QWidget>

//QWT
#include <qwt_plot_marker.h>

namespace Ui { class RasterHistogramWidgetForm; }

namespace te
{
  namespace rst { class Raster; }

  namespace qt
  {
    namespace widgets
    {
      class ChartStyle;
      class ChartDisplay;
      class Histogram;
      class HistogramChart;
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

          void setInputRaster(te::rst::Raster* raster);

          void setOutputRaster(te::rst::Raster* raster);

          void drawHistogram(int band);

          void setMinimumValueEnabled(bool enable);

          void updateMinimumValueLine(int value, bool replot = false);

          void updateMinimumValueLabel(QString value);

          void setMaximumValueEnabled(bool enable);

          void updateMaximumValueLine(int value, bool replot = false);

          void updateMaximumValueLabel(QString value);


        protected slots:

          void onApplyToolButtonClicked();

          void onLeftPointSelected(const QPointF& point);

          void onRigthPointSelected(const QPointF& point);

        signals:

          void minValueSelected(int value, int band);

          void maxValueSelected(int value, int band);


        private:

         std::auto_ptr<Ui::RasterHistogramWidgetForm> m_ui;

         std::auto_ptr<te::rst::Raster> m_inputRaster;
         std::auto_ptr<te::rst::Raster> m_outputRaster;

         te::qt::widgets::ChartStyle* m_chartStyle;
         te::qt::widgets::ChartDisplay* m_chartDisplay;

         te::qt::widgets::Histogram* m_histogramInput;
         te::qt::widgets::HistogramChart* m_histogramChartInput;

         te::qt::widgets::Histogram* m_histogramOutput;
         te::qt::widgets::HistogramChart* m_histogramChartOutput;

         QwtPlotMarker* m_minValueLine;
         QwtPlotMarker* m_maxValueLine;
      }; 

    } // end namespace widgets
  }   // end namespace qt
}     // end namespace te

#endif  // __TERRALIB_QT_WIDGETS_CANVAS_INTERNAL_RASTERHISTOGRAMWIDGET_H

