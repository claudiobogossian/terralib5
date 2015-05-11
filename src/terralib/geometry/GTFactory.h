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
  \file terralib/geometry/GTFactory.h

  \brief 2D Geometric transformation factory.
*/

#ifndef __TERRALIB_GEOMETRY_INTERNAL_GTFACTORY_H
#define __TERRALIB_GEOMETRY_INTERNAL_GTFACTORY_H

// TerraLib
#include "../common/AbstractFactory.h"
#include "Config.h"
#include "GeometricTransformation.h"

// STL
#include <string>

namespace te
{
  namespace gm
  {
    /*!
      \class GTFactory

      \brief 2D Geometric transformation factory.
    */
    class TEGEOMEXPORT GTFactory : public te::common::AbstractFactory<GeometricTransformation, std::string>
    {
      public:

        virtual ~GTFactory();

      protected:

        GTFactory( const std::string& factoryKey );
    };

  } // end namespace gm
}   // end namespace te

#endif  // __TERRALIB_GEOMETRY_INTERNAL_GTFACTORY_H

