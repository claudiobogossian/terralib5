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
  \file terralib/qt/widgets/charts/ChartWidget.h

  \brief A base widget to be used on the chart settings.
*/

#ifndef __TERRALIB_QT_WIDGETS_INTERNAL_CHARTWIDGET_H
#define __TERRALIB_QT_WIDGETS_INTERNAL_CHARTWIDGET_H

// QT
#include <QWidget>

// TerraLib
#include "../Config.h"

class QwtPlotSeriesItem;

namespace te
{
  namespace qt
  {
    namespace widgets
      {
      //forward declarations
      class ChartDisplay;
      /*!
        \class ChartWidget

        \brief A base widget for settings.
      */
      class TEQTWIDGETSEXPORT ChartWidget : public QWidget
      {
        public:

          /*!
            \brief Constructor

            \param parent this widget's parent
          */
          ChartWidget(QWidget* parent = 0);

          /*! 
            \brief Destructor.
          */
          ~ChartWidget();

          /*!
            \brief Returns a pointer to the chart being configured

            \return A QwtPlotSeriesItem type pointer to the chart being configured
            \note The caller will take ownership of the returned pointer. 
          */
          virtual QwtPlotSeriesItem* getChart();

          /*!
            \brief Returns a pointer to the display being configured

            \return A ChartDisplay type pointer to the display being configured
            \note The caller will take ownership of the returned pointer. 
          */
          virtual te::qt::widgets::ChartDisplay* getDisplay();

          /*!
            \brief It sets the chart being configured

            \param newChart The new QwtPlotSeriesItem.
            \note It will not take ownership of the given pointer 
          */
          virtual void setChart(QwtPlotSeriesItem* newChart);

          /*!
            \brief It sets the ChartDisplay being configured

            \param newDisplay The new ChartDisplay.
            \note It will not take ownership of the given pointer 
          */
          virtual void setDisplay(te::qt::widgets::ChartDisplay* newDisplay);

        public:

          std::string                     m_label;    //!< The chart's Label.
          QwtPlotSeriesItem*              m_chart;    //!< The chart that will be configured by this widget.
          ChartDisplay*                   m_display;  //!< The display that will be configured by this widget.
      };
    }   // end namespace widgets
  }     // end namespace qt
}       // end namespace te

#endif  // __TERRALIB_QT_WIDGETS_INTERNAL_CHARTWIDGET_H
