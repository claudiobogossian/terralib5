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
  \file CoverageDataSetInfo.h

  \brief This file contains an abstract class called CoverageDataSetInfo
*/

#ifndef __TERRALIB_ST_INTERNAL_COVERAGEDATASETINFO_H
#define __TERRALIB_ST_INTERNAL_COVERAGEDATASETINFO_H

//ST
#include "../../Config.h"
#include "../../Enums.h"

//STL
#include <vector>

namespace te { namespace da { class DataSourceInfo; } }

namespace te
{
  namespace st
  {    
    //Forward declarations
    class ObservationDataSetInfo;
    
     /*!
      \class CoverageDataSetInfo

      \brief  An abstract class that contains infos about a DataSource and DataSet 
              that contains observations of a coverage.

      This class contains information about a DataSource and DataSet that
      contains observations of a coverage.
      This information includes: 
      (1) the DataSource which it is from
      (2) the DataSet name 
      (3) the DataSet properties that contains the phenomenon time, 
          the result time.  
      
      \sa ObservationDataSetInfo  
    */
    class TESTEXPORT CoverageDataSetInfo
    {
      public:
        
        /*!
          \brief Constructor
        */
        CoverageDataSetInfo();
        
        /*!
          \brief It returns the type of the coverage

          \return the type of the coverage.
        */
        virtual te::st::CoverageType getCoverageType() const = 0;
        
        /*!
          \brief It returns information about the DataSet that contains the coverage observations

          \return A reference to the internal ObservationDataSetInfo.
        */
        virtual const ObservationDataSetInfo& getObservationDataSetInfo() const = 0;
        
        /*! \brief Virtual destructor. */
        virtual ~CoverageDataSetInfo();        
    };
  } // end namespace st
}   // end namespace te

#endif  // __TERRALIB_ST_INTERNAL_COVERAGEDATASETINFO_H

