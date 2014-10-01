/*  Copyright (C) 2008-2013 National Institute For Space Research (INPE) - Brazil.

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
  \file terralib/geometry/GEOSGeometryFactory.h

  \brief The global factory used by TerraLib in order to create GEOS geometries.
*/

#ifndef __TERRALIB_GEOMETRY_INTERNAL_GEOSGEOMETRYFACTORY_H
#define __TERRALIB_GEOMETRY_INTERNAL_GEOSGEOMETRYFACTORY_H

// TerraLib
#include "Config.h"

// GEOS
#ifdef TERRALIB_GEOS_ENABLED
#include <geos/geom/GeometryFactory.h>

namespace te
{
  namespace gm
  {
    /*!
      \class GEOSGeometryFactory

      \brief The global factory used by TerraLib in order to create GEOS geometries.

      \sa Geometry
     */
    class TEGEOMEXPORT GEOSGeometryFactory
    {
      public:

        /*!
          \brief It returns the global Geometry factory used in TerraLib.

          \return The global Geometry Factory used in TerraLib.
         */
        static const geos::geom::GeometryFactory* getGeomFactory();

        /** @name Factory Initialization Methods
         *  Methods to control the startup and finish of the global Geometry Factory.
         */
        //@{

        /*! \brief It starts up the global Factory. This method will be automatically called by Geometry Platform class initialization. */
        static void initialize();

        /*! \brief It terminates the global Factory. This method will be automatically called by Geometry Platform class finalization. */
        static void finalize();

        //@}

      private:

        /** @name Not Instantiable
         *  You can not create objects from this Class.
         */
        //@{

        /*! Private construtor: not instantiable. */
        GEOSGeometryFactory();

        //@}

      private:

        static const geos::geom::GeometryFactory* sm_gFactory; //!< The global Geometry Factory.
    };

    inline const geos::geom::GeometryFactory* GEOSGeometryFactory::getGeomFactory()
    {
      return sm_gFactory;
    }

  } // end namespace gm
}   // end namespace te

#endif  // TERRALIB_GEOS_ENABLED
#endif  // __TERRALIB_GEOMETRY_INTERNAL_GEOSGEOMETRYFACTORY_H

