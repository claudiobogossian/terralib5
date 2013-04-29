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
  \file terralib/qt/widgets/charts/ScatterFrame.h

  \brief A frame used to adjust a histogram's chart parameters, weather it is about it's data or it's visual style.
*/

#ifndef __TERRALIB_QT_WIDGETS_INTERNAL_SCATTERFRAME_H
#define __TERRALIB_QT_WIDGETS_INTERNAL_SCATTERFRAME_H

// TerraLib
#include "../Config.h"
#include "ChartWidget.h"

#include <memory>

namespace Ui { class ScatterFrameWidgetForm; }

namespace te
{
  namespace qt
  {
    namespace widgets
      {
      /*!
        \class ScatterFrame

        \brief A frame for setting display options.
      */
      class TEQTWIDGETSEXPORT ScatterFrame : public ChartWidget
      {
        Q_OBJECT

        public:

          ScatterFrame(QWidget* parent = 0);

          ~ScatterFrame();

          void setDataSet(te::da::DataSet* newDataSet);

        private:

          std::auto_ptr<Ui::ScatterFrameWidgetForm>  m_ui;        //!< The widgetv form.
      };

    }   // end namespace widgets
  }     // end namespace qt
}       // end namespace te

#endif  // __TERRALIB_QT_WIDGETS_INTERNAL_SCATTERFRAME_H

