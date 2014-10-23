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
  \file terralib/rp/TiePointsLocatorInputParameters.h
  \brief TiePointsLocator input parameters.
 */

#ifndef __TERRALIB_RP_INTERNAL_TIEPOINTSLOCATORINPUTPARAMETERS_H
#define __TERRALIB_RP_INTERNAL_TIEPOINTSLOCATORINPUTPARAMETERS_H

#include "TiePointsLocatorStrategyParameters.h"

namespace te
{
  namespace rp
  {
    /*!
      \class TiePointsLocatorInputParameters
      \brief TiePointsLocator input parameters
     */        
    class TERPEXPORT TiePointsLocatorInputParameters : public TiePointsLocatorStrategyParameters
    {
      public:
        
        /**
        * \name Global parameters
        */
        /**@{*/            
        
        //@}
        
        TiePointsLocatorInputParameters();
        
        TiePointsLocatorInputParameters( const TiePointsLocatorInputParameters& );
        
        ~TiePointsLocatorInputParameters();
        
        //overload
        void reset() throw( te::rp::Exception );
        
        //overload
        const  TiePointsLocatorInputParameters& operator=( const TiePointsLocatorInputParameters& params );
        
        //overload
        AbstractParameters* clone() const;
    };

  } // end namespace rp
}   // end namespace te

#endif

