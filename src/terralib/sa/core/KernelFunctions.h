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
  namespace sa
  {
    ///< Tree used to store the dataset geometries MBR and its id value
    typedef te::sam::rtree::Index<int> KernelTree;

    ///< Map with id, associated with geometry and its attribute (intensity value)
    typedef std::map<int, std::pair<te::gm::Geometry*, double> > KernelMap;

    TESAEXPORT void GridStatRadiusKernel(te::sa::KernelParams* params, te::sa::KernelTree& kTree, te::sa::KernelMap& kMap, te::rst::Raster* raster, double radius);

    TESAEXPORT void GridAdaptRadiusKernel(te::sa::KernelParams* params, te::sa::KernelTree& kTree, te::sa::KernelMap& kMap, te::rst::Raster* raster);

    TESAEXPORT double KernelValue(te::sa::KernelParams* params, te::sa::KernelMap& kMap, double radius, te::gm::Coord2D& coord, std::vector<int> idxVec); 

    TESAEXPORT double CalculateDistance(te::gm::Geometry* geom, te::gm::Coord2D& coord);

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

    TESAEXPORT void KernelNormalize(te::sa::KernelParams* params, te::sa::KernelMap& kMap, te::rst::Raster* raster, double totKernel);
  }
}

#endif //__TERRALIB_SA_INTERNAL_KERNELFUNCTIONS_H
