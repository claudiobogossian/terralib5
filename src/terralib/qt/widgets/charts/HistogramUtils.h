/*  Copyright (C) 2010-2013 National Institute For Space Research (INPE) - Brazil.

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
  \file terralib/qt/widgets/charts/HistogramUtils.h

  \brief This file contains a set of utility histogram functions
*/

#ifndef __TERRALIB_QT_WIDGETS_INTERNAL_HISTOGRAMUTILS_H
#define __TERRALIB_QT_WIDGETS_INTERNAL_HISTOGRAMUTILS_H

//TerraLib
#include "../../datatype/SimpleData.h"
#include "../Config.h"

//STL
#include <map>
#include <set>
#include <string>
#include <vector>

// Boost
#include <boost/multi_index_container.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include <boost/multi_index/identity.hpp>
#include <boost/multi_index/member.hpp>

namespace te
{
  namespace qt
  {
    namespace widgets
    {

          struct CompareHistogramInterval
          {
              bool operator()(te::dt::AbstractData* v1, te::dt::AbstractData* v2)
              {
                  int type = v1->getTypeCode();
                  switch(type) 
                  {
                      case te::dt::DOUBLE_TYPE:
                        return (static_cast<te::dt::Double*>(v1)->getValue() < static_cast<te::dt::Double*>(v2)->getValue());  
                      case te::dt::STRING_TYPE:
                        return (static_cast<te::dt::String*>(v1)->getValue() < static_cast<te::dt::String*>(v2)->getValue());
                      default:
                        return false;
                  }
              }
          };

          typedef std::map<te::dt::AbstractData*, unsigned int, CompareHistogramInterval>   HistogramValues;  //!< Histogram's values

          // The struct used to store the interval and it'd object id
          struct IntervalToObjectId
          {
            te::dt::AbstractData*  interval;
            std::string            oid;

            IntervalToObjectId(te::dt::AbstractData* interval, std::string oid) :interval(interval),oid(oid){}

            bool operator<(const IntervalToObjectId& v)const 
            { 
              CompareHistogramInterval comp = CompareHistogramInterval();
              return comp(interval, v.interval);
            }

          };

           // define a multiply indexed set with indices by interval and it's objectId
          typedef boost::multi_index::multi_index_container<
            IntervalToObjectId,
            boost::multi_index::indexed_by<

              // sort by operator<
              boost::multi_index::ordered_non_unique<boost::multi_index::member<IntervalToObjectId, te::dt::AbstractData*, &IntervalToObjectId::interval> >,

              // sort by less<string> on objectID
              boost::multi_index::ordered_unique<boost::multi_index::member<IntervalToObjectId,std::string,&IntervalToObjectId::oid> >
            > 
          > IntervalToObjectIdSet;

    } // end namespace widgets
  }   // end namespace qt
}     // end namespace te

#endif  // __TERRALIB_QT_WIDGETS_INTERNAL_HISTOGRAMUTILS_H