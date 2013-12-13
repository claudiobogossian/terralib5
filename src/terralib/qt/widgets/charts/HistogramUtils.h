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
#include "../Config.h"
#include "../../datatype/SimpleData.h"
#include "../../../dataaccess/dataset/ObjectId.h"

//STL
#include <map>
#include <set>
#include <string>

// Boost
#include <boost/multi_index_container.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include <boost/multi_index/identity.hpp>
#include <boost/multi_index/member.hpp>
#include <boost/multi_index/mem_fun.hpp>

namespace te
{
  namespace qt
  {
    namespace widgets
    {

         //The struct used to sort the elements of the HistogramValues map
          struct CompareHistogramInterval
          {
              bool operator()(te::dt::AbstractData* v1, te::dt::AbstractData* v2) const
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

          // The struct used to store the interval and it's object id
          struct IntervalToObjectId
          {
            te::dt::AbstractData*  interval;
            te::da::ObjectId*      oid;

            IntervalToObjectId(te::dt::AbstractData* p_interval, te::da::ObjectId* p_oid) : interval(p_interval), oid(p_oid){}

            bool operator<(const IntervalToObjectId& v) const 
            { 
              CompareHistogramInterval comp = CompareHistogramInterval();
              return comp(interval, v.interval);
            }

            std::string getObjIdAsString() const
            {
              return oid->getValueAsString();
            }
          };

          // define a multiply indexed set with indices by
          typedef boost::multi_index::multi_index_container<
            IntervalToObjectId,
            boost::multi_index::indexed_by<

              // sort by less<string> or <  on Interval
              boost::multi_index::ordered_non_unique<
                  boost::multi_index::identity<IntervalToObjectId> >,

              // sort by less<string> on objectID
              boost::multi_index::ordered_unique<
                  boost::multi_index::const_mem_fun<IntervalToObjectId, std::string, &IntervalToObjectId::getObjIdAsString> >
            > 
          > IntervalToObjectIdSet;

    } // end namespace widgets
  }   // end namespace qt
}     // end namespace te

#endif  // __TERRALIB_QT_WIDGETS_INTERNAL_HISTOGRAMUTILS_H