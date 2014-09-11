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
  \file terralib/sa/core/SkaterParams.h

  \brief This file contains a class that represents the skater parameters.

  \reference Adapted from TerraLib4.
*/

#ifndef __TERRALIB_SA_INTERNAL_SKATERPARAMS_H
#define __TERRALIB_SA_INTERNAL_SKATERPARAMS_H

// Terralib Includes
#include "../../dataaccess/dataset/DataSet.h"
#include "../../dataaccess/dataset/DataSetType.h"
#include "../../dataaccess/datasource/DataSource.h"
#include "../Config.h"
#include "../Enums.h"
#include "GeneralizedProximityMatrix.h"

// STL Includes
#include <memory>
#include <string>
#include <vector>

namespace te
{
  namespace sa
  {
    /*!
      \class SkaterInputParams

      \brief Class that represents the skater input parameters.

      \sa SkaterOperation
    */

    class TESAEXPORT SkaterInputParams
    {
      public:
      
        /*! \brief Default constructor. */
        SkaterInputParams()
        {
          m_gpmAttrLink = "";
          m_nClusters = 0;
          m_minPop = 0;
          m_attrPop = "";
        }

        /*! \brief Virtual destructor. */
        ~SkaterInputParams()
        {
          m_attrs.clear();
        }

      public:

        std::auto_ptr<te::da::DataSetType> m_dsType;              //!< Attribute used to access the data set metadata
        std::auto_ptr<te::da::DataSet> m_ds;                      //!< Attribute with data set
        std::auto_ptr<te::sa::GeneralizedProximityMatrix> m_gpm;  //!< Attribute with graph information
        std::string m_gpmAttrLink;                                //!< Attribute from dataset that was used to generate the gpm.

        te::sa::SkaterAggregationType m_aggregType;               //!< Aggregation type
        std::size_t m_nClusters;                                  //!< Number of clusters (Aggregation type = Clusters)
        std::size_t m_minPop;                                     //!< Number of minimum population (Aggregation type = Population)
        std::string m_attrPop;                                    //!< Attribute used to represent the population (Aggregation type = Population)

        std::vector<std::string> m_attrs;                         //!< List of attributes selected to calculate the weight of the edge
    };

    /*!
      \class SkaterOutputParams

      \brief Class that represents the skater output parameters.

      \sa SkaterOperation
    */

    class TESAEXPORT SkaterOutputParams
    {
      public:
      
        /*! \brief Default constructor. */
        SkaterOutputParams()
        {
          m_outputDataSetName = "";
        }

        /*! \brief Virtual destructor. */
        ~SkaterOutputParams()
        {
        }

      public:

        te::da::DataSourcePtr m_dataSource;               //!< Pointer to the output data source

        std::string m_outputDataSetName;                  //!< Attribute that defines the output dataset name
    };
  } // end namespace sa
} // end namespace te

#endif //__TERRALIB_SA_INTERNAL_SKATERPARAMS_H