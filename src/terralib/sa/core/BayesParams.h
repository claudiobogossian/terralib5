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
  \file terralib/sa/core/BayesParams.h

  \brief This file contains a class that represents the Bayes parameters.

  \reference Adapted from TerraLib4.
*/

#ifndef __TERRALIB_SA_INTERNAL_BAYESPARAMS_H
#define __TERRALIB_SA_INTERNAL_BAYESPARAMS_H

// Terralib Includes
#include "../../dataaccess/dataset/DataSet.h"
#include "../../dataaccess/dataset/DataSetType.h"
#include "../../dataaccess/datasource/DataSource.h"
#include "../core/GeneralizedProximityMatrix.h"
#include "../Config.h"
#include "../Enums.h"

// STL Includes
#include <map>
#include <memory>
#include <string>

namespace te
{
  namespace sa
  {
    /*!
      \class BayesInputParams

      \brief Class that represents the Bayes input parameters.

      \sa BayesGlobalOperation BayesLocalOperation
    */

    class TESAEXPORT BayesInputParams
    {
      public:
      
        /*! \brief Default constructor. */
        BayesInputParams()
        {
          m_rate = 1.;
          m_eventAttrName = "";
          m_populationAttrName = "";
        }

        /*! \brief Virtual destructor. */
        ~BayesInputParams()
        {
        }

      public:

        std::auto_ptr<te::da::DataSetType> m_dsType;              //!< Attribute used to access the data set metadata
        std::auto_ptr<te::da::DataSet> m_ds;                      //!< Attribute with data set
        std::auto_ptr<te::sa::GeneralizedProximityMatrix> m_gpm;  //!< Attribute with gpm information (local bayes)

        std::string m_eventAttrName;                      //!< Attribute from dataset with event information.
        std::string m_populationAttrName;                 //!< Attribute from dataset with population information.
        std::string m_gpmAttrLink;                        //!< Attribute from dataset that was used to generate the gpm.

        double  m_rate;                                   //!< Attribute with multiplicative rate correction
    };

    /*!
      \class BayesOutputParams

      \brief Class that represents the Bayes output parameters.

      \sa BayesGlobalOperation BayesLocalOperation
    */

    class TESAEXPORT BayesOutputParams
    {
      public:
      
        /*! \brief Default constructor. */
        BayesOutputParams()
        {
          m_outputDataSetName = "";
        }

        /*! \brief Virtual destructor. */
        ~BayesOutputParams()
        {
        }

      public:

        te::da::DataSourcePtr m_ds;                       //!< Pointer to the output datasource.

        std::string m_outputDataSetName;                  //!< Attribute that defines the output dataset name
    };
  } // end namespace sa
} // end namespace te

#endif //__TERRALIB_SA_INTERNAL_BAYESPARAMS_H