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
  \file terralib/rp/Algorithm.h
  \brief Raster Processing algorithm base interface class.
 */

#ifndef __TERRALIB_RP_INTERNAL_ALGORITHM_H
#define __TERRALIB_RP_INTERNAL_ALGORITHM_H

#include "AlgorithmInputParameters.h"
#include "AlgorithmOutputParameters.h"
#include "Config.h"
#include "Exception.h"

namespace te
{
  namespace rp
  {
    /*!
      \class Algorithm
      \brief Raster Processing algorithm base interface.
     */
    class TERPEXPORT Algorithm
    {
      public:
        
        virtual ~Algorithm();
        
        /*!
          \brief Initialize the algorithm instance making it ready for execution.
          
          \param inputParams Input parameters.
          
          \return true if OK, false on errors.
          
          \note A return error string can be obtained via te::rp::Module::getLastLogStr()
         */
        virtual bool initialize( const AlgorithmInputParameters& inputParams ) throw( te::rp::Exception ) = 0;
        
        /*!
          \brief Returns true if the algorithm instance is initialized and ready for execution.
          
          \return true if the algorithm instance is initialized and ready for execution.
         */        
        virtual bool isInitialized() const = 0;
        
        /*!
          \brief Executes the algorithm using the supplied parameters.
          
          \param outputParams Output parameters.
          
          \return true if OK, false on errors.
         */
        virtual bool execute( AlgorithmOutputParameters& outputParams ) throw( te::rp::Exception ) = 0;          
        
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

