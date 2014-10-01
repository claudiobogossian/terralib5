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
  \file PointCoverage.h

  \brief This file contains a concrete class to represent a point coverage.
*/

#ifndef __TERRALIB_ST_INTERNAL_POINTCOVERAGE_H
#define __TERRALIB_ST_INTERNAL_POINTCOVERAGE_H

//TerraLib
#include "../../../datatype/DateTime.h"
#include "../../../sam/rtree/Index.h"

//ST
#include "../../Config.h"
#include "Coverage.h"
#include "PointCoverageObservation.h"
#include "PointCoverageIterator.h"

//STL
#include <vector>
#include <map>

// Boost
#include <boost/shared_ptr.hpp>
#include <boost/ptr_container/ptr_vector.hpp>

namespace te
{
  namespace st
  {
    // Forward declarations
    class AbstractPointCoverageInterp;
    
    //Typedef
    typedef boost::shared_ptr<te::sam::rtree::Index<std::size_t> >   PointCvRTreeShrPtr; 

    /*!
      \class PointCoverage

      \brief A concrete class to represent a point coverage. 
      
      A point coverage is a coverage composed of a set of observations where
      each one contains a location (point) and a set properties
      associated to it.

      \ingroup st

      \sa Coverage Point Property
    */
    class TESTEXPORT PointCoverage : public Coverage 
    {
      public:

        /*!
          \brief A constructor.

          It constructs an empty point coverage. The user does not indicate the interpolator
          associated to it. Internally, it uses a default interpolator
          that is IDWInterpolator.
        */
        PointCoverage();        

        /*!
          \brief A constructor.

          It constructs an empty point coverage. 

          \param interp   The interpolator associated to the PointCoverage 
          \param sextent  The spatial extent
          \param textent  The time associated to the coverage.
          \param np       The number of properties of the point coverage.
          \param ptypes   The types of the properties
          \param pnames   The names of the properties

          \note It will take the ownership of the given pointers.
        */
        PointCoverage(AbstractPointCoverageInterp* interp, te::gm::Geometry* sextent, 
                      te::dt::DateTime*  textent, int np, const std::vector<int>& ptypes, 
                      const std::vector<std::string>& pnames);  

        /*!
          \brief A constructor.

          It constructs an empty point coverage. 

          \param interp   The interpolator associated to the PointCoverage 
          \param sextent  The spatial extent
          \param textent  The time associated to the coverage.
          \param np       The number of properties of the point coverage.
          \param ptypes   The types of the properties
          \param pnames   The names of the properties
          \param tp       The index of the property that contains time associated to each observation

          \note It will take the ownership of the given pointers.
        */
        PointCoverage(AbstractPointCoverageInterp* interp, te::gm::Geometry* sextent, 
                      te::dt::DateTime*  textent, int np, const std::vector<int>& ptypes, 
                      const std::vector<std::string>& pnames, int tp);  
                
        /*! \name PointCoverage Iterator 
                         
            An example of use:

            PointCoverageIterator it = cv.begin();
            while(it!=cv.end())
            {
              Point& l = it.getLocation();
              double v = it.getDouble(i); //or getValue() or getInt()...                    
              ++it;
            } 
        */
        //@{
        /*!
          \brief It returns an iterator that points to the first observation of the point coverage
          
          \return The point coverage iterator.
        */
        PointCoverageIterator begin() const;

        /*!
          \brief It returns an iterator that points to the end of the time series.
          
          \return The point coverage iterator.
        */
        PointCoverageIterator end() const;
        //@}

        /*!
          \brief It adds an observtion to the PointCoverage. 
          
          \param l      The given location.
          \param value  The value associated to the given location.

          \note It will take the ownwership of the given pointers.
          \note The caller can use this methos when the number of properties is equal to 1. Otherwise, it must use the other add method.
        */
        void add(const te::gm::Point& l, te::dt::AbstractData* value);

        /*!
          \brief It adds an observtion to the PointCoverage. 
          
          \param l      The given location.
          \param value  A vector of values associated to the given location.

          \note It will take the ownwership of the given pointers.
        */
        void add(const te::gm::Point& l, boost::ptr_vector<te::dt::AbstractData>& value);

        /*!
          \brief It adds an observtion to the PointCoverage. 
          
          \param obs    The observation.

          \note It will share the same observtion. 
        */
        void add(PointCoverageObservation& obs);
        //@}
                
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
        virtual ~PointCoverage();

       protected:
          
          std::auto_ptr<te::gm::Geometry>       m_sextent;
          std::auto_ptr<te::dt::DateTime>       m_textent;
          PointCoverageObservationSet           m_observations;
          AbstractPointCoverageInterp*          m_interpolator;
          PointCvRTreeShrPtr                    m_rTree;
          unsigned int                          m_np;     //! The number of properties of the point coverage
          std::vector<int>                      m_ptypes; //! The types of the point coverage properties
          std::vector<std::string>              m_pnames; //! The names of the point coverage properties
          int                                   m_tp;     //! The index of the observed property that has the time of each observation.  
     };
  } // end namespace st
}   // end namespace te

#endif  // __TERRALIB_ST_INTERNAL_POINTCOVERAGE_H

