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
  \file terralib/qt/widgets/charts/ScatterUtils.h

  \brief This file contains a set of utility scatter functions
*/

#ifndef __TERRALIB_QT_WIDGETS_INTERNAL_SCATTERUTILS_H
#define __TERRALIB_QT_WIDGETS_INTERNAL_SCATTERUTILS_H

//TerraLib
#include "../Config.h"
#include "../../../dataaccess/dataset/ObjectId.h"

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
#include <boost/multi_index/mem_fun.hpp>

namespace te
{
  namespace qt
  {
    namespace widgets
    {
          // The struct used to store the scatter's point  and it's objectId
          struct PointToObjectId
          {
            double  x;    //!< Double values for axis X.
            double  y;    //!< Double values for axis Y.
            te::da::ObjectId*    oid;  //!< This point's objectId.

            PointToObjectId(double p_x, double p_y, te::da::ObjectId* p_oid) : x(p_x), y(p_y), oid(p_oid){}

            bool operator<(const PointToObjectId& v) const 
            {
                return (v.x < x);
            }

            std::string getObjIdAsString() const
            {
              return oid->getValueAsString();
            }
          };

          // define a multiply indexed set with indices by
          typedef boost::multi_index::multi_index_container<
            PointToObjectId,
            boost::multi_index::indexed_by<

              // sort by less<string> on Interval
              boost::multi_index::ordered_non_unique<
                  boost::multi_index::identity<PointToObjectId> >,

              // sort by less<string> on objectID
              boost::multi_index::ordered_unique<
                  boost::multi_index::const_mem_fun<PointToObjectId, std::string, &PointToObjectId::getObjIdAsString> >
            > 
          > PointToObjectIdSet;

    } // end namespace widgets
  }   // end namespace qt
}     // end namespace te

#endif  // __TERRALIB_QT_WIDGETS_INTERNAL_SCATTERUTILS_H