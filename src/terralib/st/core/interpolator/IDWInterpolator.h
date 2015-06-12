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
  \file IDWInterpolator.h

  \brief  This file contains an inverse distance weighted (IDW) 
          interpolationan function for PointCoverage. 
*/

#ifndef __TERRALIB_ST_INTERNAL_IDWINTERPOLATOR_H
#define __TERRALIB_ST_INTERNAL_IDWINTERPOLATOR_H

// Common
#include "../../../common/Singleton.h"

// ST
#include "../../Config.h"
#include "AbstractPointCoverageInterp.h"

namespace te
{
  namespace st
  {
    /*!
      \class IDWInterpolator

      \brief A class that implements an inverse distance weighted (IDW) 
          interpolationan function for PointCoverage. 

      \sa AbstractPointCoverageInterp PointCoverage 
    */
    class TESTEXPORT IDWInterpolator : public AbstractPointCoverageInterp, public te::common::Singleton<IDWInterpolator>
    {
      friend class te::common::Singleton<IDWInterpolator>;

      public:
        
        te::common::AbstractParameters* getParams() { return 0; }

        void setParams(te::common::AbstractParameters* p) { return; }
                                    
        std::auto_ptr<te::dt::AbstractData> estimate(const PointCoverage& cv, unsigned int p, const te::gm::Point& l);
        
        /*! \brief Virtual destructor. */
        virtual ~IDWInterpolator();   
      
      protected:

        /*! \brief Constructor. */
        IDWInterpolator();     
     };

  } // end namespace st
}   // end namespace te

#endif  // __TERRALIB_ST_INTERNAL_IDWINTERPOLATOR_H

