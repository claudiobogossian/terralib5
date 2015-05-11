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
  \file terralib/common/BoostUtils.cpp

  \brief This file contains several utility functions for dealing with Boost containers and algorithms.
*/

// TerraLib
#include "BoostUtils.h"

// Boost
#include <boost/foreach.hpp>
#include <boost/lexical_cast.hpp>

//#include <boost/locale.hpp>

//#include <boost/locale/encoding.hpp>
//#include <boost/locale/util.hpp>
#include <boost/property_tree/ptree.hpp>


//class std::locale::id boost::locale::info::id;

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

void te::common::Convert(const boost::property_tree::ptree& p, std::vector<std::pair<std::string, std::string> > & vp)
{
  BOOST_FOREACH(const boost::property_tree::ptree::value_type& v, p)
  {
    BOOST_FOREACH(const boost::property_tree::ptree::value_type& v2, v.second.get_child(""))
    {
      //const std::string& f = v2.first.data();
      //const std::string& s = v2.second.data();
      std::pair<std::string, std::string> in_pair;
      in_pair.first = v2.first.data();
      in_pair.second = v2.second.data();
      vp.push_back(in_pair);
    }
  }
}

void te::common::Convert(const boost::property_tree::ptree& p, std::vector<std::string>& vect)
{
  BOOST_FOREACH(const boost::property_tree::ptree::value_type& v, p)
  {
    vect.push_back(v.second.data());
    continue;
  }
}

void te::common::Convert(const boost::property_tree::ptree& p, std::vector<unsigned char>& vectd)
{
  BOOST_FOREACH(const boost::property_tree::ptree::value_type& v, p)
  {
    //vectd.push_back(boost::lexical_cast<unsigned char>(v.second.data()));
    vectd.push_back(boost::lexical_cast<unsigned char>(v.second.data()));
    continue;
  }
}

void te::common::Convert(const boost::property_tree::ptree& p, std::vector<double>& vectd)
{
  BOOST_FOREACH(const boost::property_tree::ptree::value_type& v, p)
  {
    vectd.push_back(boost::lexical_cast<double>(v.second.data()));
    continue;
  }
}

void te::common::Convert(const boost::property_tree::ptree& p, std::vector<size_t>& vect)
{
  BOOST_FOREACH(const boost::property_tree::ptree::value_type& v, p)
  {
    vect.push_back(boost::lexical_cast<size_t>(v.second.data()));
    continue;
  }
}

void te::common::Convert(const boost::property_tree::ptree& p, std::vector<std::vector<double> >& vect)
{
  BOOST_FOREACH(const boost::property_tree::ptree::value_type& v, p)
  {
    BOOST_FOREACH(const boost::property_tree::ptree::value_type& v2, v.second.get_child(""))
    {
      std::vector<double> envrep;
      te::common::Convert(v2.second.get_child(""), envrep);
      //{
      //    std::vector<double>::iterator it;
      //    double  d[4]={0.0,0.0,0.0,0.0}; int i=0;
      //    for(it = envrep.begin(); it < envrep.end(); it++)
      //     {
      //         d[i] = (*it);
      //         i+= 1;
      //     }
      //}
      vect.push_back(envrep);
    }
  }
}

void te::common::Convert(const boost::property_tree::ptree& p, std::vector<std::map<std::string, std::string> >& vectm)
{
  BOOST_FOREACH(const boost::property_tree::ptree::value_type& v, p)
  {
    // used to get the properties parameters into a map
    std::map<std::string, std::string> dict;
    BOOST_FOREACH(const boost::property_tree::ptree::value_type& v2, v.second.get_child(""))
    {
      const std::string& f = v2.first.data();
      const std::string& s = v2.second.data();

      dict[f] = s;
    }
    vectm.push_back(dict);
  }
}

//std::string GetSystemEncoding()
//{
//  std::string loc_name = boost::locale::util::get_system_locale();
//
//  size_t pos = loc_name.find(".");
//
//  if(pos == std::string::npos)
//    return "";
//
//  std::string encod = loc_name.substr(pos+1);
//
//  if(encod.compare("windows-1252") == 0)
//    return "ISO8859-1";
//
//  return encod;
//}

std::string te::common::ConvertLatin1UTFString(const std::string& data, const bool& toUtf)
{
  return data;
  //std::string encoding = GetSystemEncoding();

  //if(encoding.empty())
  //  return data;

  //if(toUtf)
  //  return boost::locale::conv::to_utf<char>(data, "ISO8859-1");
  //else
  //{
  //  if(encoding.compare("ISO8859-1") == 0)
  //    return data;

  //  std::string word = data;

  //  if (encoding.compare("UTF-8") != 0)
  //    word = boost::locale::conv::to_utf<char>(data, encoding);

  //  return boost::locale::conv::from_utf<char>(data, "ISO8859-1");
  //}
}
