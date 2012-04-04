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
  \file CoordinateSystemEPSGFactory.h

  \brief This is implementation of the EPSG concrete factory of coordinate systems.
 */

#ifndef __TERRALIB_SRS_INTERNAL_COORDINATESYSTEMEPSGFACTORY_H
#define __TERRALIB_SRS_INTERNAL_COORDINATESYSTEMEPSGFACTORY_H

#include "Config.h"
#include "CoordinateSystemFactory.h"
#include "Ellipsoid.h"
#include "Datum.h"
#include "ProjectedCoordinateSystem.h"
#include "GeographicCoordinateSystem.h"

#include <map>

namespace te
{
  namespace srs
  {
    /*!
      \class CoordinateSystemEPSGFactory

      \brief A concrete implementation for a CS Factory according to the EPSG entity.
      
     In this factory, the name of the entitiy is "EPSG". CS descriptions should follow 
     OGC's WKT for SRS sintax (www.opengeospatial.org). CS integer codes should follow 
     the EPSG database (www.epsg.org). 

      \sa CoordinateSystemFactory, CoordinateSystem
     */
    class TESRSEXPORT CoordinateSystemEPSGFactory : public te::srs::CoordinateSystemFactory
    {
      public:

        /*! \brief Destructor. */
        ~CoordinateSystemEPSGFactory();

        /*! \brief Initalizes this factory */
        static void initialize();

        /*! \brief Finalizes this factory */
        static void finalize();

        /*!
          \brief Associates a CS description to an integer code.
           
           For this entity, CS descriptions should follow OGC's WKT for SRS sintax. 
           And integer codes should follow the EPSG database.
       
          \param description The CS description.
          \param code        The CS code.
        */      
        void add(const std::string& description, int code);

        std::string getDescription(int csId) const;
      
        int suggestCode() const;

      protected:

        te::srs::CoordinateSystem* build(const std::string& desc) const;

        CoordinateSystemEPSGFactory();   

      private:

        te::srs::CoordinateSystem* build();

        static CoordinateSystemEPSGFactory* sm_factory;    

        static std::map<int, std::string> sm_mapEPSG2WKT;
    };

  } // end namespace srs
}   // end namespace te

#endif  // __TERRALIB_SRS_INTERNAL_COORDINATESYSTEMEPSGFACTORY_H
