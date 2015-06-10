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
  \file terralib/sa/core/BayesLocalOperation.h

  \brief This file contains a class that represents the bayes global operation.

  \reference Adapted from TerraLib4.
*/

#ifndef __TERRALIB_SA_INTERNAL_BAYESLOCALOPERATION_H
#define __TERRALIB_SA_INTERNAL_BAYESLOCALOPERATION_H

// Terralib Includes
#include "../Config.h"
#include "BayesParams.h"

// STL Includes
#include <map>
#include <memory>


namespace te
{
  //forward declarations
  namespace da  { class DataSetType; }
  namespace gm  { class Geometry; }
  namespace mem { class DataSet; }

  namespace sa
  {
    /*!
      \class BayesGlobalOperation

      \brief Class used to execute the bayes global operations

    */
    class TESAEXPORT BayesLocalOperation
    {
      public:

        /*! \brief Default constructor. */
        BayesLocalOperation();

        /*! \brief Virtual destructor. */
        ~BayesLocalOperation();

      public:

        /*! \brief Function to execute the bayes operation. */
        void execute();

        void setParameters(te::sa::BayesInputParams* inParams, te::sa::BayesOutputParams* outParams);

      protected:

        /*! Function used to save the output dataset */
        void saveDataSet(te::da::DataSet* dataSet, te::da::DataSetType* dsType);

        /*! Function used to create the output dataset type */
        std::auto_ptr<te::da::DataSetType> createDataSetType(te::da::DataSetType* dsType);

        /*! Function used to create the output dataset */
        std::auto_ptr<te::mem::DataSet> createDataSet(te::da::DataSet* inputDataSet, te::da::DataSetType* dsType);

        void runBayesLocal(te::mem::DataSet* ds, std::size_t idIdx, std::size_t eventIdx, std::size_t popIdx, 
          std::size_t neighEventIdx, std::size_t neighPopIdx, std::size_t bayesIdx, std::size_t gpmEventIdx, std::size_t gpmPopIdx);

      protected:

        std::auto_ptr<te::sa::BayesInputParams> m_inputParams;       //!< Attribute with the bayes input parameters.

        std::auto_ptr<te::sa::BayesOutputParams> m_outputParams;     //!< Attribute with the bayes output parameters.
    };
  } // end namespace sa
} // end namespace te

#endif //__TERRALIB_SA_INTERNAL_BAYESLOCALOPERATION_H