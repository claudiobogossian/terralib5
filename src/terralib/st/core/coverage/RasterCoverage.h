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
  \file RasterCoverage.h

  \brief This file contains a concrete class to represent a raster coverage.
*/

#ifndef __TERRALIB_ST_INTERNAL_RASTERCOVERAGE_H
#define __TERRALIB_ST_INTERNAL_RASTERCOVERAGE_H

//TerraLib
#include "../../../raster/Raster.h"
#include "../../../datatype/DateTime.h"

//ST
#include "../../Config.h"
#include "Coverage.h"

//STL
#include <vector>


namespace te
{
  namespace st
  {
    /*!
      \class RasterCoverage

      \brief A concrete class to represent a raster coverage.

      A raster coverage is a coverage composed of an observation
      that is represented by a raster. 

      \ingroup st

      \sa Coverage Raster Property
    */
    class TESTEXPORT RasterCoverage : public Coverage 
    {
      public:

        /*!
          \brief A constructor.

          It constructs an empty raster coverage. 
        */
        RasterCoverage();        

        /*!
          \brief A constructor.

          It constructs a raster coverage. 

          \param r  A raster 
          \param t  The spatial extent
          
          \note It will take the ownership of the given pointers.
          \note The grid associated to the given raster must be valid (method Raster::getGrid()).
        */
        RasterCoverage(te::rst::Raster* r, te::dt::DateTime* t);        
        
        /*!
          \brief It sets the raster and its associated datetime 
          
          \param r  The given raster.
          \param t  The time associated to the given raster. 

          \note It will take the ownwership of the given pointers.
          \note The grid associated to the given raster must be valid (method Raster::getGrid()).
        */
        void set(te::rst::Raster* r, te::dt::DateTime* t);

        /*!
          \brief It returns the raster. 

          \return the raster
          
          \note The caller will NOT take the ownership of the returned raster.
        */
        te::rst::Raster* getRaster();

        /*!
          \brief It returns the time. 

          \return the time
          
          \note The caller will NOT take the ownership of the returned time.
        */
        te::dt::DateTime* getTime();

                       
        /*! \name Coverage inherited methods */
        //@{
        Coverage* clone() const; 

        CoverageType getType() const;
        
        te::gm::Geometry* getSpatialExtent() const;

        te::dt::DateTime* getTime() const;

        unsigned int getNumberOfProperties() const;

        const std::vector<int>& getPropertyTypes() const;

        const std::vector<std::string>& getPropertyNames() const;

         void getValue(const te::gm::Point& l, boost::ptr_vector<te::dt::AbstractData>& result) const;

        std::auto_ptr<te::dt::AbstractData> getValue(const te::gm::Point& l, unsigned int p=0) const;

        void getValue(const te::gm::Polygon& l, unsigned int p, boost::ptr_vector<te::dt::AbstractData>& result) const; 

        void getValue(const te::gm::Polygon& l, boost::ptr_vector<te::dt::AbstractData>& result) const; 

        void getInt(const te::gm::Point& l, std::vector<int>& result) const;

        int getInt(const te::gm::Point& l, unsigned int p=0) const;

        void getInt(const te::gm::Polygon& l, unsigned int p, std::vector<int>& result) const;

        void getInt(const te::gm::Polygon& l, std::vector<int>& result) const; 
        
        void getDouble(const te::gm::Point& l, std::vector<double>& result) const;

        double getDouble(const te::gm::Point& l, unsigned int p=0) const;

        void getDouble(const te::gm::Polygon& l, unsigned int p, std::vector<double>& result) const;

        void getDouble(const te::gm::Polygon& l, std::vector<double>& result) const; 

        std::auto_ptr<te::rst::Raster> getRaster() const;

        std::auto_ptr<te::rst::Raster> getRaster(unsigned int p) const;
        //@}

        /*! \brief Virtual destructor. */
        virtual ~RasterCoverage(); 

       protected:
          
          te::rst::RasterPtr                    m_raster;
          te::dt::DateTimeShrPtr                m_time;
          std::auto_ptr<te::gm::Geometry>       m_sextent;
          std::vector<int>                      m_ptypes; //! The types of the raster coverage properties
          std::vector<std::string>              m_pnames; //! The names of the raster coverage properties
     };
  } // end namespace st
}   // end namespace te

#endif  // __TERRALIB_ST_INTERNAL_RASTERCOVERAGE_H

