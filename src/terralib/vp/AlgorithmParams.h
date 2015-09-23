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
 \file AlgorithmParams.h
 
 \brief Algorithm Parameters.
 
 \ingroup vp
 */

#ifndef __TERRALIB_VP_INTERNAL_ALGORITHM_PARAMS_H
#define __TERRALIB_VP_INTERNAL_ALGORITHM_PARAMS_H

// Terralib
#include "../dataaccess/dataset/DataSet.h"
#include "../dataaccess/datasource/DataSource.h"
#include "Config.h"
#include "InputParams.h"

// STL
#include <string>
#include <vector>

namespace te
{
  namespace vp
  {
    /*!
    \class Algorithm Parameters

    \brief Define input/output parameters for an operation.
    */
    class TEVPEXPORT AlgorithmParams
    {
    public:

      AlgorithmParams();
      
      /*!
      \brief Constructor.

      \param map with a DataSetName as a key and a DataSource as a value.

      \note The AlgorithmParams will take the ownership of DataSource.
      */
      AlgorithmParams(std::vector<const te::vp::InputParams> inputParams,
                      te::da::DataSource* outputDataSource,
                      const std::string& outputDataSetName,
                      const std::map<std::string, te::dt::AbstractData*>& specificParams);
      
      /*! \brief Destructor. */
      ~AlgorithmParams();


      std::vector<const te::vp::InputParams> getInputParams();

      void setInputParams(std::vector<const te::vp::InputParams> setInputParams);


      te::da::DataSource* getOutputDataSource();

      void setOutputDataSource(te::da::DataSource* outputDataSource);


      const std::string& getOutputDataSetName();

      void setOutputDataSetName(const std::string& outputDataSetName);


      const std::map<std::string, te::dt::AbstractData*>& getSpecificParams();

      void setSpecificParams(const std::map<std::string, te::dt::AbstractData*>& specificParams);

    protected:

      std::vector<const te::vp::InputParams> m_inputParams;
      te::da::DataSource* m_outputDataSource;
      std::string m_outputDataSetName;
      std::map<std::string, te::dt::AbstractData*> m_specificParams;
    };
  }
}
#endif // __TERRALIB_VP_INTERNAL_ALGORITHM_PARAMS_H