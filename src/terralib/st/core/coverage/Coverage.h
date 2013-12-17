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
  \file Coverage.h

  \brief This file contains an abstract class to represent a coverage.
*/

#ifndef __TERRALIB_ST_INTERNAL_COVERAGE_H
#define __TERRALIB_ST_INTERNAL_COVERAGE_H

//ST
#include "../../Config.h"
#include "../../Enums.h"

//STL
#include <vector>
#include <map>
#include <memory>

// Boost
#include <boost/shared_ptr.hpp>
#include <boost/ptr_container/ptr_vector.hpp>

// Forward declarations
namespace te { namespace dt { class DateTime; class AbstractData; } }

namespace te { namespace gm { class Geometry; class Point; class Polygon;} }

namespace te { namespace rst { class Raster; } }

namespace te
{
  namespace st
  {
    /*!
      \class Coverage

      \brief An abstract class to represent a coverage.

      A coverage represents the variation of the values of a property  
      within a spatial extent at a specific time. 

      \ingroup st
    */
    class TESTEXPORT Coverage 
    {
      public:

        /*!
          \brief A constructor.
        */
        Coverage();       

        /*!
          \brief It returns a clone of this coverage. 

          \return A new coverage. 

          \note The caller will take the ownership of the returned pointer.
        */
        virtual Coverage* clone() const = 0; 
        
        /*!
          \brief It returns the coverage type.
          
          For while, there are two kinds of Coverages: Point Coverage and Raster Coverage. 

          \return Returns the coverage type.
        */
        virtual CoverageType getType() const = 0;

        /*!
          \brief It returns the spatial extent of a coverage
          
          \return Returns the coverage spatial extent.

          \note The caller will NOT take the ownership of the returned geometry.
        */
        virtual te::gm::Geometry* getSpatialExtent() const = 0;

        /*!
          \brief It returns the time associated to the coverage
          
          \return Returns the time associated to the coverage.

          \note The caller will NOT take the ownership of the returned date and time.
        */
        virtual te::dt::DateTime* getTime() const = 0;

        /*!
          \brief It returns the number of properties associated to the coverage.

          For raster coverage, the number of properties is equal to the number of bands.
          For point coverage, this means the number of properties associated to each point. 
          
          \return The number of properties associated to the coverage.

          \note The caller will NOT take the ownership of the returned date and time.
        */
        virtual unsigned int getNumberOfProperties() const = 0;

        /*!
          \brief It returns the types of the coverage properties.

          \return The types of the coverage properties.

          \note The caller will take a refence to the internal vector. 
        */
        virtual const std::vector<int>& getPropertyTypes() const = 0;

        /*!
          \brief It returns the names of the coverage properties.

          \return The names of the coverage properties.

          \note The caller will take a refence to the internal vector. 
        */
        virtual const std::vector<std::string>& getPropertyNames() const = 0;

        /*!
          \brief It returns the values associated to a given location
          
          This method returns the values of all properties of the coverage. 

          \param l        The given location
          \param result   The returned values associated to a given location

          \note The caller will take the ownership of the returned value.
        */
        virtual void getValue(const te::gm::Point& l, boost::ptr_vector<te::dt::AbstractData>& result) const = 0;

        /*!
          \brief It returns the value of the p-th property associated to a given location
          
          \param l    The given location
          \param p    The index of the property that will be considered. 

          \return Returns the value associated to a given location

          \note The caller will take the ownership of the returned value.
        */
        virtual std::auto_ptr<te::dt::AbstractData> getValue(const te::gm::Point& l, unsigned int p=0) const = 0;

        /*!
          \brief It returns the values of the p-th property associated to the locations inside a given polygon
          
          \param l        The given polygon
          \param p        The desired property
          \param result   The returned values of the p-th property associated to the locations inside a given polygon

          \note The caller will take the ownership of the returned value.
        */
        virtual void getValue(const te::gm::Polygon& l, unsigned int p, boost::ptr_vector<te::dt::AbstractData>& result) const = 0;

        /*!
          \brief It returns values associated to the locations inside a given polygon
          
          This method returns the values of all properties of the coverage, ordered by locations.
          
          An example, if the coverage has two properties:
          The first position of the result vector contains the first property value of first location.
          The second position of the result vector contains the second property value of the first location.
          The third position of the result vector contains the first property value of the second location.
          And so on.

          \param l        The given polygon
          \param result   The returned values associated the locations inside a given polygon

          \note The caller will take the ownership of the returned value.
        */
        virtual void getValue(const te::gm::Polygon& l, boost::ptr_vector<te::dt::AbstractData>& result) const = 0;
        
