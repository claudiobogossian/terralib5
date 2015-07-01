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
  \file terralib/raster/RasterProperty.h

  \brief Raster property.
*/

#ifndef __TERRALIB_RASTER_INTERNAL_RASTERPROPERTY_H
#define __TERRALIB_RASTER_INTERNAL_RASTERPROPERTY_H

// TerraLib
#include "../datatype/SimpleProperty.h"
#include "Config.h"

// STL
#include <map>
#include <vector>

namespace te
{
  namespace rst
  {
// Forward declaration
    class BandProperty;
    class Grid;

    /*!
      \brief Raster property.

      A raster property is used to describe a raster data. It inherits
      from SimpleProperty, so it can be used through the dataaccess API.

      It contains a pointer to its grid
      support and a list of bands description.

      \ingroup rst

      \sa te::dt::SimpleProperty
    */
    class TERASTEREXPORT RasterProperty : public te::dt::SimpleProperty
    {
      public:

        /*!
          \brief It constructs a new raster property.

          \param grid        The grid definition. Do not pass a null pointer.
          \param bands       Information about raster bands (This RasterProperty instance will acquire the ownership of all band properties objects).
          \param rinfo       Detailed information about the raster data.
          \param isRequired  Tells if the the property is required (mandatory) or not.
          \param id          The property identifier.
          \param parent      A reference to the parent Property of the new object if it has one.

          \warning The identifier value (id) may be used by data source implementations.
                   So, don't rely on its value!
        */
        RasterProperty(const std::string& name,
                       bool isRequired = false,
                       unsigned int id = 0,
                       te::dt::Property* parent = 0);

        /*!
          \brief It constructs a new raster property.

          \param grid        The grid definition. Do not pass a null pointer.
          \param bands       Information about raster bands (This RasterProperty instance will acquire the ownership of all band properties objects).
          \param rinfo       Detailed information about the raster data.
          \param isRequired  Tells if the the property is required (mandatory) or not.
          \param id          The property identifier.
          \param parent      A reference to the parent Property of the new object if it has one.

          \warning The identifier value (id) may be used by data source implementations.
                   So, don't rely on its value!
        */
        RasterProperty(Grid* grid,
                       const std::vector<BandProperty*>& bands,
                       const std::map<std::string, std::string>& rinfo,
                       bool isRequired = false,
                       unsigned int id = 0,
                       te::dt::Property* parent = 0);
        /*!
          \brief Copy constructor.

          \param rhs The right-hand-side copy that would be used to copy from.
        */
        RasterProperty(const RasterProperty& rhs);

        /*! \brief Destructor. */
        virtual ~RasterProperty();

        /*!
          \brief Assignment operator.

          \param rhs The right-hand-side copy used to copy from.

          \return A reference to this object.
        */
        RasterProperty& operator=(const RasterProperty& rhs);

        /*!
          \brief Sets the definition of the raster grid support.

          \param grid The grid definition. Do not pass a null pointer.

          \note The class will take the ownership of the pointer.
        */
        void set(te::rst::Grid* grid);
        
        /*! \brief Returns the definition of the raster grid support. */
        te::rst::Grid* getGrid();

        /*! \brief Returns the definition of the raster grid support. */
        const te::rst::Grid* getGrid() const;

        /*! 
          \brief It adds a new band information to the property.

          \param b A pointer to the band definition.

          \note The class will take the ownership of the pointer.
        */
        void add(te::rst::BandProperty* b);

        /*!
          \brief It sets the i-th band to the new information provided.

          \param i The band index.
          \param b The i-th band information to be assigned to the property.

          \note The method will clear any previous band associated to the i-th position.

          \note The class will take the ownership of the pointer.

          \warning The i-th position must be a valid index.
        */
        void set(std::size_t i, te::rst::BandProperty* b);

        /*! \brief Returns a reference to the list of bands definitions. */
        std::vector<te::rst::BandProperty*>& getBandProperties();

        /*! \brief Returns a reference to the list of bands definitions. */
        const std::vector<te::rst::BandProperty*>& getBandProperties() const;

        /*!
          \brief Sets the detailed information about the raster data.

          \param rinfo Detailed information about the raster data.
        */
        void setInfo(const std::map<std::string, std::string>& rinfo);

        /*! \brief Returns the detailed information about the raster data. */
        const std::map<std::string, std::string>& getInfo() const;

        /*!
          \brief It returns a clone of the object.

          The new property will NOT have associations to other elements.

          \return A clone of the object.
        */
        te::dt::Property* clone() const;

        ///*!
        //  \brief Sets the Raster Property name

        //  \param name The name.

        //  \warning This method will update the 'm_rinfo["URI"]' property.
        //*/
        //void setName(const std::string& name);

      protected:

        Grid* m_grid;                                //!< The spatial support for raster data.
        std::vector<BandProperty*> m_bands;          //!< A vector of bands description.
        std::map<std::string, std::string> m_rinfo;  //!< Additional raster information.
    };

  } // end namespace rst
}   // end namespace te

#endif  // __TERRALIB_RASTER_INTERNAL_RASTERPROPERTY_H

