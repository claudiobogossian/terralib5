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
  \file VisualizationArea.h
   
  \brief Creates the viewing area. Ex.: creation of the sheet of paper.

  \ingroup layout
*/

#ifndef __TERRALIB_LAYOUT_INTERNAL_VISUALIZATION_AREA_H 
#define __TERRALIB_LAYOUT_INTERNAL_VISUALIZATION_AREA_H

#include <QObject>
#include "../../../geometry/Envelope.h"
#include "../../core/Config.h"

namespace te
{
  namespace layout
  {
    class Scene;
    /*!
    \brief Creates the viewing area. Ex.: creation of the sheet of paper.
    
    \ingroup layout
    */
    class TELAYOUTEXPORT VisualizationArea : public QObject
    {
      Q_OBJECT //for slots/signals

    public:

      VisualizationArea(Scene* scene, te::gm::Envelope boxArea);

      virtual ~VisualizationArea();

      virtual void changeBoxArea(te::gm::Envelope boxArea);

      virtual void build();

    protected:

      virtual void createPaper();

    protected:

      te::gm::Envelope m_boxArea;
      Scene*           m_scene;

    };
  }
}

#endif
