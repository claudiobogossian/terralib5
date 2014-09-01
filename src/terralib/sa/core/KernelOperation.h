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
  \file terralib/sa/core/KernelOperation.h

  \brief This file contains a class that represents the kernel operation.

  \reference Adapted from TerraLib4.
*/

#ifndef __TERRALIB_SA_INTERNAL_KERNELOPERATION_H
#define __TERRALIB_SA_INTERNAL_KERNELOPERATION_H

// Terralib Includes
#include "../Config.h"
#include "KernelFunctions.h"
#include "KernelParams.h"

// STL Includes
#include <map>
#include <memory>


namespace te
{
  //forward declarations
  namespace da  { class DataSetType; }
  namespace gm  { class Geometry; }
  namespace mem { class DataSet; }
  namespace rst { class Raster; }

  namespace sa
  {
    /*!
      \class KernelOperation

      \brief Virtual class used to execute the kernel operations

      \sa KernelFunctions
    */
    class TESAEXPORT KernelOperation
    {
      public:

        /*! \brief Default constructor. */
        KernelOperation();

        /*! \brief Virtual destructor. */
        virtual ~KernelOperation();

      public:

        /*! \brief Function to execute the kernel operation. */
        virtual void execute() = 0;

        void setOutputParameters(te::sa::KernelOutputParams* outParams);

      protected:

        /*! Function used to build the tree with data set information */
        virtual void buildTree() = 0;

        /*! Function used to run kernel when output data is a raster */
        void runRasterKernel(te::sa::KernelInputParams* inputParams, te::sa::KernelTree& kTree, te::sa::KernelMap& kMap, te::rst::Raster* raster);

        /*! Function used to run kernel when output data is dataset  */
        std::auto_ptr<te::mem::DataSet> runDataSetKernel(te::sa::KernelInputParams* inputParams, te::sa::KernelTree& kTree, te::sa::KernelMap& kMap, te::da::DataSetType* dsType);

        /*! Function used to create the output raster */
        std::auto_ptr<te::rst::Raster> buildRaster(te::sa::KernelInputParams* inputParams, te::sa::KernelTree& kTree, std::string driver);

        /*! Function used to save the output dataset */
        void saveDataSet(te::da::DataSet* dataSet, te::da::DataSetType* dsType);

        /*! Function used to create the output dataset type */
        std::auto_ptr<te::da::DataSetType> createDataSetType(te::da::DataSetType* dsType);

        /*! Function used to create the output dataset */
        std::auto_ptr<te::mem::DataSet> createDataSet(te::da::DataSet* inputDataSet, te::da::DataSetType* dsType);

      protected:

        std::auto_ptr<te::sa::KernelOutputParams> m_outputParams;     //!< Attribute with the kernel output parameters.

        te::sa::KernelTree m_kTree;                                   //!< Attribute used to locate near geometries.
    };
  } // end namespace sa
} // end namespace te

#endif //__TERRALIB_SA_INTERNAL_KERNELOPERATION_H