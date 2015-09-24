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
  \file terralib/vp/InputParams.h
 
  \brief A structure to hold the input parameters of vector processing.
*/

#ifndef __TERRALIB_VP_CORE_INTERNAL_INPUTPARAMS_H
#define __TERRALIB_VP_CORE_INTERNAL_INPUTPARAMS_H

// TerraLib
#include "../dataaccess/dataset/DataSet.h"
#include "../dataaccess/dataset/DataSetType.h"
#include "../dataaccess/datasource/DataSource.h"
#include "../dataaccess/query/Select.h"
#include "Config.h"

// STL
#include <complex>
#include <map>
#include <vector>

namespace te
{
  namespace vp
  {
    /*!
      \struct InputParams

      \brief A structure to hold the input parameters of vector processing.
    */
    class TEVPEXPORT InputParams
    {
      public:

        /*! \brief Constructor. */
        InputParams();

        /*!
          \brief Copy constructor.

          \param rhs The right-hand-side copy used to copy from.
        */
        InputParams(const InputParams& rhs);

        /*! \brief Destructor. */
        ~InputParams();

        /*!
          \brief Assignment operator.

          \param rhs The right-hand-side copy used to copy from.

          \return A reference to this object.
        */
        InputParams& operator=(const InputParams& rhs);

        /*! \brief Clear the structure. */
        void clear();

      public:

        te::da::DataSourcePtr m_inputDataSource;
        std::string m_inputDataSetName;
        te::da::DataSet* m_inputDataSet;
        te::da::DataSetType* m_inputDataSetType;
        te::da::Select* m_inputRestriction;
    };

  } // end namespace vp
} // end namespace te

#endif  // __TERRALIB_VP_CORE_INTERNAL_INPUTPARAMS_H
