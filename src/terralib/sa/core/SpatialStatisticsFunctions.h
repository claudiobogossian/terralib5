/*  Copyright (C) 2008-2013 National Institute For Space Research (INPE) - Brazil.

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
  \file terralib/sa/core/SpatialStatisticsFunctions.h

  \brief Functions used to calculate spatial statistics operations.

  \reference Methods adapted from TerraLib4.
*/

#ifndef __TERRALIB_SA_INTERNAL_SPATIALSTATISTICSFUNCTIONS_H
#define __TERRALIB_SA_INTERNAL_SPATIALSTATISTICSFUNCTIONS_H

// TerraLib
#include "../Config.h"

// STL
#include <string>

namespace te
{
  namespace sa
  {
    // Forward declaration
    class GeneralizedProximityMatrix;

    /*!
      \brief The local spatial statistic G is calculated for each zone based on the spatial weights object used. 
             The value returned is a Z-value, and may be used as a diagnostic tool. High positive values indicate 
             the posibility of a local cluster of high values of the variable being analysed, very low relative 
             values a similar cluster of low values.

      \param gpm  Pointer to the gpm
      \param attrIdx Attribute index used to calculate the GStatistics.

    */
    TESAEXPORT void GStatistics(te::sa::GeneralizedProximityMatrix* gpm, int attrIdx);

    /*!
      \brief Function used to calculate the local mean of each vertex from gpm graph.

      \param gpm  Pointer to the gpm.
      \param attrIdx Attribute index used to calculate the local mean.

    */
    TESAEXPORT void LocalMean(te::sa::GeneralizedProximityMatrix* gpm, int attrIdx);

    /*!
      \brief Function used to calculate the standard deviation Z and local mean of the desviation Z (WZ).
             of each vertex from gpm graph.

      \param gpm  Pointer to the gpm.
      \param attrIdx Attribute index used to calculate the Z and WZ.

    */
    TESAEXPORT void ZAndWZ(te::sa::GeneralizedProximityMatrix* gpm, int attrIdx);

    /*!
      \brief Function used to calculate the moran index, also calculates the local moran value.

      \param gpm  Pointer to the gpm.

      \return Double value that represents the moran index.

      \note This functions only works if the gpm has the Z and WZ attributes calculated.
    */
    TESAEXPORT double MoranIndex(te::sa::GeneralizedProximityMatrix* gpm);

    /*!
      \brief Function used to calculate the moran index to calculate the significance of the global moran index

      \param gpm  Pointer to the gpm.
      \param mean The mean of the original gpm of the attrIdx.
      \param variance The variance of the original gpm of the attrIdx.
      \param attrIdx Attribute selected to calculate the moran index.

      \return Double value that represents the moran index.

      \note This is a internal function used in GlobalMoranSignificance method.
    */
    TESAEXPORT double MoranIndex(te::sa::GeneralizedProximityMatrix* gpm, double mean, double variance, int attrIdx);

    /*!
      \brief Function used to calculate the global moran significance.

      \param gpm  Pointer to the gpm.
      \param attrIdx Attribute index used to calculate the global moran significance.
      \param permutationsNumber Value of pertumations.
      \param moranIndex The global moran index value.

      \return Double value that represents the global moran significance.

    */
    TESAEXPORT double GlobalMoranSignificance(te::sa::GeneralizedProximityMatrix* gpm, int attrIdx, int permutationsNumber, double moranIndex);

    /*!
      \brief Function used to calculate LISA Statical Significance for each gpm element.

      \param gpm  Pointer to the gpm.
      \param int The number of permutations.

      \note This functions only works if the gpm has the Z, Local Moran and Number of Neighbours attributes calculated.
    */
    TESAEXPORT void LisaStatisticalSignificance(te::sa::GeneralizedProximityMatrix* gpm, int permutationsNumber);

    /*!
      \brief Function used to calculate the box map info for a gpm, classifies the objects in quadrants based in the scatterplot of moran index.

      \param gpm  Pointer to the gpm.
      \param mean Mean value

      \note This functions only works if the gpm has the Z and WZ attributes calculated.
    */
    TESAEXPORT void BoxMap(te::sa::GeneralizedProximityMatrix* gpm, double mean);

    /*!
      \brief Function used to calculate the lisa map info for a gpm, classifies the objects based in the statistical significance
             of the moran local indexes (LISA).

      \param gpm  Pointer to the gpm.
      \param int The number of permutations.

      \note This functions only works if the gpm has the LISASig (LisaStatisticalSignificance) attribute calculated.
    */
    TESAEXPORT void LISAMap(te::sa::GeneralizedProximityMatrix* gpm, int permutationsNumber);

    /*!
      \brief Function used to calculate the moran map info for a gpm, classifies the objects based in the 
             scatterplot of Moran index and its statistical significance

      \param gpm  Pointer to the gpm.

      \note This functions only works if the gpm has the LISAMap and BoxMAP attributes calculated.
    */
    TESAEXPORT void MoranMap(te::sa::GeneralizedProximityMatrix* gpm);

  } // end namespace sa
}   // end namespace te

#endif  // __TERRALIB_SA_INTERNAL_SPATIALSTATISTICSFUNCTIONS_H

