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
  \file GeneralConfigWidget.h

  \brief A frame for setting general options.
*/

#ifndef __TERRALIB_QT_AF_GENERALCONFIGWIDGET_H
#define __TERRALIB_QT_AF_GENERALCONFIGWIDGET_H

// TerraLib
#include "../Config.h"

// Qt
#include "AbstractSettingWidget.h"

// STL
#include <memory>

// Forward declaration
namespace Ui
{
  class GeneralConfigWidgetForm;
}

namespace te
{
  namespace qt
  {
// Forward declaration
    namespace widgets
    {
      class ColorPickerToolButton;
    }

    namespace af
    {
      /*!
        \class DisplayWidget

        \brief A frame for setting general options.
      */
      class TEQTAFEXPORT GeneralConfigWidget : public AbstractSettingWidget
      {
        Q_OBJECT

        public:

          GeneralConfigWidget(QWidget* parent = 0);

          ~GeneralConfigWidget();

          void saveChanges();

          void resetState();

          void getHelpInformations(QString& ns, QString& helpFile);

        private:

          void initialize();

          void setupSRSUi();

        private slots:

          void onDefaultSRSToolButtonPressed();

          void onSelectionColorChanged(const QColor& color);

          void onToolBarIconSizeValueChanged(int value);

          void onIconThemeCurrentIndexChanged(const QString& value);

        private:

          std::auto_ptr<Ui::GeneralConfigWidgetForm> m_ui;
          te::qt::widgets::ColorPickerToolButton* m_colorPicker;
          int m_defaultSRID;
          bool m_needRestart;
      };

    }   // end namespace af
  }     // end namespace qt
}       // end namespace te

#endif  // __TERRALIB_QT_AF_GENERALCONFIGWIDGET_H
