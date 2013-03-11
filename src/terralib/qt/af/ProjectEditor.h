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
  \file terralib/qt/af/ProjectEditor.h

  \brief 
*/

#ifndef __TERRALIB_QT_AF_INTERNAL_PROJECTEDITOR_H
#define __TERRALIB_QT_AF_INTERNAL_PROJECTEDITOR_H

#include <QtGui/QDialog>

#include "Config.h"

namespace te
{
  namespace qt
  {
    namespace af
    {
      // Forward declaration
      class Project;
      class ProjectInfo;

      class TEQTAFEXPORT ProjectEditor : public QDialog
      {
        Q_OBJECT

        public:

          ProjectEditor(QWidget* parent=0);

          ~ProjectEditor();

          void setProject(Project* proj);

        protected slots:

          void onOkPushButtonPressed();

          void onHelpPushButtonPressed();

        protected:

          ProjectInfo* m_info;
      };
    }
  }
}


#endif //__TERRALIB_QT_AF_INTERNAL_PROJECTEDITOR_H