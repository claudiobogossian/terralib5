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
  \file terralib/qt/af/ProjectInfo.h

  \brief 
*/

#ifndef __TERRALIB_QT_AF_INTERNAL_PROJECTINFOWIDGET_H
#define __TERRALIB_QT_AF_INTERNAL_PROJECTINFOWIDGET_H

#include <QtGui/QWidget>

#include "Config.h"

// Forward declaration
namespace Ui
{
  class ProjectInfoWidgetForm;
}

namespace te
{
  namespace qt
  {
    namespace af
    {
      // Forward declaration
      class Project;

      class TEQTAFEXPORT ProjectInfoWidget : public QWidget
      {
        public:

          ProjectInfoWidget(QWidget* parent=0);

          ~ProjectInfoWidget();

          void setProject(Project* proj);

          void updateProjectInfo();

        protected:

          Project* m_proj;

        private:

          Ui::ProjectInfoWidgetForm* m_ui;
      };
    }
  }
}

#endif //__TERRALIB_QT_AF_INTERNAL_PROJECTINFOWIDGET_H