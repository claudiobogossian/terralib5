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
  \file terralib/qt/widgets/st/ObservationPropertiesWidget.h

  \brief A widget used to adjust a Observation layer's properties
*/

#ifndef __TERRALIB_QT_WIDGETS_INTERNAL_OBSERVATIONPROPERTIESWIDGET_H
#define __TERRALIB_QT_WIDGETS_INTERNAL_OBSERVATIONPROPERTIESWIDGET_H

//TerraLib
#include "../../../dataaccess/dataset/DataSetType.h"
#include "../Config.h"

// Qt
#include <QWidget>

//STL
#include <memory>

namespace Ui { class ObservationPropertiesWidgetForm; }

namespace te
{
  namespace qt
  {
    namespace widgets
    {

      class DoubleListWidget;

    /*!
        \class ObservationPropertiesWidget

        \brief A widget used to adjust a Observation layer's properties
      */
      class TEQTWIDGETSEXPORT ObservationPropertiesWidget : public QWidget
      {

        Q_OBJECT

        public:

          /*!
            \brief Constructor

            \param dataSetType The datasetType that will be used to generate a ObservationLayer.
            \param parent this widget's parent
            \param f Window flags used to configure this widget
          */
          ObservationPropertiesWidget(QWidget* parent = 0,  Qt::WindowFlags f = 0);

          /*!
            \brief Destructor
          */
          ~ObservationPropertiesWidget();

          /*!
            \brief Returns a pointer to the widget's form

            \return A ObservationPropertiesWidgetForm type pointer to the widget's form.
            \note The caller will not take ownership of the returned pointer.
          */
          Ui::ObservationPropertiesWidgetForm* getForm();

          /*!
            \brief Returns a vector containing the indexes of the observed properties

            \return A vector containing the indexes of the observed properties.
          */
          std::vector<int> getOutputValues();

          /*!
            \brief Returns the index of the temporal property geometry

            \return The index of the temporal property geometry
            \note Will return an invalid index if the dataSeType hasn't been given.
          */
          int getGeometryId();

          /*!
            \brief Adjusts the widget's components based on the given datasettype

            \param dataType The datasetType that will be used to configure the widget.
          */
          void setUp(const te::da::DataSetTypePtr dataType);

        private:

          std::auto_ptr<DoubleListWidget>                     m_obsWidget;  //!< The widget used to select the observed properties.
          std::auto_ptr<Ui::ObservationPropertiesWidgetForm>  m_ui;         //!< The widget's form.
          te::da::DataSetTypePtr                              m_dataType;   //!< The datasetType that will be used to generate the spatio-temporal layer.
       };
    } // end namespace widgets
  }   // end namespace qt
}     // end namespace te

#endif  // __TERRALIB_QT_WIDGETS_INTERNAL_OBSERVATIONPROPERTIESWIDGET_H