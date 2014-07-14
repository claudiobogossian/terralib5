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
  \file terralib/qt/widgets/st/TimePropertiesWidget.h

  \brief A widget used to adjust a temporal layer's properties
*/

#ifndef __TERRALIB_QT_WIDGETS_INTERNAL_TIMEPROPERTIESWIDGET_H
#define __TERRALIB_QT_WIDGETS_INTERNAL_TIMEPROPERTIESWIDGET_H

//TerraLib
#include "../../../dataaccess/dataset/DataSetType.h"
#include "../Config.h"

// Qt
#include <QWidget>

//STL
#include <memory>

namespace Ui { class TemporalPropertiesWidgetForm; }

namespace te
{

    //forward declarations
  namespace da  {class DataSetType;}

  namespace qt
  {
    namespace widgets
    {
    /*!
        \class TemporalPropertiesWidget

        \brief A widget used to adjust a temporal layer's properties
      */
      class TEQTWIDGETSEXPORT TemporalPropertiesWidget : public QWidget
      {

        Q_OBJECT

        public:

          /*!
            \brief Constructor


            \param parent this widget's parent
            \param f Window flags used to configure this widget
          */
          TemporalPropertiesWidget(QWidget* parent = 0,  Qt::WindowFlags f = 0);

          /*!
            \brief Destructor
          */
          ~TemporalPropertiesWidget();

          /*!
            \brief Returns a pointer to the widget's form

            \return A TemporalPropertiesWidgetForm type pointer to the widget's form.
            \note The caller will not take ownership of the returned pointer.
          */
          Ui::TemporalPropertiesWidgetForm* getForm();

          /*!
            \brief Returns the index of the Phenomenom Time

            \return The index of the Phenomenom Time.
            \note Will return an invalid index if the dataSeType hasn't been given.
          */
          int getPhenomenonTime();

          /*!
            \brief Returns the index of the Result Time

            \return The index of the result Time.
            \note Will return an invalid index if the dataSeType hasn't been given.
          */
          int getResultTime();

          /*!
            \brief Returns the index of the Valid Time

            \return The index of the valid Time.
            \note Will return an invalid index if the dataSeType hasn't been given.
          */
          int getValidTime();

          /*!
            \brief Adjusts the widget values according to the given datasettype

            \param dataType The datasetType that will be used to configure the widget.
          */
          void setUp(const te::da::DataSetTypePtr dataType);

        protected slots:

          void onPropertyComboBoxIndexChanged(QString text);

        private:

          std::auto_ptr<Ui::TemporalPropertiesWidgetForm>  m_ui;        //!< The widget's form.
          te::da::DataSetTypePtr                           m_dataType;  //!< The datasetType that will be used to generate the spatio-temporal layer.

       };
    } // end namespace widgets
  }   // end namespace qt
}     // end namespace te

#endif  // __TERRALIB_QT_WIDGETS_INTERNAL_TIMEPROPERTIESWIDGET_H