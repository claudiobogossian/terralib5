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
  \file CoordinateSystemProj4Factory.h

  \brief This is implementation of the Proj4 concrete factory of coordinate systems.
 */

#ifndef __TERRALIB_SRS_INTERNAL_COORDINATESYSTEMPROJ4FACTORY_H
#define __TERRALIB_SRS_INTERNAL_COORDINATESYSTEMPROJ4FACTORY_H

#include "Config.h"
#include "CoordinateSystemFactory.h"

#include <map>
#include <string>

namespace te
{
  namespace srs
  {
    class CoordinateSystem;
    /*!
      \class CoordinateSystemProj4Factory

      \brief This is implementation of the PROJ4 concrete factory of coordinate systems.

      This class aims at building Coordinate Systems from PROJ4 CRS descriptions.

      \sa CoordinateSystemAuthorityFactory, CoordinateSystem
     */
    class TESRSEXPORT CoordinateSystemProj4Factory : public te::srs::CoordinateSystemFactory
    {
      public:

        /*!
          \brief Destructor.

          \note It will automatically unregister the factory from the dictionary.
         */
        ~CoordinateSystemProj4Factory();

        /*! \brief It initializes the factory. */
        static void initialize();

        /*! \brief It finalizes the factory. */
        static void finalize();

      protected:

        /*!
          \brief It creates a new Coordinate System.

          \param desc The SRS textual description. In this factory a PROJ4 description.

          \return A pointer to a Coordinate System if it succeeds or a null otherwise.
        */
        te::srs::CoordinateSystem* build(const std::string& desc) const;

        //! It creates the factory.
        CoordinateSystemProj4Factory();

        void add(const std::string& description, int code);
      
        int suggestCode() const;

        std::string getDescription(int csId) const;

    private:

        te::srs::CoordinateSystem* build();

        static CoordinateSystemProj4Factory* sm_factory;        //!< A pointer to the global PROJ4 coordinate system factory.

        static std::map<int, std::string> sm_mapCode2Proj4;   //!< A dictionary from codes to PROJ4 description

    };
  } // end namespace srs
}   // end namespace te

#endif  // __TERRALIB_SRS_INTERNAL_COORDINATESYSTEMProj4FACTORY_H
