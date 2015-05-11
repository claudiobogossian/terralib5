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
  \file terralib/qt/widgets/charts/ChartFrame.h

  \brief A frame used to adjust a Chart's visual style.
*/

#ifndef __TERRALIB_QT_WIDGETS_INTERNAL_CHARTFRAME_H
#define __TERRALIB_QT_WIDGETS_INTERNAL_CHARTFRAME_H

// TerraLib
#include "../Config.h"
#include "ChartWidget.h"

#include <memory>

namespace Ui { class ChartStyleFrameWidgetForm; }

namespace te
{
  namespace qt
  {
    namespace widgets
      {
      //Forward declarations
      class ChartDisplay;
      class ChartStyleWidget;
      /*!
        \class ChartFrame

        \brief A frame for setting display options.
      */
      class TEQTWIDGETSEXPORT ChartStyleFrame : public ChartWidget
      {
        Q_OBJECT

        public:

          /*!
            \brief Constructor

            It constructs a chart display with the given title.

            \param parent this widget's parent
          */
          ChartStyleFrame(QWidget* parent = 0);

          /*! 
            \brief Destructor.
          */
          ~ChartStyleFrame();

          /*!
            \brief Returns a pointer to the ChartDisplay being configured

            \return A ChartDisplay type pointer to the display being configured
            \note The caller will take ownership of the returned pointer. 
          */
          te::qt::widgets::ChartDisplay* getDisplay();

          /*!
            \brief It sets the ChartDisplay being configured

            \param newDisplay The new ChartDisplay.
            \note It will not take ownership of the given pointer 
          */
          void setDisplay(te::qt::widgets::ChartDisplay* newDisplay);

        private:

          std::auto_ptr<Ui::ChartStyleFrameWidgetForm>  m_ui;           //!< The Widget form.
          ChartDisplay*                                 m_display;      //!< The display that will be configured by this widget.
          ChartStyleWidget*                             m_styleWidget;  //!< The widget used to configure the display's style
      };

    }   // end namespace widgets
  }     // end namespace qt
}       // end namespace te

#endif  // __TERRALIB_QT_WIDGETS_INTERNAL_CHARTFRAME_H

