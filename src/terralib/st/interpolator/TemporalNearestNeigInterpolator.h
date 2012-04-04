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
  \file TemporalNearestNeigInterpolator.h

  \brief This file contains a temporal nearest neighbors interpolator.
*/

#ifndef __TERRALIB_ST_INTERNAL_TEMPORALNEARESTNEIGINTERPOLATOR_H
#define __TERRALIB_ST_INTERNAL_TEMPORALNEARESTNEIGINTERPOLATOR_H

// Common
#include "../../common/Singleton.h"

// ST
#include "../Config.h"
#include "AbstractInterpolator.h"

// Forward declarations
namespace te { namespace dt { class AbstractData; } }

namespace te
{
  namespace st
  {
        
     /*!
      \class TemporalNearestNeigInterpolator

      \brief It is a temporal nearest neighbors interpolator.

      It returns the values whose associated time is the given time nearest
      neighbor. Its getValue method must receive a time position.  

      \sa AbstractInterpolatorParams
    */
    class TESTEXPORT TemporalNearestNeigInterpolator: public AbstractInterpolator, public te::common::Singleton<TemporalNearestNeigInterpolator>
    {
      friend class te::common::Singleton<TemporalNearestNeigInterpolator>;
      
      public:
        
        /*!
          \brief It gets the parameters associated to the interpolator.

          \return It returns the parameters associated to the interpolator.
        */
        AbstractInterpolatorParams* getParams()  { return 0; }

        /*!
          \brief It sets the parameters associated to the interpolator.

          \param p The parameters to be associated to the interpolator.

          \note It will take the ownership of the given pointer.
        */
        void setParams(AbstractInterpolatorParams* p) { return; } 

        /*!
          \brief It estimates the values associated to a given non-observed position, in an observation set.

          \param obSet    An observation set used in the estimation. 
          \param position A non-observed position. 
          \param values   The output estimated values. (output)  
          
           \note The caller will take the ownership of the output pointer (values).         
        */
        void getValues(ObservationSet* obSet, te::dt::AbstractData* position, std::vector<te::dt::AbstractData*>& values);
        
        /*! \brief Virtual destructor. */
        virtual ~TemporalNearestNeigInterpolator() {}; 

      protected:

        /*! \brief Constructor. */
        TemporalNearestNeigInterpolator() { };             
     };

  } // end namespace st
}   // end namespace te

#endif  // __TERRALIB_ST_INTERNAL_TEMPORALNEARESTNEIGINTERPOLATOR_H

