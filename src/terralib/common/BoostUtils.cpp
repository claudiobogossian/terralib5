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
  \file terralib/common/BoostUtils.cpp

  \brief This file contains several utility functions for dealing with Boost containers and algorithms.
*/

// TerraLib
#include "BoostUtils.h"

// Boost
#include <boost/foreach.hpp>
#include <boost/property_tree/ptree.hpp>

void te::common::Convert(const boost::property_tree::ptree& p, std::map<std::string, std::string>& dict)
{
  BOOST_FOREACH(const boost::property_tree::ptree::value_type& v, p)
  {
    //if(v.first.data().empty())
    //if(v.second.data().empty())
      //continue;

    BOOST_FOREACH(const boost::property_tree::ptree::value_type& v2, v.second.get_child(""))
    {
      const std::string& f = v2.first.data();
      const std::string& s = v2.second.data();

      dict[f] = s;
    }
  }
}

