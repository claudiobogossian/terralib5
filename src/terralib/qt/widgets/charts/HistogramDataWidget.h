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
  \file terralib/qt/widgets/charts/histogramDataWidget.h

  \brief A widget used to adjust a histogram's input data.
*/

#ifndef __TERRALIB_QT_WIDGETS_INTERNAL_HISTOGRAMDATAWIDGET_H
#define __TERRALIB_QT_WIDGETS_INTERNAL_HISTOGRAMDATAWIDGET_H

//TerraLib
#include "../Config.h"

// Qt
#include <QtGui/QWidget>

//STL
#include <memory>

namespace Ui { class HistogramDataWidgetForm; }

namespace te
{

  namespace da
  {
    //forward declarations
    class DataSet;
  }

  namespace qt
  {
    namespace widgets
    {

    //forward declarations
    class Histogram;
    class ChartDisplay;

    /*!
        \class Histogram Creator

        \brief A widget used to adjust a histogram's input data.
      */
      class TEQTWIDGETSEXPORT HistogramDataWidget : public QWidget
      {

        Q_OBJECT

        public:

          HistogramDataWidget(te::da::DataSet* dataSet, QWidget* parent = 0,  Qt::WindowFlags f = 0);

          ~HistogramDataWidget();

          Ui::HistogramDataWidgetForm* getForm();

          te::qt::widgets::Histogram* getHistogram();

        protected slots:

          void onPropertyComboBoxIndexChanged(QString text);

        private:

          std::auto_ptr<Ui::HistogramDataWidgetForm>  m_ui;       //!< The widget form.
          std::auto_ptr<te::da::DataSet>            m_dataSet;  //!< The dataset that will be used to generate the histogram graph.
       };
    } // end namespace widgets
  }   // end namespace qt
}     // end namespace te

#endif  // __TERRALIB_QT_WIDGETS_INTERNAL_HISTOGRAMDATAWIDGET_H