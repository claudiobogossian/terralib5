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
  \file terralib/sa/core/KernelParams.h

  \brief This file contains a class that represents the kernel parameters.

  \reference Adapted from TerraLib4.
*/

#ifndef __TERRALIB_SA_INTERNAL_KERNELPARAMS_H
#define __TERRALIB_SA_INTERNAL_KERNELPARAMS_H

// Terralib Includes
#include "../../dataaccess/dataset/DataSet.h"
#include "../../dataaccess/dataset/DataSetType.h"
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
      \class KernelInputParams

      \brief Class that represents the kernel input parameters.

      \sa KernelOperation
    */

    class TESAEXPORT KernelInputParams
    {
      public:
      
        /*! \brief Default constructor. */
        KernelInputParams()
        {
          m_intensityAttrName = "";
          m_functionType = te::sa::Normal;
          m_estimationType = te::sa::Density;
          m_useAdaptativeRadius = true;
          m_radiusPercentValue = 10;
        }

        /*! \brief Virtual destructor. */
        ~KernelInputParams()
        {
        }

      public:

        std::auto_ptr<te::da::DataSetType> m_dsType;      //!< Attribute used to access the data set metadata
        std::auto_ptr<te::da::DataSet> m_ds;              //!< Attribute with data set

        std::string m_intensityAttrName;                  //!< Attribute from dataset used as intensity attr (not obrigatory)

        te::sa::KernelFunctionType m_functionType;        //!< Kernel function type
        te::sa::KernelEstimationType m_estimationType;    //!< Kernel estimation type

        bool m_useAdaptativeRadius;                       //!< Attribute to indicate if a an adaptative radius has to be used.
        int  m_radiusPercentValue;                        //!< Attribute with radius percent value (m_useAdaptativeRadius must be false)
    };

    /*!
      \class KernelOutputParams

      \brief Class that represents the kernel output parameters.

      \sa KernelOperation
    */

    class TESAEXPORT KernelOutputParams
    {
      public:
      
        /*! \brief Default constructor. */
        KernelOutputParams()
        {
          m_storageType = te::sa::Grid;                   //!< Kernel storage type
          m_combinationType = te::sa::Ratio;
          m_nCols = 50;
          m_outputAttrName = "";
          m_outputPath = "";
          m_outputDataSetName = "";
        }

        /*! \brief Virtual destructor. */
        ~KernelOutputParams()
        {
        }

      public:

        te::sa::KernelOutputType m_storageType;           //!< Kernel storage type
        te::sa::KernelCombinationType m_combinationType;  //!< Kernel combination type (used by ratio kernel)

        int m_nCols;                                      //!< Attribute with number of columns to create the grid (if KernelOutputType is Grid)
        std::string m_outputAttrName;                     //!< Attribute with the new attr name (if KernelOutputType is Attribute)
        std::string m_outputPath;                         //!< Attribute with URI of the output file
        std::string m_outputDataSetName;                  //!< Attribute that defines the output dataset name
    };
  } // end namespace sa
} // end namespace te

#endif //__TERRALIB_SA_INTERNAL_KERNELPARAMS_H