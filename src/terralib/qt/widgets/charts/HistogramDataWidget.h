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
  \file terralib/qt/widgets/charts/histogramDataWidget.h

  \brief A widget used to adjust a histogram's input data.
*/

#ifndef __TERRALIB_QT_WIDGETS_INTERNAL_HISTOGRAMDATAWIDGET_H
#define __TERRALIB_QT_WIDGETS_INTERNAL_HISTOGRAMDATAWIDGET_H

//TerraLib
#include "../Config.h"

// Qt
#include <QWidget>

//STL
#include <memory>

namespace Ui { class HistogramDataWidgetForm; }

namespace te
{
    //forward declarations
  namespace da  { class DataSet; class DataSetType; }

  namespace qt
  {
    namespace widgets
    {

    //forward declarations
    class Histogram;
    class ChartDisplay;

    /*!
        \class HistogramDataWidget

        \brief A widget used to adjust a histogram's input data.
      */
      class TEQTWIDGETSEXPORT HistogramDataWidget : public QWidget
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
          HistogramDataWidget(te::da::DataSet* dataSet, te::da::DataSetType* dataType, QWidget* parent = 0,  Qt::WindowFlags f = 0);

          /*!
            \brief Destructor
          */
          ~HistogramDataWidget();

          /*!
            \brief Returns a pointer to the widget's form

            \return A HistogramDataWidgetForm type pointer to the widget's form.
            \note The caller will not take ownership of the returned pointer.
          */
          Ui::HistogramDataWidgetForm* getForm();

          /*!
            \brief Returns a pointer to the widget's form

            \return A HistogramDataWidgetForm type pointer to the widget's form.
            \note The caller will take ownership of the returned pointer.
          */
          te::qt::widgets::Histogram* getHistogram();

          /*!
            \brief Sets the property to be used in order to generate the histogram

            \note This function is intented to prevent the user from selecting a property through this widget in special cases,
                  if that is not the intended behaviour, do not call this function.
          */
          void setHistogramProperty(int propId);

          /*!
          \brief Returns the name of the chosen summary function. Default is None

          \return The name of the chosen summary function.
          */
          std::string getSummaryFunction();

        protected slots:

          void onPropertyComboBoxIndexChanged(QString text);

        private:

          std::auto_ptr<Ui::HistogramDataWidgetForm>  m_ui;        //!< The widget's form.
          std::auto_ptr<te::da::DataSet>              m_dataSet;   //!< The dataset that will be used to generate the histogram graph.
          std::auto_ptr<te::da::DataSetType>          m_dataType;  //!< The type of the dataset that will be used to generate the histogram graphic.
       };
    } // end namespace widgets
  }   // end namespace qt
}     // end namespace te

#endif  // __TERRALIB_QT_WIDGETS_INTERNAL_HISTOGRAMDATAWIDGET_H