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
  \file TrajectoryDataSetInfo.h

  \brief This file contains a class called TrajectoryDataSetInfo
*/

#ifndef __TERRALIB_ST_INTERNAL_TRAJECTORYDATASETINFO_H
#define __TERRALIB_ST_INTERNAL_TRAJECTORYDATASETINFO_H

//ST
#include "../../Config.h"
#include "../observation/ObservationDataSetInfo.h"

//STL
#include <vector>

namespace te { namespace da { class DataSourceInfo; } }

namespace te
{
  namespace st
  {
    
     /*!
      \class TrajectoryDataSetInfo

      \brief A class that contains infos about a DataSet that contains 
            observations of one or more trajectories.

      This class contains information about a DataSet that
      contains observations of one or more trajectories. 
      This information includes: 
      (1) the DataSource which it is from
      (2) the DataSet name 
      (3) the DataSet properties that contains the phenomenon time, 
          the result time, the observed geometries and the identity of each trajectory.  
      
      \sa ObservationDataSetInfo  
    */
    class TESTEXPORT TrajectoryDataSetInfo
    {
      public:
        
        /*! 
          \brief Constructor.
          
          \param dsinfo     Infos about the DataSource. 
          \param dsName     The DataSet name.   
          \param tPropName  The property name that contains the times associated to the trajectory.
          \param gPropName  The property name that contains the geometries associated to the trajectory.
          \param idPropName The property name that contains the identification of each trajectory. 
          \param id         The identification of the trajectory hat this DataSet contains. 
        */
        TrajectoryDataSetInfo(const te::da::DataSourceInfo& dsinfo, const std::string& dsName, 
                              const std::string& tPropName, const std::string& gPropName, 
                              const std::string& idPropName, const std::string& id); 

        /*! 
          \brief Constructor.
          
          \param info       Infos about the DataSource and DataSet that contains the trajectory observations    
        */
        TrajectoryDataSetInfo(const ObservationDataSetInfo& info); 
                
        /*!
          \brief It returns information about the DataSet that contains the trajectory observations

          \return A reference to the internal ObservationDataSetInfo.
        */
        const ObservationDataSetInfo& getObservationDataSetInfo() const;
 
        /*! \brief Virtual destructor. */
        virtual ~TrajectoryDataSetInfo(); 

      protected:

        ObservationDataSetInfo  m_obsDsInfo;    //!< Infos about the DataSource and DataSet that contains the trajectory observations    
     };
  } // end namespace st
}   // end namespace te

#endif  // __TERRALIB_ST_INTERNAL_TRAJECTORYDATASETINFO_H

