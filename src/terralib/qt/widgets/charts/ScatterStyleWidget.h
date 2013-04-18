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
  \file terralib/qt/widgets/charts/scatterStyleWidget.h

  \brief A widget used to adjust a scatter's style.
*/

#ifndef __TERRALIB_QT_WIDGETS_INTERNAL_SCATTERSTYLEWIDGET_H
#define __TERRALIB_QT_WIDGETS_INTERNAL_SCATTERSTYLEWIDGET_H

//TerraLib
#include "../Config.h"

// Qt
#include <QtGui/QWidget>

//STL
#include <memory>

namespace Ui { class ScatterStyleWidgetForm; }

namespace te
{
  namespace qt
  {
    namespace widgets
    {

    //forward declarations
    class Symbol;

    /*!
        \class Scatter creator

        \brief A wdiget used to customize a Scatter parameters.
      */
      class TEQTWIDGETSEXPORT ScatterStyleWidget : public QWidget
      {

        Q_OBJECT

        public:

          ScatterStyleWidget(QWidget* parent = 0, Qt::WindowFlags f = 0);

          ~ScatterStyleWidget();

        protected slots:

          void onPlotStylePushButtonClicked();

        private:

          std::auto_ptr<Ui::ScatterStyleWidgetForm>  m_ui;    //!< The iwdget form.
          te::qt::widgets::Symbol*          m_symbol;         //!< The symbol that will be used to plot the scatter's points.
      };
    } // end namespace widgets
  }   // end namespace qt
}     // end namespace te

#endif  // __TERRALIB_QT_WIDGETS_INTERNAL_SCATTERSTYLEWIDGET_H