/*  Copyright (C) 2011-2012 National Institute For Space Research (INPE) - Brazil.

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
  \file terralib/qt/widgets/se/BasicStrokeWidget.h

  \brief A widget used to build a basic stroke element.
*/

#ifndef __TERRALIB_QT_WIDGETS_SE_INTERNAL_BASICSTROKEWIDGET_H
#define __TERRALIB_QT_WIDGETS_SE_INTERNAL_BASICSTROKEWIDGET_H

// TerraLib
#include "../Config.h"

// Qt
#include <QtGui/QWidget>

// STL
#include <memory>
#include <vector>
#include <string>

// Forward declaraion
class QButtonGroup;
namespace Ui { class BasicStrokeWidgetForm; }

namespace te
{
// Forward declarations
  namespace se
  {
    class Stroke;
  }

  namespace qt
  {
    namespace widgets
    {
      /*!
        \class BasicStrokeWidget

        \brief A widget used to build a basic stroke element.
      */
      class TEQTWIDGETSEXPORT BasicStrokeWidget : public QWidget
      {
        Q_OBJECT

        public:

          BasicStrokeWidget(QWidget* parent = 0, Qt::WindowFlags f = 0);

          ~BasicStrokeWidget();

        public:

          void setStroke(const te::se::Stroke* stroke);

          te::se::Stroke* getStroke() const;

        protected:

          void initialize();

          void updateUi();

          void updateUiStrokeColor();

          void updateUiDashStyle(const std::string& pattern);

          void updateUiJoinStyle(const std::string& style);

          void updateUiCapStyle(const std::string& style);

          void fillStrokeDashStyleComboBox();

        protected slots:

          void onStrokeColorPushButtonClicked();

          void onStrokeOpacitySliderValueChanged(int value);

          void onStrokeWidthDoubleSpinBoxValueChanged(const QString& text);

          void onStrokeDashComboBoxCurrentIndexChanged(int index);

          void onStrokeJoinStyleChanged(int style);

          void onStrokeCapStyleChanged(int style);

        signals:

          void strokeChanged();

        private:

          std::auto_ptr<Ui::BasicStrokeWidgetForm> m_ui;

          QButtonGroup* m_strokeJoinStyleButtonGroup;
          QButtonGroup* m_strokeCapStyleButtonGroup;

          QColor m_color;

          te::se::Stroke* m_stroke;

          std::vector<std::string> m_dashes;
      }; 

    } // end namespace widgets
  }   // end namespace qt
}     // end namespace te

#endif  // __TERRALIB_QT_WIDGETS_SE_INTERNAL_BASICSTROKEWIDGET_H
