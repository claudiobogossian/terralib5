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
#include <boost/lexical_cast.hpp>

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

void te::common::Convert(const boost::property_tree::ptree& p, std::vector<std::string>& vect)
{
  BOOST_FOREACH(const boost::property_tree::ptree::value_type& v, p)
  {
    vect.push_back(v.second.data());
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


////void te::common::Convert(std::vector<double>vectd, te::gm::Envelope& env )
////{
////   std::vector<double>::iterator it;
////   double  d[4]={0.0,0.0,0.0,0.0}; int i=0;
////   for(it = vectd.begin(); it < vectd.end(); it++)
////   {
////       d[i] = (*it);       
////       i+= 1;
////   }
////   std::string s;
////   s = "marisa";
////   double  d0 = boost::lexical_cast<const double >(d[0]);
////   double  d1 = boost::lexical_cast<const double >(d[1]);
////   double  d2 = boost::lexical_cast<const double >(d[2]);
////   double  d3 = boost::lexical_cast<const double >(d[3]);
////   
////   te::gm::Envelope const env_aux(d0,d1,d2,d3);
////   //env.operator=(env_aux);
////   env.init(d[0],d[1],d[2],d[3]);
////   //env.init(d0,d1,d2,d3);
////   std::string s1;
////   s1 = "marisa1";
////}

////void te::common::Convert(const boost::property_tree::ptree& p, std::vector<te::gm::Envelope>& vect)
////{
////  BOOST_FOREACH(const boost::property_tree::ptree::value_type& v, p)
////  {
////    BOOST_FOREACH(const boost::property_tree::ptree::value_type& v2, v.second.get_child(""))
////    {
////      te::gm::Envelope env_i;
////      std::vector<double> envrep;
////      te::common::Convert(v2.second.get_child(""), envrep); 
////      te::common::Convert(envrep, env_i);
////      vect.push_back(env_i);
////      int xx;
////      xx=0;
////
////    }
////  }
////}

void te::common::Convert(const boost::property_tree::ptree& p, std::vector<std::vector<double>>& vect)
{
  BOOST_FOREACH(const boost::property_tree::ptree::value_type& v, p)
  {
    BOOST_FOREACH(const boost::property_tree::ptree::value_type& v2, v.second.get_child(""))
    {
      std::vector<double> envrep;
      te::common::Convert(v2.second.get_child(""), envrep);
      //te::gm::Envelope envelope_res;
      //te::common::Convert(envrep, envelope_res);
      {
          std::vector<double>::iterator it;
          double  d[4]={0.0,0.0,0.0,0.0}; int i=0;
          for(it = envrep.begin(); it < envrep.end(); it++)
           {
               d[i] = (*it);       
               i+= 1;
           }
          //te::gm::Envelope envelope1;
          //(d[0],d[1],d[2],d[3]);
          //double dd1 = envelope1.getLowerLeftX();
           std::string s;
           s = "marisa";
      }
      vect.push_back(envrep);
    }
  }
}

void te::common::Convert(const boost::property_tree::ptree& p, std::vector<std::map<std::string, std::string>>& vectm)
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