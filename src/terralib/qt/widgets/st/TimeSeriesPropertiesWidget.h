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
  \file terralib/qt/widgets/st/TimeSeriesPropertiesWidget.h

  \brief A widget used to adjust a time series layer's properties
*/

#ifndef __TERRALIB_QT_WIDGETS_INTERNAL_TIMESERIESWIDGET_H
#define __TERRALIB_QT_WIDGETS_INTERNAL_TIMESERIESWIDGET_H

//TerraLib
#include "../Config.h"

// Qt
#include <QWidget>

//STL
#include <memory>

namespace Ui { class TimeSeriesPropertiesWidgetForm; }

namespace te
{

    //forward declarations
  namespace da  {class DataSet;}

  namespace qt
  {
    namespace widgets
    {
    /*!
        \class TimeSeriesPropertiesWidget

        \brief A widget used to adjust a time series layer's properties
      */
      class TEQTWIDGETSEXPORT TimeSeriesPropertiesWidget : public QWidget
      {

        Q_OBJECT

        public:

          /*!
            \brief Constructor

            \param dataSet The dataset that will be used to generate a TemporalLayer.
            \param parent this widget's parent
            \param f Window flags used to configure this widget
          */
          TimeSeriesPropertiesWidget(te::da::DataSet* dataSet, QWidget* parent = 0,  Qt::WindowFlags f = 0);

          /*!
            \brief Destructor
          */
          ~TimeSeriesPropertiesWidget();

          /*!
            \brief Returns a pointer to the widget's form

            \return A TimeSeriesPropertiesWidgetForm type pointer to the widget's form.
            \note The caller will not take ownership of the returned pointer.
          */
          Ui::TimeSeriesPropertiesWidgetForm* getForm();

        protected slots:

          void onPropertyComboBoxIndexChanged(QString text);

        private:

          std::auto_ptr<Ui::TimeSeriesPropertiesWidgetForm>  m_ui;        //!< The widget's form.
          std::auto_ptr<te::da::DataSet>           m_dataSet;   //!< The dataset that will be used to generate the histogram graph.
       };
    } // end namespace widgets
  }   // end namespace qt
}     // end namespace te

#endif  // __TERRALIB_QT_WIDGETS_INTERNAL_TIMESERIESWIDGET_H