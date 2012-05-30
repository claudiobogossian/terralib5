/*  Copyright (C) 2008-2011 National Institute For Space Research (INPE) - Brazil.

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
  \file terralib/maptools/AbstractMarkFactory.h

  \brief The abstract factory for conversion of Symbology Enconding Mark elements to an image pattern.
*/

#ifndef __TERRALIB_MAPTOOLS_INTERNAL_ABSTRACTMARKFACTORY_H
#define __TERRALIB_MAPTOOLS_INTERNAL_ABSTRACTMARKFACTORY_H

// TerraLib
#include "../color/RGBAColor.h"
#include "../common/AbstractFactory.h"
#include "Config.h"

namespace te
{
// Forward declarations
  namespace se
  {
    class Mark;
  }

  namespace map
  {
    /*!
      \class AbstractMarkFactory

      \brief This is the abstract factory for conversion of Symbology Enconding Mark elements to an image pattern.

      If you want to build a mark, use commands like:
      <code>
      te::color::RBGA** pattern = te::map::AbstractMarkFactory::make(mark);
      or
      te::color::RBGA** pattern = te::map::AbstractMarkFactory::make("MyMarkBuilderId", mark);
      </code>

      \sa AbstractFactory
    */
    class TEMAPEXPORT AbstractMarkFactory : public te::common::AbstractFactory<AbstractMarkFactory, std::string>
    {
      public:

        /** @name Public Destructor
         *  Destructor.
         */
        //@{

        /*!
          \brief Virtual destructor.

          \note It will automatically unregister the factory from the dictionary.
        */
        virtual ~AbstractMarkFactory();

        //@}

        /*!
          \brief It creates a graphical mark pattern.

          \param mark The Symbology Enconding Mark element that will be used as base to build the graphical mark pattern.
          \param size The size of graphical mark pattern.

          \return The RGBA image that represents the mark.

          \note This method will look the name of the mark and try extract the associated factoryKey.
                The pattern used is "factoryKey://markName".
                Case empty will be used "DefaultMarkFactory".
          \note The caller will take the ownership of the returned pointer.
        */
        static te::color::RGBAColor** make(const te::se::Mark* mark, std::size_t size);

        /*!
          \brief It creates a graphical mark pattern using the given mark builder.

          \param factoryKey    The key of the specific mark builder to be used to create the graphical mark pattern.
          \param mark          The Symbology Enconding Mark element that will be used as base to build the graphical mark pattern.
          \param size          The size of graphical mark pattern.

          \return The RGBA image that represents the mark.

          \note The caller will take the ownership of the returned pointer.
        */
        static te::color::RGBAColor** make(const std::string& factoryKey, const te::se::Mark* mark, std::size_t size);

        /*!
          \brief It informs the set of supported marks.

          \param marks A pre-created vector of string that will be filled with the supported marks names.

          \note All registered mark factories will be consulted.
        */
        static void SupportedMarks(std::vector<std::string>& marks);

      protected:

        /*!
          \brief Pure virtual method that creates a graphical pattern based on given Symbology Enconding Mark element.

          \param mark The Symbology Enconding Mark element that will be used as base to build the mark graphical pattern.

          \return The RGBA image that represents the mark.

          \note The caller will take the ownership of the returned pointer.
        */
        virtual te::color::RGBAColor** create(const te::se::Mark* mark, std::size_t size) = 0;

        /*!
          \brief Pure virtual method that informs the set of supported marks by a specific concrete factory.

          \param marks A pre-created vector of string that will be filled with the supported marks names.
        */
        virtual void getSupportedMarks(std::vector<std::string>& marks) const = 0;

      protected:

        /** @name Protected Initializers
         *  Initializers.
         */
        //@{

        /*!
          \brief It creates the factory.

          The key of a AbstractMarkFactory is a string.

          \param factoryKey The key that identifies the factory.
        */
        AbstractMarkFactory(const std::string& factoryKey);

        //@}
    };

  } // end namespace map
}   // end namespace te


#endif  // __TERRALIB_MAPTOOLS_INTERNAL_ABSTRACTMARKFACTORY_H
