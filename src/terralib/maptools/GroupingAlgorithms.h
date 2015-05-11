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
  \file GroupingAlgorithms.h

  \brief This file contains functions containing the algorithms for grouping values.
*/

#ifndef __TERRALIB_MAPTOOLS_INTERNAL_GROUPINGALGORITHMS_H
#define __TERRALIB_MAPTOOLS_INTERNAL_GROUPINGALGORITHMS_H

// TerraLib
#include "Config.h"
#include "../common/StringUtils.h"
#include "../maptools/GroupingItem.h"

// STL
#include <string>
#include <vector>

#include <math.h>
#include <stdlib.h>


#ifndef MIN
#define MIN(a,b) ( (a<b) ? a : b )  //!< Macro that returns min between two values
#endif

#ifndef MAX
#define MAX(a,b) ( (a>b) ? a : b )  //!< Macro that returns max between two values
#endif


namespace te
{
  namespace map
  {
    /*!
      \brief It groups the values using the unique value algorithm.

      \param inputValues  The data to be grouped by unique values.
      \param dataType     The data type of the values.
      \param legend       The container of legend items where the groups will be placed.
      \precision          The precision to be used in the conversion of the values.

      \output             The groups will be stored in the container of legend items.
    */
    TEMAPEXPORT void GroupingByUniqueValues(std::vector<std::string>& inputValues, int dataType,
                                            std::vector<te::map::GroupingItem*>& legend, int precision);

    /*!
      \brief It adjusts a value to the precision specified

      \param value        The value to be adjusted.
      \param precision    The precision the given value must be adjusted to.
      \param reduce       Flag indicating if the value must be added or subtracted to the value represented by the precision.

      \output             The value adjusted to the precision specified.
    */
    TEMAPEXPORT double AdjustToPrecision(double val, int precision, bool reduce = false);

    /*!
      \brief It groups the values defined by a range of iterators using the equal steps algorithm.

      \param begin          The iterator associated to the first element value.
      \param end            The iterator associated to the last element value.
      \param nSteps         The number of steps.
      \param legend         The container of legend items.
      \param precision      The precision to be used in the conversion of the values.
      \param countElements  The flag indicating if the counting of elements must be done.

      \output               The groups will be stored in the container of legend items.
    */
    template<class iterator>
    void GroupingByEqualSteps(iterator begin, iterator end, int nSteps, std::vector<te::map::GroupingItem*>& legend,
                              int precision = 0, bool countElements = true)
    {
      double min = std::numeric_limits<double>::max();
      double max = -std::numeric_limits<double>::max();

      iterator it = begin;

      while(it < end)
      {
        min = MIN(min, *it);
        max = MAX(max, *it);
        ++it;
      }

      double slice = (max - min)/double(nSteps);
    
      for(int i = 0; i < nSteps; ++i)
      {
        te::map::GroupingItem* legendItem = new te::map::GroupingItem;
        legendItem->setLowerLimit(te::common::Convert2String(min + double(i) * slice, precision));
        legendItem->setUpperLimit(te::common::Convert2String(min + double(i+1) * slice, precision));
        legend.push_back(legendItem);
      }

      min = te::map::AdjustToPrecision(min, precision, true);
      legend[0]->setLowerLimit(te::common::Convert2String(min, precision));
      max = te::map::AdjustToPrecision(max, precision, false);
      legend[nSteps-1]->setUpperLimit(te::common::Convert2String(max, precision));

      // Set the number of elements for each slice
      if (countElements == true)
        SetNumberOfElementsByLegendItem(begin, end, legend);
    }

    /*!
      \brief It groups the values defined by a range of iterators using the quantil algorithm.

      \param begin          The iterator associated to the first element value.
      \param end            The iterator associated to the last element value.
      \param nSteps         The number of steps.
      \param legend         The container of legend items.
      \param precision      The precision to be used in the conversion of the values.
      \param countElements  The flag indicating if the counting of elements must be done.

      \output               The groups will be stored in the container of legend items.
    */
    template<class iterator>
    void GroupingByQuantil(iterator begin, iterator end, int nSteps, std::vector<te::map::GroupingItem*>& legend,
                          int precision = 0, bool countElements = true)
    {
      sort(begin, end);

      int size = end - begin;
      double steps = (double)size / (double)nSteps;

      int  n = 0;
      iterator it = begin;
      while(it < end)
      {
        te::map::GroupingItem* legendItem = new te::map::GroupingItem;
        legendItem->setLowerLimit(te::common::Convert2String(*it, precision));

        int p = (int)(steps * (double)++n + .5);

        it = begin + p;
        if(it < end)
          legendItem->setUpperLimit(te::common::Convert2String(*it, precision));
        else
          legendItem->setUpperLimit(te::common::Convert2String(*(it-1), precision));

        legend.push_back(legendItem);
      }

      if(end-begin > 1)
      {
        double min = (*begin);
        double max = (*(end-1));

        min = te::map::AdjustToPrecision(min, precision, true);
        legend[0]->setLowerLimit(te::common::Convert2String(min, precision));

        max = te::map::AdjustToPrecision(max, precision, false);
        legend[legend.size()-1]->setUpperLimit(te::common::Convert2String(max, precision));
      }

      // Set the number of elements for each slice
      if (countElements == true)
        SetNumberOfElementsByLegendItem(begin, end, legend);
    }

