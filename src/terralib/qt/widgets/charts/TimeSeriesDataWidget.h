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
  \file terralib/qt/widgets/charts/TimeSeriesDataWidget.h

  \brief A widget used to adjust a timeSeries' input data.
*/

#ifndef __TERRALIB_QT_WIDGETS_INTERNAL_TIMESERIESDATAWIDGET_H
#define __TERRALIB_QT_WIDGETS_INTERNAL_TIMESERIESDATAWIDGET_H

//TerraLib
#include "../Config.h"

// Qt
#include <QWidget>

//STL
#include <memory>

namespace Ui { class TimeSeriesDataWidgetForm; }

namespace te
{
    //forward declarations
  namespace da  { class DataSet; class DataSetType; }
  namespace st { class TimeSeries; }

  namespace qt
  {
    namespace widgets
    {
    //forward declarations
    class ChartDisplay;

    /*!
        \class TimeSeriesDataWidget

        \brief A widget used to adjust a histogram's input data.
      */
      class TEQTWIDGETSEXPORT TimeSeriesDataWidget : public QWidget
      {

        Q_OBJECT

        public:

          /*!
            \brief Constructor

            \param dataSet The dataset that will be used to generate a histogram.
            \param dataType The dataset that will be used to generate a histogram.
            \param parent this widget's parent
            \param f Window flags used to configure this widget
          */
          TimeSeriesDataWidget(te::da::DataSet* dataSet, te::da::DataSetType* dataType, QWidget* parent = 0,  Qt::WindowFlags f = 0);

          /*!
            \brief Destructor
          */
          ~TimeSeriesDataWidget();

          /*!
            \brief Returns a pointer to the widget's form

            \return A TimeSeriesDataWidgetForm type pointer to the widget's form.
            \note The caller will not take ownership of the returned pointer.
          */
          Ui::TimeSeriesDataWidgetForm* getForm();

          /*!
            \brief Returns a pointer to the widget's form

            \return A TimeSeriesDataWidgetForm type pointer to the widget's form.
            \note The caller will take ownership of the returned pointer.
          */
          te::st::TimeSeries* getTimeSeries();

        private:

          std::auto_ptr<Ui::TimeSeriesDataWidgetForm>  m_ui;        //!< The widget's form.
          std::auto_ptr<te::da::DataSet>               m_dataSet;   //!< The dataset that will be used to generate the histogram graph.
          std::auto_ptr<te::da::DataSetType>           m_dataType;  //!< The type of the dataset that will be used to generate the histogram graphic.
       };
    } // end namespace widgets
  }   // end namespace qt
}     // end namespace te

#endif  // __TERRALIB_QT_WIDGETS_INTERNAL_TIMESERIESDATAWIDGET_H