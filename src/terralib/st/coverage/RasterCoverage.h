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

  \brief This file contains a class to represent a raster coverage.
 */

#ifndef __TERRALIB_ST_INTERNAL_RASTERCOVERAGE_H
#define __TERRALIB_ST_INTERNAL_RASTERCOVERAGE_H

//ST
#include "../Config.h"
#include "Coverage.h"

namespace te
{
  namespace st
  {
    /*!
      \class RasterCoverage

      \brief A class to represent a raster coverage.

      A coverage represents a variation of a property in a spatiotemporal
      extent { (S,T) -> A }. A raster coverage is a coverage
      whose each observation is a raster associated to a 
      specific time.
      
      \sa Coverage Raster
    */
    class TESTEXPORT RasterCoverage : public Coverage
    {
      public:

        /*! \name Raster Coverage Constructors */
        //@{
        
        /*! \brief Constructor. */
        RasterCoverage(); 

        /*! 
          \brief Constructor. 
          
          \param obs      A pointer to coverage observations (raster measured at specific times).
          \param bound    The coverage spatial extent or boundary. 
                   
          \note It will take the ownership of the input pointers.
        */
        RasterCoverage(ObservationSet* obs, te::gm::Envelope* bound);

        /*! 
          \brief Constructor. 
          
          \param obs       A pointer to coverage observations (raster measured at specific times).
          \param bound    The coverage spatial extent or boundary. 
          \param interp    A pointer to an interpolator.         

          \note It will take the ownership of the input pointers.
        */
        RasterCoverage(ObservationSet* obs, te::gm::Envelope* bound, AbstractInterpolator* interp);               
        //@}

        /*! \name Inherited Methods */
        //@{
        virtual ObservationSet* getObservationSet() const;

        virtual AbstractInterpolator* getInterpolator() const;

        virtual te::gm::Geometry* getBoundary() const;        
        
        virtual void snapshot(te::dt::DateTime* t, te::rst::Raster* output) const;
        
        virtual void timeSeries(const te::gm::Point& p, std::vector<TimeSeries*>& result) const;
        //@}

        /*!
          \brief It adds an observation to coverage.

          \param phTime   The phenomenon time. 
          \param raster   The observed value presented by a raster type.
         
          \note It will take the ownership of the input pointer.
        */
        void addObservation(te::dt::DateTime* phTime, te::rst::Raster* raster);        

        /*!
          \brief It returns the raster associated to a given date and time.

          \param t  A given date and time.
           
          \return   A pointer to the raster associated to the given date and time.

          \note The caller will take the ownership of the returned pointer.
        */
        te::rst::Raster* getRaster(te::dt::DateTime* t);

        /*! \brief Virtual destructor. */
        virtual ~RasterCoverage(); 

      private:

        ObservationSet*                       m_observations;    //!< The coverage observations 
        AbstractInterpolator*                 m_interpolator;    //!< The interpolators used to estimate rasters in non-observed times
        te::gm::Envelope*                     m_boundary;        //!< The coverage spatial extent 
     };

  } // end namespace st
}   // end namespace te

#endif  // __TERRALIB_ST_INTERNAL_RASTERCOVERAGE_H

