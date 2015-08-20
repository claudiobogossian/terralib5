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
  \file AlignItems.h
   
  \brief Class applying the alignment of one or more objects. Ex .: send to back, bring to front, align right, align bottom.

  \ingroup layout
*/

#ifndef __TERRALIB_LAYOUT_INTERNAL_ALIGN_ITEMS_H
#define __TERRALIB_LAYOUT_INTERNAL_ALIGN_ITEMS_H

// TerraLib
#include "../../core/Config.h"
#include "../../core/PaperConfig.h"

// Qt
#include <QRectF>

class QGraphicsScene;

namespace te
{
  namespace layout
  {
    /*!
    \brief Class applying the alignment of one or more objects. Ex .: send to back, bring to front, align right, align bottom.
    
    \ingroup layout
    */
    class TELAYOUTEXPORT AlignItems
    {
      public:

        AlignItems( QGraphicsScene* scene, PaperConfig* config );

        virtual ~AlignItems();

        virtual void bringToFront();

        virtual void sendToBack();

        virtual void alignLeft();

        virtual void alignRight();

        virtual void alignTop();

        virtual void alignBottom();

        virtual void alignCenterHorizontal();

        virtual void alignCenterVertical();

        virtual QRectF getSelectionItemsBoundingBox();

      protected:

        QGraphicsScene* m_scene;
        PaperConfig*    m_config;
    };
  }
}

#endif 
