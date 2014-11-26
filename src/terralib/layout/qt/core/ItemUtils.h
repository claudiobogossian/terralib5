/*  Copyright (C) 2001-2014 National Institute For Space Research (INPE) - Brazil.

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
  \file ItemUtils.h
   
  \brief 

  \ingroup layout
*/

#ifndef __TERRALIB_LAYOUT_INTERNAL_ITEM_UTILS_H 
#define __TERRALIB_LAYOUT_INTERNAL_ITEM_UTILS_H

// TerraLib
#include "../../core/Config.h"
#include "../../../geometry/Point.h"

// STL
#include <vector>
#include <string>
#include <map>

// Qt
#include <QList>

class QGraphicsItem;
class QGraphicsScene;
class QFont;

namespace te
{
  namespace layout
  {
    class MapItem;
    class EnumType;
    class MapModel;

    class TELAYOUTEXPORT ItemUtils
    {
      public:

        ItemUtils( QGraphicsScene* scene );

        virtual ~ItemUtils();

        virtual std::vector<MapItem*> getMapItemList(bool selected = false);

        virtual MapItem* getMapItem(std::string name);

        virtual std::vector<std::string> mapNameList(bool selected = false);

        virtual int countType(te::layout::EnumType* type);

        virtual int maxTypeId(te::layout::EnumType* type);

        virtual bool isCurrentMapTools();

        virtual void setCurrentToolInSelectedMapItems(EnumType* mode);

        virtual void createTextGridAsObject();

        virtual void createTextMapAsObject();

        virtual void createLegendChildAsObject();

      protected:

        virtual QList<QGraphicsItem*> getItems(bool selected = false);

        virtual void createTextItemFromObject(std::map<te::gm::Point*, std::string> map, QFont* ft = 0);

        virtual void createLegendChildItemFromLegend(std::map<te::gm::Point*, std::string> map, MapModel* visitable);

        QGraphicsScene* m_scene;
    };
  }
}

#endif 