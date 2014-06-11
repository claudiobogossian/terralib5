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
  \file terralib/qt/widgets/st/TrajectoryPropertiesWidget.h

  \brief A widget used to adjust a Trajectory layer's properties
*/

#ifndef __TERRALIB_QT_WIDGETS_INTERNAL_TRAJECTORYPROPERTIESWIDGET_H
#define __TERRALIB_QT_WIDGETS_INTERNAL_TRAJECTORYPROPERTIESWIDGET_H

//TerraLib
#include "../Config.h"

// Qt
#include <QWidget>

//STL
#include <memory>

namespace Ui { class TrajectoryPropertiesWidgetForm; }

namespace te
{

    //forward declarations
  namespace da  {class DataSetType;}

  namespace qt
  {
    namespace widgets
    {
    /*!
        \class TrajectoryPropertiesWidget

        \brief A widget used to adjust a Trajectory layer's properties
      */
      class TEQTWIDGETSEXPORT TrajectoryPropertiesWidget : public QWidget
      {

        Q_OBJECT

        public:

          /*!
            \brief Constructor

            \param dataSetType The datasetType that will be used to generate a TrajectoryLayer.
            \param parent this widget's parent
            \param f Window flags used to configure this widget
          */
          TrajectoryPropertiesWidget(te::da::DataSetType* dataType, QWidget* parent = 0,  Qt::WindowFlags f = 0);

          /*!
            \brief Destructor
          */
          ~TrajectoryPropertiesWidget();

          /*!
            \brief Returns a pointer to the widget's form

            \return A TrajectoryPropertiesWidgetForm type pointer to the widget's form.
            \note The caller will not take ownership of the returned pointer.
          */
          Ui::TrajectoryPropertiesWidgetForm* getForm();

        protected slots:

          void onPropertyComboBoxIndexChanged(QString text);

        private:

          std::auto_ptr<Ui::TrajectoryPropertiesWidgetForm>  m_ui;        //!< The widget's form.
          std::auto_ptr<te::da::DataSetType>                 m_dataType;  //!< The datasetType that will be used to generate the TrajectoryLayer.
       };
    } // end namespace widgets
  }   // end namespace qt
}     // end namespace te

#endif  // __TERRALIB_QT_WIDGETS_INTERNAL_TRAJECTORYPROPERTIESWIDGET_H