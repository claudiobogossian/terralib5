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
  \file terralib/qt/widgets/charts/chartStyleWidget.h

  \brief A dialog used to customize the style parameters of a chart
*/

#ifndef __TERRALIB_QT_WIDGETS_INTERNAL_CHARTSTYLEWIDGET_H
#define __TERRALIB_QT_WIDGETS_INTERNAL_CHARTSTYLEWIDGET_H

//TerraLib
#include "../Config.h"

// Qt
#include <QtGui/QDialog>

//STL
#include <memory>

namespace Ui { class chartStyleWidgetForm; }

namespace te
{
  namespace qt
  {
    namespace widgets
    {

    class ColorPickerToolButton;
    class ChartStyle;
    class ChartDisplay;

    /*!
        \class Chart Style

        \brief A wdiget used to customize a chart's style parameters.
      */
      class TEQTWIDGETSEXPORT ChartStyleWidget : public QWidget
      {

        Q_OBJECT

        public:

          ChartStyleWidget(QWidget* parent = 0, Qt::WindowFlags f = 0, QString title = "", QString PropertyX = "", QString PropertyY = "");

          ~ChartStyleWidget();

          static te::qt::widgets::ChartStyle* getStyle(te::qt::widgets::ChartStyle* initial, QWidget* parent, Qt::WindowFlags f, QString title, QString PropertyX, QString PropertyY);

          void setStyle(te::qt::widgets::ChartStyle* newStyle);

          te::qt::widgets::ChartStyle* adjustStyle();

        protected slots:

          void onTitleLineEditFinish();
          void onTitleStylePushButtonClicked();
          void onLabelStylePushButtonClicked();
          void onGridCheckBoxToggled();
          void onColorChanged(const QColor& color);

        private:

          std::auto_ptr<Ui::chartStyleWidgetForm>  m_ui;
          ColorPickerToolButton*               m_colorPicker;   //!< The color picker used to customise the color of several chart parameters.
          te::qt::widgets::ChartStyle*         m_chartStyle;    //!< The display's style that will be configured by this dialog.
      };
    } // end namespace widgets
  }   // end namespace qt
}     // end namespace te

#endif  // __TERRALIB_QT_WIDGETS_INTERNAL_CHARTSTYLEWIDGET_H
