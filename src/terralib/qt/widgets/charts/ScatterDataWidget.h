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
  \file terralib/qt/widgets/charts/scatterDataWidget.h

  \brief A widget used to adjust a scatter's input data.
*/

#ifndef __TERRALIB_QT_WIDGETS_INTERNAL_SCATTERDATAWIDGET_H
#define __TERRALIB_QT_WIDGETS_INTERNAL_SCATTERDATAWIDGET_H

//TerraLib
#include "../Config.h"

// Qt
#include <QWidget>

//STL
#include <memory>

namespace Ui { class ScatterDataWidgetForm; }


namespace te
{
    //forward declarations
  namespace da  { class DataSet; class DataSetType; }

  namespace qt
  {
    namespace widgets
    {

    //forward declarations
    class Scatter;
    /*!
        \class Scatter creator

        \brief A widget used to adjust a scatter's input data.
      */
      class TEQTWIDGETSEXPORT ScatterDataWidget : public QWidget
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
          ScatterDataWidget(te::da::DataSet* dataSet, te::da::DataSetType* dataType, QWidget* parent = 0, Qt::WindowFlags f = 0);

          /*!
            \brief Destructor
          */
          ~ScatterDataWidget();

          /*!
            \brief Returns a pointer to the widget's form

            \return A ScatterDataWidgetForm type pointer to the widget's form.
            \note The caller will not take ownership of the returned pointer.
          */
          Ui::ScatterDataWidgetForm* getForm();

          /*!
            \brief Returns a pointer to the widget's form

            \return A HistogramDataWidgetForm type pointer to the widget's form.
            \note The caller will take ownership of the returned pointer.
          */
          te::qt::widgets::Scatter* getScatter();

        protected slots:

        private:

          std::auto_ptr<Ui::ScatterDataWidgetForm>  m_ui;        //!< THe widget Form.
          std::auto_ptr<te::da::DataSet>            m_dataSet;   //!< The dataset that will be used to generate the Scatter graphic.
          std::auto_ptr<te::da::DataSetType>        m_dataType;  //!< The type of the dataset that will be used to generate the Scatter graphic.
      };
    } // end namespace widgets
  }   // end namespace qt
}     // end namespace te

#endif  // __TERRALIB_QT_WIDGETS_INTERNAL_SCATTERDATAWIDGET_H
