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
  \file RasterCoverageDataSetInfo.h

  \brief This file contains a class called RasterCoverageDataSetInfo
*/

#ifndef __TERRALIB_ST_INTERNAL_RASTERCOVERAGEDATASETINFO_H
#define __TERRALIB_ST_INTERNAL_RASTERCOVERAGEDATASETINFO_H

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
      \class RasterCoverageDataSetInfo

      \brief  A class that contains infos about a DataSource and DataSet 
              that contains observations of a raster coverage.

       \sa ObservationDataSetInfo  
    */
    class TESTEXPORT RasterCoverageDataSetInfo : public CoverageDataSetInfo
    {
      public:
        
         /*! 
          \brief Constructor.
          
          \param dsinfo     Infos about the DataSource. 
          \param dsName     The DataSet name.   
          \param tPropIdx   The property index that contains the times associated to the coverage.
          \param rstPropIdx   The property index that contains the raster associated to the coverage. 
        */
        RasterCoverageDataSetInfo(const te::da::DataSourceInfo& dsinfo, const std::string& dsName, 
                              int tPropIdx, int rstPropIdx);

        /*! 
          \brief Constructor.
          
          \param dsinfo     Infos about the DataSource. 
          \param dsName     The DataSet name.   
          \param rstPropIdx   The property index that contains the raster associated to the coverage. 
          \param time       The date and time associated to this raster coverage, when this information is not in the DataSet.

          \note It will take the ownership of the given pointers.
        */
        RasterCoverageDataSetInfo(const te::da::DataSourceInfo& dsinfo, const std::string& dsName, 
                                  int rstPropIdx, te::dt::DateTime* time);
        
        /*! 
          \brief Constructor.
          
          \param info       Infos about the DataSource and DataSet that contains the coverage observations    
          \param rstPropIdx   The property index that contains the raster associated to the coverage.
        */
        RasterCoverageDataSetInfo(const ObservationDataSetInfo& info, int rstPropIdx);

        /*! 
          \brief Constructor.
          
          \param info       Infos about the DataSource and DataSet that contains the coverage observations    
          \param rstPropIdx   The property index that contains the raster associated to the coverage.
          \param time       The date and time associated to this raster coverage, when this information is not in the DataSet.

          \note It will take the ownership of the given pointers.
        */
        RasterCoverageDataSetInfo(const ObservationDataSetInfo& info, int rstPropIdx, te::dt::DateTime* time);
                
         /*! \name Inhered methods*/
        //@{        
        te::st::CoverageType getCoverageType() const ;
        
        const ObservationDataSetInfo& getObservationDataSetInfo() const;
        
        const te::da::DataSourceInfo& getDataSourceInfo() const;
        
        const std::vector<int>& getTimePropIdxs() const;
        //@}

        /*!
          \brief It returns the property index that contains the raster of the coverage. 

          \return The property index that contains the raster of the coverage. 
        */
        int getRasterPropIdx() const;

        /*!
          \brief It returns the date and time associated to the raster coverage, when this information is not in the DataSet.

          \return The date and time associated to the raster coverage. 
        */
        std::auto_ptr<te::dt::DateTime> getTime() const;
        
        /*! \brief Virtual destructor. */
        virtual ~RasterCoverageDataSetInfo(); 

      protected:

        ObservationDataSetInfo  m_obsDsInfo;  //!< Infos about the DataSource and DataSet that contains the point coverage observations    
        int                     m_rstPropIdx;  //!< The property index that contains the raster 
        std::auto_ptr<te::dt::DateTime> m_time;    //! The date and time associated to this raster coverage, when this information is not in the DataSet
     };
  } // end namespace st
}   // end namespace te

#endif  // __TERRALIB_ST_INTERNAL_RASTERCOVERAGEDATASETINFO_H

