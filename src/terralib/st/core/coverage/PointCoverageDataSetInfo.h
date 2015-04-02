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
  \file PointCoverageDataSetInfo.h

  \brief This file contains a class called PointCoverageDataSetInfo
*/

#ifndef __TERRALIB_ST_INTERNAL_POINTCOVERAGEDATASETINFO_H
#define __TERRALIB_ST_INTERNAL_POINTCOVERAGEDATASETINFO_H

//ST
#include "../../Config.h"
#include "../observation/ObservationDataSetInfo.h"
#include "CoverageDataSetInfo.h"

//Forward declarations
namespace te { namespace dt { class DateTime; } }

namespace te
{
  namespace st
  {
    
    /*!
      \class PointCoverageDataSetInfo

      \brief  A class that contains infos about a DataSource and DataSet 
              that contains observations of a point coverage.

      \sa ObservationDataSetInfo  
    */
    class TESTEXPORT PointCoverageDataSetInfo : public CoverageDataSetInfo
    {
      public:
        
        /*! 
          \brief Constructor.
          
          \param info       Infos about the DataSource and DataSet that contains the coverage observations    
          \param vlPropNames  The property names that contains the values of the coverage.  
        */
        PointCoverageDataSetInfo(const ObservationDataSetInfo& info, const std::vector<std::string>& vlPropNames);
                
         /*! \name Inhered methods*/
        //@{        
        te::st::CoverageType getCoverageType() const ;
        
        const ObservationDataSetInfo& getObservationDataSetInfo() const;
        //@}

        /*!
          \brief It returns the property names that contains the values of the coverage. 

          \return The property names that contains the values of the coverage. 
        */
        const std::vector<std::string>& getValuePropNames() const;
        
        /*! \brief Virtual destructor. */
        virtual ~PointCoverageDataSetInfo(); 

      protected:

        ObservationDataSetInfo          m_obsDsInfo;  //!< Infos about the DataSource and DataSet that contains the point coverage observations    
        std::vector<std::string>        m_vlPropNames;  //!< The names of the properties that contain the point coverage values. 
     };
  } // end namespace st
}   // end namespace te

#endif  // __TERRALIB_ST_INTERNAL_POINTCOVERAGEDATASETINFO_H

