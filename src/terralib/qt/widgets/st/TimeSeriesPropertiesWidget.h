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
  \file terralib/qt/widgets/st/TimeSeriesPropertiesWidget.h

  \brief A widget used to adjust a time series layer's properties
*/

#ifndef __TERRALIB_QT_WIDGETS_INTERNAL_TIMESERIESWIDGET_H
#define __TERRALIB_QT_WIDGETS_INTERNAL_TIMESERIESWIDGET_H

//TerraLib
#include "../../../dataaccess/dataset/DataSetType.h"
#include "../Config.h"

// Qt
#include <QWidget>

//STL
#include <memory>

namespace Ui { class TimeSeriesPropertiesWidgetForm; }

namespace te
{
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
          TimeSeriesPropertiesWidget(QWidget* parent = 0,  Qt::WindowFlags f = 0);

          /*!
            \brief Destructor
          */
          ~TimeSeriesPropertiesWidget();

          /*!
            \brief Returns the name of the property that holds the ID of each timeSeries in the dataset

            \return The name of the property that holds the ID of each timeSeries in the dataset
.
          */
          std::string getIdPropName();

          /*!
            \brief Returns the index of the ID of each timeSeries in the dataset

            \return The index of the ID of each timeSeries in the dataset
            \note Will return an invalid index if the dataSeType hasn't been given.
          */
          int getIdIndex();

          /*!
            \brief Returns the name of the property that holds the attribute values associated to each time series in the dataset

            \return The name of the property that holds the the attribute values associated to each time series in the dataset
.
          */
          std::string getValuePropName();

          /*!
            \brief Returns the index of the attribute values associated to each time series in the dataset

            \return The index of the attribute values associated to each time series in the dataset
            \note Will return an invalid index if the dataSeType hasn't been given.
          */
          int getValueIndex();

          /*!
            \brief Returns the name of the property that holds the geometry

            \return The name of the property that holds the geometry
.
          */
          std::string getGeometryPropName();

          /*!
            \brief Returns the index of the temporal property geometry

            \return The index of the temporal property geometry
            \note Will return an invalid index if the dataSeType hasn't been given.
          */
          int getGeometryId();




          /*!
            \brief Returns a pointer to the widget's form

            \return A TimeSeriesPropertiesWidgetForm type pointer to the widget's form.
            \note The caller will not take ownership of the returned pointer.
          */
          Ui::TimeSeriesPropertiesWidgetForm* getForm();

          /*!
            \brief Adjusts the widget's components based on the given datasettype

            \param dataType The datasetType that will be used to configure the widget.
          */
          void setUp(const te::da::DataSetTypePtr dataType);

        protected slots:

          void onPropertyComboBoxIndexChanged(QString text);

        private:

          std::auto_ptr<Ui::TimeSeriesPropertiesWidgetForm>  m_ui;         //!< The widget's form.
          te::da::DataSetTypePtr                             m_dataType;   //!< The datasetType that will be used to generate the spatio-temporal layer.
       };
    } // end namespace widgets
  }   // end namespace qt
}     // end namespace te

#endif  // __TERRALIB_QT_WIDGETS_INTERNAL_TIMESERIESWIDGET_H