/*  Copyright (C) 2001-2009 National Institute For Space Research (INPE) - Brazil.

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
  \file terralib/edit/IdGeom.h
   
  \brief Struct that represents an identified geometry.
*/

#ifndef __TERRALIB_EDIT_INTERNAL_IDGEOM_H
#define __TERRALIB_EDIT_INTERNAL_IDGEOM_H

// TerraLib
#include "../dataaccess/dataset/ObjectId.h"
#include "../geometry/Geometry.h"
#include "Config.h"

// STL
#include <cassert>

namespace te
{
  namespace edit
  {
    struct TEEDITEXPORT IdGeom
    {
      IdGeom(te::da::ObjectId* id, te::gm::Geometry* geom, bool isOwner = false)
        : m_id(id),
          m_geom(geom),
          m_isOwner(isOwner)
      {
      }

      ~IdGeom()
      {
        if(m_isOwner)
        {
          delete m_id;
          delete m_geom;
        }
      }

      te::da::ObjectId* m_id;
      te::gm::Geometry* m_geom;
      bool m_isOwner;
    };

  } // end namespace edit
}   // end namespace te

#endif  // __TERRALIB_EDIT_INTERNAL_IDGEOM_H
