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
  \file terralib/edit/qt/Utils.cpp
   
  \brief Utility Qt functions for TerraLib Edit module.
*/

// TerraLib
#include "../../dataaccess/dataset/DataSet.h"
#include "../../dataaccess/dataset/ObjectId.h"
#include "../../dataaccess/utils/Utils.h"
#include "../../datatype/SimpleData.h"
#include "../../geometry.h"
#include "../../qt/widgets/canvas/Canvas.h"
#include "../../qt/widgets/Utils.h"
#include "../../srs/Config.h"
#include "../Utils.h"
#include "Utils.h"

// Qt
#include <QColor>
#include <QUndoCommand>

// STL
#include <cassert>

//removing
#include "../../common/Exception.h"
#include "../../common/Translator.h"
#include "../../dataaccess/dataset/ObjectId.h"
#include "../../dataaccess/dataset/ObjectIdSet.h"
#include "../../dataaccess/datasource/DataSourceInfoManager.h"
#include "../../dataaccess/utils/Utils.h"
#include "../../datatype/SimpleData.h"
#include "../../edit/Feature.h"
#include "../../edit/Repository.h"
#include "../../edit/RepositoryManager.h"
#include "../../edit/qt/core/UndoStackManager.h"
#include "../../geometry/GeometryProperty.h"
#include "../../geometry/GeometryCollection.h"
#include "../../maptools/DataSetLayer.h"
#include "../../memory/DataSet.h"
#include "../../memory/DataSetItem.h"
#include "../../qt/widgets/canvas/MapDisplay.h"
#include "../../qt/af/events/MapEvents.h"

//#include "../af/ApplicationController.h"
//#include "../af/events/LayerEvents.h"
//#include "../af/events/MapEvents.h"

// QT
#include <QMessageBox>

QPointF te::edit::GetPosition(QMouseEvent* e)
{
#if QT_VERSION >= 0x050000
  return e->localPos();
#else
  return e->posF();
#endif
}

te::gm::Geometry* te::edit::convertGeomType(const te::map::AbstractLayerPtr& layer, te::gm::Geometry* geom)
{
  te::gm::Geometry* geomConvert = 0;

  // Get the geometry type of layer
  std::auto_ptr<te::da::DataSetType> dt = layer->getSchema();
  te::gm::GeometryProperty* geomProp = te::da::GetFirstGeomProperty(dt.get());

  switch (geomProp->getGeometryType())
  {
    case te::gm::MultiPolygonType:
    {
      te::gm::MultiPolygon* mp = 0;

      if (geom->getGeomTypeId() == te::gm::MultiPolygonType)
        mp = dynamic_cast<te::gm::MultiPolygon*>(geom);
      else
      {
        mp = new te::gm::MultiPolygon(0, te::gm::MultiPolygonType);
        mp->add(geom);
      }

      geomConvert = mp;

      break;
    }
    case te::gm::PolygonType:
    {
      te::gm::Polygon* p = dynamic_cast<te::gm::Polygon*>(geom);

      geomConvert = p;

      break;
    }
    //others type
    //MultiLine
    //...
  }

  //projection
  if (geomConvert->getSRID() == layer->getSRID())
    return geomConvert;

  //else, need conversion...
  geomConvert->transform(layer->getSRID());

  return geomConvert;

}
