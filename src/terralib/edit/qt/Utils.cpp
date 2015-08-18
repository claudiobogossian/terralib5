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
#include "../../geometry/Coord2D.h"
#include "../../geometry/Envelope.h"
#include "../../geometry/Geometry.h"
#include "../../geometry/GeometryCollection.h"
#include "../../geometry/GeometryProperty.h"
#include "../../geometry/LineString.h"
#include "../../geometry/MultiPoint.h"
#include "../../geometry/Point.h"
#include "../../geometry/Polygon.h"
#include "../../geometry/Utils.h"
#include "../../qt/widgets/canvas/Canvas.h"
#include "../../qt/widgets/Utils.h"
#include "../../srs/Config.h"
#include "../Utils.h"
#include "Utils.h"

//#include "../../srs/Config.h"
#include "../Feature.h"
//#include "../SnapManager.h"
#include "Utils.h"

// Qt
#include <QColor>

// STL
#include <cassert>

QPointF te::edit::GetPosition(QMouseEvent* e)
{
#if QT_VERSION >= 0x050000
  return e->localPos();
#else
  return e->posF();
#endif
}
