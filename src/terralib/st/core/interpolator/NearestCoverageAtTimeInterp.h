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
  \file NearestCoverageAtTimeInterp.h

  \brief  This file contains an interpolation function that returns the
          nearest coverage at time. 
*/

#ifndef __TERRALIB_ST_INTERNAL_NEARESTCOVERAGEATTIMEINTERP_H
#define __TERRALIB_ST_INTERNAL_NEARESTCOVERAGEATTIMEINTERP_H

// Common
#include "../../../common/Singleton.h"

// ST
#include "../../Config.h"
#include "AbstractCoverageSeriesInterp.h"

namespace te
{
  namespace st
  {
     /*!
      \class NearestCoverageAtTimeInterp

      \brief  It is an interpolation function the estimates the nearest coverage 
              at a given non-observed time. 

      \sa AbstractCoverageSeriesInterp
    */
    class TESTEXPORT NearestCoverageAtTimeInterp: public AbstractCoverageSeriesInterp, public te::common::Singleton<NearestCoverageAtTimeInterp>
    {
      friend class te::common::Singleton<NearestCoverageAtTimeInterp>;
      
      public:
        
        te::common::AbstractParameters* getParams()  { return 0; }

        void setParams(te::common::AbstractParameters* p) { return; } 

        std::auto_ptr<Coverage> estimate(const CoverageSeries& cvs, te::dt::DateTime* time) const;
        
        /*! \brief Virtual destructor. */
        virtual ~NearestCoverageAtTimeInterp(); 

      protected:

        /*! \brief Constructor. */
        NearestCoverageAtTimeInterp();             
     };

  } // end namespace st
}   // end namespace te

#endif  // __TERRALIB_ST_INTERNAL_NEARESTCOVERAGEATTIMEINTERP_H

