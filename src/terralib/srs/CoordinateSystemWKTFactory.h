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
  \file CoordinateSystemWKTFactory.h

  \brief This is implementation of the WKT concrete factory of coordinate systems.
 */

#ifndef __TERRALIB_SRS_INTERNAL_COORDINATESYSTEMWKTFACTORY_H
#define __TERRALIB_SRS_INTERNAL_COORDINATESYSTEMWKTFACTORY_H

#include "Config.h"
#include "CoordinateSystemFactory.h"

namespace te
{
  namespace srs
  {
    /*!
      \class CoordinateSystemWKTFactory

      \brief This is implementation of the WKT concrete factory of coordinate systems.

      \sa CoordinateSystemAuthorityFactory, CoordinateSystem
     */
    class TESRSEXPORT CoordinateSystemWKTFactory : public te::srs::CoordinateSystemFactory
    {
      public:

        /*!
          \brief Destructor.

          \note It will automatically unregister the factory from the dictionary.
         */
        ~CoordinateSystemWKTFactory();

        /*! \brief It initializes the factory. */
        static void initialize();

        /*! \brief It finalizes the factory. */
        static void finalize();

      protected:        

        /*!
          \brief It creates a new Coordinate System.

          \param desc The SRS textual description. In this factory a SRS WKT description.

          \return A pointer to a Coordinate System if it succeeds or a null otherwise.
          
          \note Caller of this methos is responsible for deleting the returned pointer.
        */
        te::srs::CoordinateSystem* build(const std::string& desc) const;

        //! It creates the factory.
        CoordinateSystemWKTFactory();

      private:

        te::srs::CoordinateSystem* build();

        void add(const std::string& description, int csId);

        std::string getDescription(int csId) const;
      
        int suggestCode() const;

        static CoordinateSystemWKTFactory* sm_factory; 

    };
  } // end namespace srs
}   // end namespace te

#endif  // __TERRALIB_SRS_INTERNAL_COORDINATESYSTEMWKTFACTORY_H