        /*!
          \brief It returns the values as integers associated to a given location
          
          This method returns the values of all properties of the coverage. 

          \param l    The given location
          \param result   The returned values associated to a given location
        */
        virtual void getInt(const te::gm::Point& l, std::vector<int>& result) const = 0;

        /*!
          \brief It returns the value as integer of the p-th property associated to a given location
          
          \param l    The given location
          \param p    The index of the property that will be considered. 

          \return Returns the value as an integer associated to a given location
        */
        virtual int getInt(const te::gm::Point& l, unsigned int p=0) const = 0;

        /*!
          \brief It returns the values as integers of the p-th property associated to the locations inside a given polygon
          
          \param l        The given polygon
          \param p        The desired property
          \param result   The returned values of the p-th property associated to the locations inside a given polygon
        */
        virtual void getInt(const te::gm::Polygon& l, unsigned int p, std::vector<int>& result) const = 0;

        /*!
          \brief It returns values as integers associated to the locations inside a given polygon
          
          This method returns the values of all properties of the coverage, ordered by locations.
          
          An example, if the coverage has two properties:
          The first position of the result vector contains the first property value of first location.
          The second position of the result vector contains the second property value of the first location.
          The third position of the result vector contains the first property value of the second location.
          And so on.

          \param l        The given polygon
          \param result   The returned values associated the locations inside a given polygon.
        */
        virtual void getInt(const te::gm::Polygon& l, std::vector<int>& result) const = 0;

        /*!
         \brief It returns the values as doubles associated to a given location
          
          This method returns the values of all properties of the coverage. 

          \param l        The given location
          \param result   The returned values associated to a given location
        */
        virtual void getDouble(const te::gm::Point& l, std::vector<double>& result) const = 0;

        /*!
          \brief It returns the value as double of the p-th property associated to a given location
          
          \param l    The given location
          \param p    The index of the property that will be considered. 

          \return Returns the value as a double associated to a given location
        */
        virtual double getDouble(const te::gm::Point& l, unsigned int p=0) const = 0;

        /*!
          \brief It returns the values as doubles of the p-th property associated to the locations inside a given polygon
          
          \param l        The given polygon
          \param p        The desired property
          \param result   The returned values of the p-th property associated to the locations inside a given polygon
        */
        virtual void getDouble(const te::gm::Polygon& l, unsigned int p, std::vector<double>& result) const = 0;

        /*!
          \brief It returns values as doubles associated to the locations inside a given polygon
          
          This method returns the values of all properties of the coverage, ordered by locations.
          
          An example, if the coverage has two properties:
          The first position of the result vector contains the first property value of first location.
          The second position of the result vector contains the second property value of the first location.
          The third position of the result vector contains the first property value of the second location.
          And so on.

          \param l        The given polygon
          \param result   The returned values associated the locations inside a given polygon
        */
        virtual void getDouble(const te::gm::Polygon& l, std::vector<double>& result) const = 0;

        /*!
          \brief It returns a raster associated to the coverage. 

          For point coverage, it considers the interpolation function to generate
          the raster. 

          The returned raster will contain all properties, each property will be
          a band. If the caller wants to consider only one property, 
          it must use the other getRaster method when the caller pass a defined property.       

          \return Returns a raster associated to the coverage. 

          \note The caller will take the ownership of the returned raster.
        */
        virtual std::auto_ptr<te::rst::Raster> getRaster() const = 0;

        /*!
          \brief It returns a raster associated to the coverage of the i-th property. 

          For point coverage, it considers the interpolation function to generate
          a raster. 

          The returned raster will have one band associated to the given property.        

          \param p The property that will be considered to generate the raster.

          \return Returns a raster associated to the coverage. 

          \note The caller will take the ownership of the returned raster.
        */
        virtual std::auto_ptr<te::rst::Raster> getRaster(unsigned int p) const = 0;
        
        /*! \brief Virtual destructor. */
        virtual ~Coverage();        
     };

     //Typedef
     typedef boost::shared_ptr<te::st::Coverage>       CoverageShrPtr;

  } // end namespace st
}   // end namespace te

#endif  // __TERRALIB_ST_INTERNAL_COVERAGE_H

