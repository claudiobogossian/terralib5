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
  \file terralib/qt/widgets/utils/HorizontalSliderWidget.h

  \brief ...
*/

#ifndef __TERRALIB_QT_WIDGETS_UTILS_INTERNAL_HORIZONTALSLIDERWIDGET_H
#define __TERRALIB_QT_WIDGETS_UTILS_INTERNAL_HORIZONTALSLIDERWIDGET_H

// TerraLib
#include "../Config.h"

// STL
#include <memory>

// Qt
#include <QWidget>

class QSilder;

namespace Ui { class HorizontalSliderWidgetForm; }

namespace te
{
  namespace qt
  {
    namespace widgets
    {
      /*!
        \class HorizontalSliderWidget

        \brief ...
      */
      class TEQTWIDGETSEXPORT HorizontalSliderWidget : public QWidget
      {
        Q_OBJECT

        public:

          HorizontalSliderWidget(QWidget* parent = 0, Qt::WindowFlags f = 0);

          ~HorizontalSliderWidget();

          Ui::HorizontalSliderWidgetForm* getForm() const;

          void setTitle(QString value);

          void setMinMaxValues(int min, int max);

          void setDefaultValue(int value);

          void setCurrentValue(int value);

          int getValue();

          void setDefaultButtonVisible(bool visible);

          void setSliderLabelVisible(bool visible);

        protected slots:

          void onDefaultButtonPressed();

          void onValueChanged(int value);

          void onSliderReleased();


        signals:

          /*! This signal is emitted when the slider value was changed. */
          void sliderValueChanged(int);
          void sliderReleased();

        private:

          std::auto_ptr<Ui::HorizontalSliderWidgetForm> m_ui;

          int m_minValue;           //!< Slider Minimum value (default 0)
          int m_maxValue;           //!< Slider Maximum value (default 99)
          int m_defaultValue;       //!< Slider default value (default 0)
      }; 

    } // end namespace widgets
  }   // end namespace qt
}     // end namespace te

#endif  // __TERRALIB_QT_WIDGETS_UTILS_INTERNAL_HORIZONTALSLIDERWIDGET_H

