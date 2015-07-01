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
  \file terralib/raster/RasterFactory.h

  \brief This is the abstract factory for Rasters.
*/

#ifndef __TERRALIB_RASTER_INTERNAL_RASTERFACTORY_H
#define __TERRALIB_RASTER_INTERNAL_RASTERFACTORY_H

// TerraLib
#include "../common/AbstractFactory.h"
#include "Raster.h"

namespace te
{
  namespace rst
  {
// Forward declaration
    class BandProperty;
    class Grid;

    /*!
      \class RasterFactory

      \brief This is the abstract factory for Rasters.

      \ingroup rst

      \sa Raster
    */
    class TERASTEREXPORT RasterFactory : public te::common::AbstractFactory<Raster, std::string>
    {
      public:

        /*!
          \brief It creates and returns an empty raster with default raster driver.

          \return An empty raster.

          \note The caller will take the ownership of the returned pointer.
        */
        static Raster* make();

        /*!
          \brief It creates an empty raster with the proper driver.

          \param rType The name of the specific driver to be used to create the raster.

          \return An empty raster.

          \note The caller will take the ownership of the returned pointer.
        */
        static Raster* make(const std::string& rType);

        /*!
          \brief It creates a raster with the given parameters using the default raster driver.

          \param g        The raster grid. The factory will take its ownership.
          \param bands    A vector of band properties with one property for each band. The factory will take the ownership of all properties.
          \param rinfo    The necessary information to create the raster.
          \param h        It may be any specific value for a given driver.
          \param deleter  A pointer to a deleter function used to free the memory pointed by h.

          \return A new raster.

          \note The caller will take the ownership of the returned pointer.

          \note If you inform a deleter when the created raster is destroyed it will call it for the informed h pointer.
        */
        static Raster* make(Grid* g, const std::vector<BandProperty*> bands, const std::map<std::string, std::string>& rinfo, void* h = 0, void (*deleter)(void*) = 0);

        /*!
          \brief It creates a raster with the given parameters using a proper driver.

          \param rType    The name of the specific driver to create the raster.
          \param g        The raster grid. The factory will take its ownership.
          \param bands    A vector of band properties, one property for each band. The factory will take the ownership of all properties.
          \param rinfo    The necessary information to create the raster.
          \param h        It may be any specific value for a given driver.
          \param deleter  A pointer to a deleter function used to free the memory pointed by h.

          \return A new raster.

          \note The caller will take the ownership of the returned pointer.

          \note If you inform a deleter when the created raster is destroyed it will call it for the informed h pointer.
        */
        static Raster* make(const std::string& rType, Grid* g, const std::vector<BandProperty*> bands, const std::map<std::string, std::string>& rinfo, void* h = 0, void (*deleter)(void*) = 0);

        /*!
          \brief It creates a raster with the given parameters using the default driver.

          \param rinfo    The necessary information to create the raster.
          \param h        It may be any specific value for a given driver.
          \param deleter  A pointer to a deleter function used to free the memory pointed by h.

          \return A new raster.

          \note The caller will take the ownership of the returned pointer.

          \note If you inform a deleter when the created raster is destroyed it will call it for the informed h pointer.
        */
        static Raster* make(const std::map<std::string, std::string>& rinfo, void* h = 0, void (*deleter)(void*) = 0);

        /*!
          \brief It creates a raster with the given parameters using a proper driver.

          \param rType    The name of the specific driver to create the raster.
          \param rinfo    The necessary information to create the raster.
          \param h        It may be any specific value for a given driver.
          \param deleter  A pointer to a deleter function used to free the memory pointed by h.

          \return A new raster.

          \note The caller will take the ownership of the returned pointer.

          \note If you inform a deleter when the created raster is destroyed it will call it for the informed h pointer.
        */
        static Raster* make(const std::string& rType, const std::map<std::string, std::string>& rinfo, void* h, void (*deleter)(void*) = 0);

        /*!
          \brief It opens a raster with the given parameters and default raster driver.

          \param rinfo  The necessary information to open the raster.
          \param p      The access policy.

          \return The opened raster.

          \note The caller will take the ownership of the returned pointer.
        */
        static Raster* open(const std::map<std::string, std::string>& rinfo, te::common::AccessPolicy p = te::common::RAccess);

        /*!
          \brief It creates a raster with the given parameters.

          \param rType  The name of the specific driver to create the raster.
          \param rinfo  The necessary information to open the raster.
          \param p      The access policy.

          \return The opened raster.

          \note The caller will take the ownership of the returned pointer.
        */
        static Raster* open(const std::string& rType, const std::map<std::string, std::string>& rinfo, te::common::AccessPolicy p = te::common::RAccess);

        /*!
          \brief It creates a raster with the given parameters and default raster driver.

          \param key    The name of the key used to define raster location, ex.: "URI".
          \param value  The value of the key to define raster location, ex.: "/path/to/raster.tif".
          \param p      The access policy.

          \return The opened raster.

          \note The caller will take the ownership of the returned pointer.
        */
        static Raster* open(const std::string& key, const std::string& value, te::common::AccessPolicy p = te::common::RAccess);

        /*!
          \brief It creates a raster with the given parameters.

          \param rType  The name of the specific driver to create the raster.
          \param key    The name of the key used to define raster location, ex.: "URI".
          \param value  The value of the key to define raster location, ex.: "/path/to/raster.tif".
          \param p      The access policy.

          \return The opened raster.

          \note The caller will take the ownership of the returned pointer.
        */
        static Raster* open(const std::string& rType, const std::string& key, const std::string& value, te::common::AccessPolicy p = te::common::RAccess);

        /*! \brief Destructor. */
        virtual ~RasterFactory() {}

        /*! \brief Returns the type (name) of this factory. */
        virtual const std::string& getType() const = 0;

        /*! \brief It returns the list of parameters accepted as raster info. */
        virtual void getCreationalParameters(std::vector< std::pair<std::string, std::string> >& params) const = 0;

        /*! \brief It returns a map<string, string> containing all supported formats. */
        virtual std::map<std::string, std::string> getCapabilities() const = 0;

      protected:

        /*!
          \brief Constructor.

          \param factorKey The key that identifies the factory.
        */
        RasterFactory(const std::string& factoryKey);

        /*!
          \brief This method may be re-implemented by subclasses in order to have a finner control for the raster object instantiation.

          \param rinfo  The necessary information to open the raster.
          \param p      The access policy.

          \return A raster.

          \note The caller will take the ownership of the returned pointer.
        */
        virtual Raster* iOpen(const std::map<std::string, std::string>& rinfo, te::common::AccessPolicy p = te::common::RAccess);

        /*!
          \brief This method must be implemented by subclasses (raster drivers).

          \param g        The raster grid. May be a NULL parameter. Implementations must take its ownership.
          \param bands    A vector of band properties, one for each band. Implementations must take ownership of the pointers in this vector.
          \param rinfo    The necessary information to create the raster.
          \param h        It may be any specific value for a given driver. May be a NULL parameter.
          \param deleter  A pointer to a deleter function used to free the memory pointed by h. May be a NULL parameter. Implementations must use this method when it doesn't use 'h' anymore.

          \return The new raster.

          \note The caller will take the ownership of the returned pointer.
        */
        virtual Raster* create(Grid* g, const std::vector<BandProperty*> bands, const std::map<std::string, std::string>& rinfo, void* h = 0, void (*deleter)(void*) = 0) = 0;
    };

  } // end namespace rst
}   // end namespace te

#endif  // __TERRALIB_RASTER_INTERNAL_RASTERFACTORY_H
