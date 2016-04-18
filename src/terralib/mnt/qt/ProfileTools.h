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
\file terralib/mnt/qt/ProfileTools.h

\brief This file defines the profile trajectory edition tools
*/

#ifndef __TERRALIB_MNT_INTERNAL_PROFILETOOLS_H
#define __TERRALIB_MNT_INTERNAL_PROFILETOOLS_H

// TerraLib
#include "../core/Config.h"
#include "../../qt/widgets/tools/AbstractTool.h"

// Qt
#include <QMouseEvent>

namespace te
{
  namespace mnt
  {
    enum TEMNTEXPORT EditType
    {
      LINE_SELECT,
      LINE_CREATE,
      VERTEX_MOVE,
      VERTEX_ADD,
      VERTEX_DELETE,
      NONE
    };

    class TEMNTEXPORT ProfileTools : public te::qt::widgets::AbstractTool
    {

      public:
        ProfileTools(te::qt::widgets::MapDisplay* display, EditType type, QObject* parent = 0);

        ~ProfileTools();

        void setType(EditType t);

        void setLines(std::vector<te::gm::LineString*> &l) { m_lines = l; }

        bool mousePressEvent(QMouseEvent* e);

        bool mouseMoveEvent(QMouseEvent* e);

        bool mouseReleaseEvent(QMouseEvent* e);

        bool mouseDoubleClickEvent(QMouseEvent* e);

      protected:
        EditType m_type;
        std::vector<te::gm::LineString*> m_lines;
        te::gm::LineString* m_selectline;
    };
  }
}

#endif //__TERRALIB_MNT_INTERNAL_PROFILETOOLS_H
