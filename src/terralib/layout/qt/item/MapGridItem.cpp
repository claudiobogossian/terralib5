/*  Copyright (C) 2014-2014 National Institute For Space Research (INPE) - Brazil.

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
  \file MapGridItem.cpp
   
  \brief 

  \ingroup layout
*/

// TerraLib
#include "MapGridItem.h"
#include "../../core/pattern/mvc/ItemModelObservable.h"
#include "../../core/pattern/mvc/ItemObserver.h"
#include "../../core/pattern/mvc/ItemController.h"
#include "../../../color/RGBAColor.h"
#include "../../../qt/widgets/Utils.h"
#include "../../../geometry/Envelope.h"
#include "../../../common/STLUtils.h"
#include "../../item/MapGridModel.h"

te::layout::MapGridItem::MapGridItem( ItemController* controller, Observable* o ) :
  MapItem(controller, o)
{
  MapGridModel* model = dynamic_cast<MapGridModel*>(m_model);
  if(model)
  {
    te::color::RGBAColor clr = model->getMapBackgroundColor();
    QColor qcolor;
    qcolor.setRed(clr.getRed());
    qcolor.setGreen(clr.getGreen());
    qcolor.setBlue(clr.getBlue());
    qcolor.setAlpha(clr.getAlpha());
    m_mapDisplay->setBackgroundColor(qcolor);
    m_mapDisplay->refresh();
  }

  m_nameClass = std::string(this->metaObject()->className());
}

te::layout::MapGridItem::~MapGridItem()
{

}