    /*!
      \brief It groups the values defined by a range of iterators using the standard deviation algorithm.

      \param begin          The iterator associated to the first element value.
      \param end            The iterator associated to the last element value.
      \param nDevs          The number of deviations.
      \param legend         The container of legend items.
      \param rmean          The mean value.
      \param precision      The precision to be used in the conversion of the values.
      \param countElements  The flag indicating if the counting of elements must be done.

      \output               The groups will be stored in the container of legend items.
    */
    template<class iterator>
    void GroupingByStdDeviation(iterator begin, iterator end, double nDevs, std::vector<te::map::GroupingItem*>& legend,
                                std::string& meanTitle, int precision = 0, bool countElements = true)
    {
      // Compute min, max and mean
      double min = std::numeric_limits<double>::max();
      double max = -std::numeric_limits<double>::max();
      long double  sum = 0.;
      long double  sm2 = 0.;

      iterator it = begin;
      while(it < end)
      {
        min = MIN(min, *it);
        max = MAX(max, *it);
        sum += (*it);
        sm2 += ((*it) * (*it));
        ++it;
      }

      double count = (double)(end - begin);
      double mean = (double)(sum/count);
      long double var = (sm2 / count) - (mean * mean);
      double stdDev = sqrt(var);

      double slice = stdDev * nDevs;

      std::vector<te::map::GroupingItem*> aux;

      std::string strMean = te::common::Convert2String(mean, precision);

      double val = mean;
      while(val - slice > min - slice)
      {
        te::map::GroupingItem* legendItem = new te::map::GroupingItem;

        double v = val - slice;

        legendItem->setLowerLimit(te::common::Convert2String(v, precision));
        legendItem->setUpperLimit(te::common::Convert2String(val, precision));
        aux.push_back(legendItem);
        val = v;
      }

      std::vector<te::map::GroupingItem*>::reverse_iterator sit;
      for(sit = aux.rbegin(); sit != aux.rend(); ++sit)
        legend.push_back(*sit);

      meanTitle = "Mean - " + strMean;

      te::map::GroupingItem* legendItemMean = new te::map::GroupingItem;
      legendItemMean->setLowerLimit(te::common::Convert2String(mean, precision));
      legendItemMean->setUpperLimit(te::common::Convert2String(mean, precision));
      legendItemMean->setTitle(meanTitle);
      legend.push_back(legendItemMean);

      val = mean;
      while(val + slice < max + slice)
      {
        te::map::GroupingItem* legendItem = new te::map::GroupingItem;

        double v = val + slice;

        legendItem->setLowerLimit(te::common::Convert2String(val, precision));
        legendItem->setUpperLimit(te::common::Convert2String(v, precision));
        legend.push_back(legendItem);

        val = v;
      }

      //adjust first and last values
      //if(legend.size() >= 3)
      //{
      //  if(legend[0]->getTitle() != meanTitle)
      //    legend[0]->setLowerLimit(te::common::Convert2String(min, precision));
      //  
      //  if(legend[legend.size()-1]->getTitle() != meanTitle)
      //    legend[legend.size()-1]->setUpperLimit(te::common::Convert2String(max, precision));
      //}


      // Set the number of elements for each slice
      if (countElements == true)
        SetNumberOfElementsByLegendItem(begin, end, legend);
    }

    /*!
      \brief It sets the number of elements belonging to the range of values associated to each legend item.

      \param begin   The begin iterator of the container of elements.
      \param end     The end iterator of the container of elements.
      \param legend  The container of legend items.

      \return        The number of elements will be stored in each legend item.
    */
    template<class iterator>
    void SetNumberOfElementsByLegendItem(iterator begin, iterator end, std::vector<te::map::GroupingItem*>& legend)
    {
      iterator it;
      double from, to;
      int count;
    
      for (unsigned int i = 0; i < legend.size(); ++i)
      {
        te::map::GroupingItem* legendItem = legend[i];

        from = atof(legendItem->getLowerLimit().c_str());
        to =   atof(legendItem->getUpperLimit().c_str());
        count = 0;

        for (it = begin; it != end; ++it)
        {
          if(*it >= from && *it < to)
            ++count;
        }

        legendItem->setCount(count);
      }
    }
  }      // end namespace map
}        // end namespace te

#endif   //__TERRALIB_MAPTOOLS_INTERNAL_GROUPINGALGORITHMS_H
