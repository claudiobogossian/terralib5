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
#include "../../edit/qt/tools/VertexTool.h"
#include "../../edit/Utils.h"
#include "../../sam/rtree/Index.h"

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
      LINE_DELETE,
      LINE_INVERT,
      NONE
    };

    class TEMNTEXPORT ProfileTools : public te::edit::VertexTool
    {

      public:
        ProfileTools(te::qt::widgets::MapDisplay* display, const te::map::AbstractLayerPtr& layer, QObject* parent = 0);

        ~ProfileTools();

        void setType(EditType t);

        EditType getType() { return m_type; }

        void setLines(std::vector<te::gm::LineString*> &l);

        std::vector<te::gm::LineString*> getLines() { return m_lines; }

        bool mousePressEvent(QMouseEvent* e);

        bool mouseMoveEvent(QMouseEvent* e);

        bool mouseReleaseEvent(QMouseEvent* e);

        bool mouseDoubleClickEvent(QMouseEvent* e);

    protected:
        EditType m_type;

    };
  }
}

#endif //__TERRALIB_MNT_INTERNAL_PROFILETOOLS_H
