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
   
  \brief Class applying the alignment in one or more components. Ex .: send to back, bring to front, align right, align bottom.

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
    \brief Class applying the alignment in one or more components. Ex .: send to back, bring to front, align right, align bottom.
    
    \ingroup layout
    */
    class TELAYOUTEXPORT AlignItems
    {
      public:

        /*!
          \brief Constructor
        */
        AlignItems( QGraphicsScene* scene, PaperConfig* config );

        /*!
          \brief Destructor
        */
        virtual ~AlignItems();

        /*!
          \brief
        */
        virtual void bringToFront();

        /*!
          \brief
        */
        virtual void sendToBack();

        /*!
          \brief
        */
        virtual void alignLeft();

        /*!
          \brief
        */
        virtual void alignRight();

        /*!
          \brief
        */
        virtual void alignTop();

        /*!
          \brief
        */
        virtual void alignBottom();

        /*!
          \brief
        */
        virtual void alignCenterHorizontal();

        /*!
          \brief
        */
        virtual void alignCenterVertical();

        /*!
          \brief

          \return
        */
        virtual QRectF getSelectionItemsBoundingBox();

        /*!
          \brief

          \return
        */
        int getMinimumZValue();

        /*!
          \brief  

          \param
        */
        void setMinimumZValue(int minimum);

      protected:

        QGraphicsScene* m_scene;
        PaperConfig*    m_config;
        int             m_minimunZValue;
    };
  }
}

#endif 
