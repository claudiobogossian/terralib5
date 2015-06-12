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
  \file terralib/rp/AlgorithmInputParameters.h
  \brief Raster Processing algorithm input parameters base interface.
 */

#ifndef __TERRALIB_RP_INTERNAL_ALGORITHM_INPUT_PARAMETERS_H
#define __TERRALIB_RP_INTERNAL_ALGORITHM_INPUT_PARAMETERS_H

#include "../common/AbstractParameters.h"
#include "Config.h"

namespace te
{
  namespace rp
  {
    /*!
      \class AlgorithmInputParameters
      \brief Raster Processing algorithm input parameters base interface.
    */
    class TERPEXPORT AlgorithmInputParameters : public te::common::AbstractParameters
    {
      public:
        
        virtual ~AlgorithmInputParameters();
        
      protected:

        AlgorithmInputParameters();
        
        AlgorithmInputParameters( const AlgorithmInputParameters& );
    };
  } // end namespace rp
}   // end namespace te

#endif  // __TERRALIB_RP_INTERNAL_ALGORITHM_INPUT_PARAMETERS_H

