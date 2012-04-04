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
  \file CoordinateSystemAuthority.h

  \brief An abstract factory for creating coordinate system objects.
 */

#ifndef __TERRALIB_DATAACCESS_INTERNAL_COORDINATESYSTEMFACTORY_H
#define __TERRALIB_DATAACCESS_INTERNAL_COORDINATESYSTEMFACTORY_H

#include "Config.h"

#include "../common/AbstractFactory.h"

#include <string>

namespace te
{
  namespace srs
  {
    class CoordinateSystem;
    
    /*!
      \class CoordinateSystemFactory
      
      \brief An abstract factory for creating Coordinate System (CS) objects.
     
      Coordinate systems (CS) have text decriptions that describes it following a sintax dictated by an entity such as standard (e.g."WKT")
      or a software package (e. g. "PROJ4"). 
      
      CS's can also be uniquely identified by integer codes associated to an entity. 
     
      This class manages the association of integer codes and text descriptions for different entities. That allows the building of CS objects 
      from integer codes or text descriptions.
      
      \sa AbstractFactory, CoordinateSystem
    */
    class TESRSEXPORT CoordinateSystemFactory : public te::common::AbstractFactory<CoordinateSystem, std::string>
    {
      public:

        /*!
          \brief Virtual destructor.

          \note It will automatically unregister the factory from the dictionary.
         */
        virtual ~CoordinateSystemFactory();

        /*!
          \brief It builds a new CoordinateSystem object from an integer code given by an entity.

          \param entity The entity name.
          \param code   A code to a CS as defined by an entity.

          \return A pointer to CoordinateSystem object if it succeeds or null otherwise.

          \note   The caller is responsible for deleting the returned pointer.
        */
        static CoordinateSystem* make(const std::string& entity, int code);

        /*!
          \brief It builds a new CoordinateSystem object from a text managed by an entity,

          \param entity      The entity name.
          \param description The CS description.

          \return A pointer to CoordinateSystem object if it succeeds or null otherwise.

          \note   The caller is responsible for deleting the returned pointer.
        */
        static CoordinateSystem* make(const std::string& entity, const std::string& description);

        /*!
          \brief Adds a CS description and integer code according to an entity.
         
         Adds a CS description and integer code according to an entity. If the code was already
         associted to a CS it will be replaced.

          \param entity      The entity name.
          \param description The CS description.
          \param code        The CS code.
        */
        static void add(const std::string& entity, const std::string& description, int code);
      
        /*!
         \brief Adds a CS description according to an entity and associate it to a valid, unique, entity given code.
       
         \param entity      The entity name.
         \param description The CS description.
         
         \return A valid unique code, given by the entity, to represent the CS.
       */
        static int add(const std::string& entity, const std::string& description);

        /*!
          \brief Returns CS description associated to code, according to an entity.

          \param entity The entity name.
          \param code   The CS code.

          \return  The CS description associated to code, according to an entity. It returns an empty text if the code is not recognizable.
        */
        static std::string getDescription(const std::string& entity, int code);

      protected:

        /*!
          \brief Constructor.
          It Register concrete factory in the abstract factory and associate it with a key.

          \param factoryKey The key associate to the concrete factory. In this case, it should be an entity name.
         */
        CoordinateSystemFactory(const std::string& factoryKey);

        /*!
          \brief It builds a new CoordinateSystem object from a text representation.

          \param description The description of the Coordinate System in the given representation.

          \return A pointer to CS object if it succeeds or null otherwise. Caller is reponsible for deleting it.
        */
        virtual CoordinateSystem* build(const std::string& description) const = 0;

        /*!
          \brief Associates a CS description to an integer code. Should be implemented by concrete classes associated to entities.

          \param description The CS description.
          \param code        The CS code.
        */
        virtual void add(const std::string& description, int code) = 0;
      
        /*!
          \brief Returns a suggestion for a valid unique integer CS code to be associated to a CS.
         
         This method should be implemented by concrete factories.
          
         \return A valid unique integer CS code to be associated to a CS.
        */ 
        virtual int suggestCode() const = 0;

        /*!
          \brief Returns a CS description associated to an integer code. Should be implemented by concrete classes associated to entities.

          \param code The identification of the Coordinate System.

          \return  The CS descritption associated to acode. It returns an empty text if the code is recognizable.
        */
        virtual std::string getDescription(int code) const = 0;
    };
  }  // end namespace da
}  // end namespace te

#endif  // __TERRALIB_DATAACCESS_INTERNAL_COORDINATESYSTEMFACTORY_H
