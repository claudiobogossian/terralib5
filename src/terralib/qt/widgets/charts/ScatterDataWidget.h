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
  \file terralib/qt/widgets/charts/scatterDataWidget.h

  \brief A widget used to adjust a scatter's input data.
*/

#ifndef __TERRALIB_QT_WIDGETS_INTERNAL_SCATTERDATAWIDGET_H
#define __TERRALIB_QT_WIDGETS_INTERNAL_SCATTERDATAWIDGET_H

//TerraLib
#include "../Config.h"

// Qt
#include <QtGui/QWidget>

//STL
#include <memory>

namespace Ui { class ScatterDataWidgetForm; }


namespace te
{

  namespace da
  {
    class DataSet;
  }

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

          ScatterDataWidget(te::da::DataSet* dataSet, QWidget* parent = 0, Qt::WindowFlags f = 0);

          ~ScatterDataWidget();

          Ui::ScatterDataWidgetForm* getForm();

          te::qt::widgets::Scatter* getScatter();

        protected slots:

        private:

          std::auto_ptr<Ui::ScatterDataWidgetForm>  m_ui;       //!< THe widget Form.
          std::auto_ptr<te::da::DataSet>            m_dataSet;  //!< The dataset that will be used to generate the Scatter graph.
      };
    } // end namespace widgets
  }   // end namespace qt
}     // end namespace te

#endif  // __TERRALIB_QT_WIDGETS_INTERNAL_SCATTERDATAWIDGET_H
