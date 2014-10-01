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
#include <QWidget>

//STL
#include <memory>

class QwtPlotSeriesItem;

namespace Ui { class ChartDisplayWidgetForm; }

namespace te
{

  namespace da    { class ObjectIdSet; }
  namespace qt
  {
    namespace widgets
    {

    //forward declarations
    class ChartDisplay;

    /*!
        \class ChartDisplayWidget

        \brief A wdiget used to display a chart

        \ingroup widgets
      */
      class TEQTWIDGETSEXPORT ChartDisplayWidget : public QWidget
      {

        Q_OBJECT

        public:

          /*!
            \brief Constructor

            \param chart The chart that will be displayed by this widget.
            \param display The display that will be used by this widget to display the chart
            \param parent this widget's parent
            \param f Window flags used to configure this widget
          */
          ChartDisplayWidget(QwtPlotSeriesItem* chart, int type, te::qt::widgets::ChartDisplay* display, QWidget* parent = 0,  Qt::WindowFlags f = 0);


          /*! 
            \brief Destructor 
          */
          ~ChartDisplayWidget();

          /*! 
            \brief Returns a pointer to the chart being displayed.

            \return A QwtPlotSeriesItem type pointer to the chart being used.
            \note The caller will not take ownership of the returned pointer.
          */
          QwtPlotSeriesItem* getChart();

          /*!
            \brief It sets the chart that will be displayed
.
            \param newChart The new chart.
            \note It will take ownership of the given pointer 
          */
          void setChart(QwtPlotSeriesItem* newChart);

          /*! 
            \brief Returns a pointer to the display being used.

            \return A ChartDisplay type pointer to the display being used.
            \note The caller will not take ownership of the returned pointer.  
          */
          te::qt::widgets::ChartDisplay* getDisplay();

          /*!
            \brief It sets the display that will be used
.
            \param newDisplay The new display.
            \note It will take ownership of the given pointer 
          */
          void setDisplay(te::qt::widgets::ChartDisplay* newDisplay);

          int getType();

          /*!
            \brief Highlights the objects identified by \a oids

            \param oids The identifiers of plotitems to be highlighted.
          */
          void highlightOIds(const te::da::ObjectIdSet* oids);

          /*!
            \brief color used to hgihlight selected objects on this display.

            \param selColor The color used to hgihlight selected objects on this display.

          */
          void setSelectionColor(QColor selColor);

        protected slots:

          /*!
            \brief Called when the user clicks on the Settings tool button.
          */
          void onSettingsToolButtonnClicked();

          /*!
            \brief Called when objects were selected.
          */
          void selectionChanged(te::da::ObjectIdSet* oids, const bool& add);

        signals:

          /*!
            \brief Emmit when objects were selected.
          */
          void selected(te::da::ObjectIdSet*, const bool&);

        private:

          std::auto_ptr<Ui::ChartDisplayWidgetForm>  m_ui;       //!< The widget form.
          QwtPlotSeriesItem*                         m_chart;    //!< The chart that will be plotted by this widget.
          int                                        m_type;     //!< The type of the chart that will be plotted by this widget.
          ChartDisplay*                              m_display;  //!< The display that will be used to plot the chart.
      };
    } // end namespace widgets
  }   // end namespace qt
}     // end namespace te

#endif  // __TERRALIB_QT_WIDGETS_INTERNAL_CHARTDISPLAYWIDGET_H