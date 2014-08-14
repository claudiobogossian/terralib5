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
  \file terralib/sa/core/KernelFunctions.h

  \brief This file contains a functions used by the kernel operation.

  \reference Adapted from TerraLib4.
*/

#ifndef __TERRALIB_SA_INTERNAL_KERNELFUNCTIONS_H
#define __TERRALIB_SA_INTERNAL_KERNELFUNCTIONS_H

//TerraLib
#include "../../geometry/Geometry.h"
#include "../../raster/Raster.h"
#include "../../sam/rtree/Index.h"
#include "../Config.h"
#include "KernelParams.h"

//STL
#include <map>

namespace te
{
  //forward declarations
  namespace mem { class DataSet; }

  namespace sa
  {
    ///< Tree used to store the dataset geometries MBR and its id value
    typedef te::sam::rtree::Index<int> KernelTree;

    ///< Map with id, associated with geometry and its attribute (intensity value)
    typedef std::map<int, std::pair<te::gm::Geometry*, double> > KernelMap;

    /*!
      \brief Evaluates kernel value using a raster as output data and a fixed value for radius.

      \param params Pointer to a Kernel Params
      \param kTree Reference to  tree with all geometries from input data
      \param kMap Reference to the kernel Map
      \param raster Pointer to raster to set the kernel values
      \param radius Radius used to calculate the kernel

    */
    TESAEXPORT void GridStatRadiusKernel(te::sa::KernelParams* params, te::sa::KernelTree& kTree, te::sa::KernelMap& kMap, te::rst::Raster* raster, double radius);

    /*!
      \brief Evaluates kernel value using a raster as output data and a adaptative value for radius.

      \param params Pointer to a Kernel Params
      \param kTree Reference to  tree with all geometries from input data
      \param kMap Reference to the kernel Map
      \param raster Pointer to raster to set the kernel values

    */
    TESAEXPORT void GridAdaptRadiusKernel(te::sa::KernelParams* params, te::sa::KernelTree& kTree, te::sa::KernelMap& kMap, te::rst::Raster* raster);

    TESAEXPORT void DataSetStatRadiusKernel(te::sa::KernelParams* params, te::sa::KernelTree& kTree, te::sa::KernelMap& kMap, te::mem::DataSet* ds, int kernelIdx, int geomIdx, double radius);

    TESAEXPORT void DataSetAdaptRadiusKernel(te::sa::KernelParams* params, te::sa::KernelTree& kTree, te::sa::KernelMap& kMap, te::mem::DataSet* ds, int kernelIdx, int geomIdx);

    /*!
      \brief Evaluates kernel value of events with intensity

      \param params Pointer to a Kernel Params
      \param kMap Reference to the kernel Map
      \param radius Double value with radius information
      \param coord Coord of the element to calculate the kernel value
      \param idxVec Index of all elements that interects the box created by coord with radius value

      \return Kernel value for one element.
    */
    TESAEXPORT double KernelValue(te::sa::KernelParams* params, te::sa::KernelMap& kMap, double radius, te::gm::Coord2D& coord, std::vector<int> idxVec); 

    /*!
      \brief Kernel functions for Quartic type

      \param tau spatial threshold to define neighboorhood
      \param distance distance between event and region centroid
      \param intensity attribute value for event

      \return Kernel value
    */
    TESAEXPORT double KernelQuartic(double tau, double distance, double intensity);

    /*!
      \brief Kernel functions for Normal type

      \param tau spatial threshold to define neighboorhood
      \param distance distance between event and region centroid
      \param intensity attribute value for event

      \return Kernel value
    */
    TESAEXPORT double KernelNormal(double tau, double distance, double intensity);

    /*!
      \brief Kernel functions for Uniform type

      \param tau spatial threshold to define neighboorhood
      \param distance distance between event and region centroid
      \param intensity attribute value for event

      \return Kernel value
    */
    TESAEXPORT double KernelUniform(double tau, double distance, double intensity);

    /*!
      \brief Kernel functions for Triangular type

      \param tau spatial threshold to define neighboorhood
      \param distance distance between event and region centroid
      \param intensity attribute value for event

      \return Kernel value
    */
    TESAEXPORT double KernelTriangular(double tau, double distance, double intensity);

    /*!
      \brief Kernel functions for Negative Exponential type

      \param tau spatial threshold to define neighboorhood
      \param distance distance between event and region centroid
      \param intensity attribute value for event

      \return Kernel value
    */
    TESAEXPORT double KernelNegExponential(double tau, double distance, double intensity);


    /*!
      \brief Normalizes kernel values based on type of estimation. 

      \param params Pointer to a Kernel Params (used to get the estimation type)
      \param kMap Reference to the kernel Map
      \param raster Pointer to raster with the kernel values
      \param totKernel Sum of all kernel values from each raster pixel

    */
    TESAEXPORT void GridKernelNormalize(te::sa::KernelParams* params, te::sa::KernelMap& kMap, te::rst::Raster* raster, double totKernel);

    TESAEXPORT void DataSetKernelNormalize(te::sa::KernelParams* params, te::sa::KernelMap& kMap, te::mem::DataSet* ds, int kernelIdx, int geomIdx, double totKernel);

    /*!
      \brief Calculates the geometric mean from kernel map (intensity value) using log.

      \param kMap Reference to the kernel Map

      \return Double value with the geometric mean

    */
    TESAEXPORT double KernelGeometricMean(te::sa::KernelMap& kMap);
  }
}

#endif //__TERRALIB_SA_INTERNAL_KERNELFUNCTIONS_H
