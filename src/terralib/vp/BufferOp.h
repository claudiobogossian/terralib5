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
 \file BufferOp.h
 
 \brief Buffer operation.
 
 \ingroup vp
 */

#ifndef __TERRALIB_VP_INTERNAL_BUFFER_OP_H
#define __TERRALIB_VP_INTERNAL_BUFFER_OP_H

//Terralib

#include "../dataaccess/dataset/DataSet.h"
#include "../dataaccess/dataset/DataSetTypeConverter.h"
#include "../dataaccess/datasource/DataSource.h"

#include "../datatype/Property.h"

#include "../memory/DataSet.h"
#include "../statistics/core/Enums.h"

#include "Config.h"
#include "Enums.h"

// STL
#include <map>
#include <memory>
#include <string>

namespace te
{
  namespace vp
  {
    /*!
      \class BufferOp

      \brief An abstract class to compute the buffer memory and buffer query operation.

      \ingroup vp
    */
    class TEVPEXPORT BufferOp
    {
    public:
      
      /*! \brief Default constructor that can be called by subclasses. */
      BufferOp();
      
      /*! \brief Virtual destructor. */
      virtual ~BufferOp() {}
      
      /*!
        \brief It executes the operation.

        \return A Boolean value that means if the operation successfully completed or not.
      */
      virtual bool run() = 0;
      
      /*!
        \brief It verifies if the parameters are valid.

        \return A Boolean value that means if the operation successfully completed or not.
      */
      virtual bool paramsAreValid();
      
      /*!
        \brief It sets the input data will be calculated.

        \param inDsrc       Where data is located.
        \param inDsetName   DataSet name.
        \param converter    DataSetTypeConverterr.
        \param oidSet       ObjectIdSet - It is necessary if has need to execute with a set of specific objects
      */
      void setInput(te::da::DataSourcePtr inDsrc,
                    std::string inDsetName,
                    std::auto_ptr<te::da::DataSetTypeConverter> converter,
                    const te::da::ObjectIdSet* oidSet = 0);

      /*!
        \brief It sets the input parameters that will be consumed to execute the operation.

        \param distance               A numeric value to set a fixed distance to buffer.
        \param bufferPolygonRule      The rule used for buffer result (INSIDE_OUTSIDE = 0, ONLY_OUTSIDE = 1, ONLY_INSIDE = 2).
        \param bufferBoundariesRule   The BoundariesBetweenBuffers can be dissolved or not - (DISSOLVE = 0 and NOT_DISSOLVE = 1).
        \param copyInputColumns       Copy columns from input DataSet.
        \param levels                 The number of levels buffer will have.
        \param attributePosition      The buffer can be calculated by attribute values. The attribute must be numeric type.

        \note If uses attibutePosition parameter, the fixed distance will be ignored.
        \note The attributePosition parameter must be numeric type.
        \note If there is a null value or zero in chosen attribute, the line that has this value will be ignored.
      */
      void setParams(const double& distance,
                     const int& bufferPolygonRule,
                     const int& bufferBoundariesRule,
                     const bool& copyInputColumns,
                     const int& levels,
                     const int& attributePosition = -1);

      /*!
        \brief It sets the output parameters that will be used to persist operation result.

        \param outDsrc  Where data is will be persisted.
        \param dsname   The name os output data.

        \note The output name must be different from other data that is already in dataSource.
      */
      void setOutput(te::da::DataSourcePtr outDsrc, std::string dsname);

    protected:

      /*!
        \brief It builds the output DataSetType.
      */
      te::da::DataSetType* GetDataSetType();

      te::da::DataSourcePtr m_inDsrc;                           //!< The input datasource.
      std::string m_inDsetName;                                 //!< The input dataset name
      std::auto_ptr<te::da::DataSetTypeConverter> m_converter;  //!< The input datasettype
      const te::da::ObjectIdSet* m_oidSet;                      //!< The input objectid set
      
      double m_distance;          //!< The fixed distance
      int m_bufferPolygonRule;    //!< The buffer polygon rule (INSIDE_OUTSIDE = 0, ONLY_OUTSIDE = 1, ONLY_INSIDE = 2).
      int m_bufferBoundariesRule; //!< The buffer boundary rule (DISSOLVE = 0 and NOT_DISSOLVE = 1).
      bool m_copyInputColumns;    //!< Copy columns from input DataSet.
      int m_levels;               //!< The number of levels buffer will have.
      int m_attributePosition;    //!< The buffer can be calculated by attribute values. The attribute must be numeric type.
      
      te::da::DataSourcePtr m_outDsrc;  //!< The output datasource.
      std::string m_outDsetName;        //!< The output dataset name.
    };
  }
}
#endif // __TERRALIB_VP_INTERNAL_BUFFER_OP_H
