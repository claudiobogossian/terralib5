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
 \defgroup RPAlgorithms Raster processing algorithms.
 \ingroup RPModule
 */ 

/*!
  \file terralib/rp/Algorithm.h
  \brief Raster Processing algorithm base interface class.
 */

#ifndef __TERRALIB_RP_INTERNAL_ALGORITHM_H
#define __TERRALIB_RP_INTERNAL_ALGORITHM_H

#include "Config.h"
#include "Exception.h"

namespace te
{
  namespace rp
  {
    class AlgorithmParameters;
    
    /*!
      \class Algorithm
      \brief Raster Processing algorithm base interface.
      \ingroup RPAlgorithms
     */
    class TERPEXPORT Algorithm
    {
      public:
        
        virtual ~Algorithm();
        
        /*!
          \brief Initialize the algorithm instance making it ready for execution.
          \param params Contrast parameters.
          \return true if OK, false on errors.
         */
        virtual bool initialize( AlgorithmParameters& params ) 
          throw( te::rp::Exception ) = 0;
        
        /*!
          \brief Executes the algorithm using the current internal parameters.
          \return true if OK, false on errors.
         */
        virtual bool execute() throw( te::rp::Exception ) = 0;          
        
        /*!
          \brief Clear all internal allocated objects and reset the algorithm to its initial state.
         */
        virtual void reset() throw( te::rp::Exception ) = 0;        

      protected:

        Algorithm();
        
      private:
        
        Algorithm( const Algorithm& );
        
        const Algorithm& operator=( const Algorithm& );
    };

  } // end namespace rp
}   // end namespace te

#endif  // __TERRALIB_RP_INTERNAL_ALGORITHM_H

