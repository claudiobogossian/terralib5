/*  Copyright (C) 2010-2013 National Institute For Space Research (INPE) - Brazil.

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
  \file terralib/qt/widgets/charts/ChartDisplayWidget.h

  \brief A widget used to display a set of charts.
*/

#ifndef __TERRALIB_QT_WIDGETS_INTERNAL_CHARTDISPLAYWIDGET_H
#define __TERRALIB_QT_WIDGETS_INTERNAL_CHARTDISPLAYWIDGET_H

//TerraLib
#include "../Config.h"
#include "Enums.h"

// Qt
#include <QtGui/QWidget>

//STL
#include <memory>

class QwtPlotSeriesItem;

namespace Ui { class ChartDisplayWidgetForm; }

namespace te
{
  namespace qt
  {
    namespace widgets
    {

    //forward declarations
    class ChartDisplay;

    /*!
        \class Histogram Creator

        \brief A wdiget used to customize a Histogram parameters.
      */
      class TEQTWIDGETSEXPORT ChartDisplayWidget : public QWidget
      {

        Q_OBJECT

        public:

          ChartDisplayWidget(QwtPlotSeriesItem* chart, int type, te::qt::widgets::ChartDisplay* display, QWidget* parent = 0,  Qt::WindowFlags f = 0);

          ~ChartDisplayWidget();

          QwtPlotSeriesItem* getChart();

          void setChart(QwtPlotSeriesItem* newChart);

          te::qt::widgets::ChartDisplay* getDisplay();

          void setDisplay(te::qt::widgets::ChartDisplay* newDisplay);

          int getType();

        protected slots:

          void onSettingsToolButtonnTriggered();

        private:

          std::auto_ptr<Ui::ChartDisplayWidgetForm>  m_ui;       //!< The widget form.
          QwtPlotSeriesItem*                         m_chart;    //!< The chart that will be plotted by this widget.
          int                                        m_type;     //!< The type of the chart that will be plotted by this widget.
          te::qt::widgets::ChartDisplay*             m_display;  //!< The display that will be used to plot the chart.
      };
    } // end namespace widgets
  }   // end namespace qt
}     // end namespace te

#endif  // __TERRALIB_QT_WIDGETS_INTERNAL_CHARTDISPLAYWIDGET_H