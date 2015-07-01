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
 \file terralib/metadata/iso19115/MD_Distribution.cpp
 */

#include "MD_Distribution.h"
#include "MD_Format.h"

#include <cassert>
      
te::md::MD_Distribution::MD_Distribution()
{} 

te::md::MD_Distribution::~MD_Distribution()
{} 

void
te::md::MD_Distribution::addDistrOption(te::md::MD_Format* opt)
{
  assert(opt);
  m_distrFormats.push_back(opt);
}

const boost::ptr_vector<te::md::MD_Format>& 
te::md::MD_Distribution::getFormats() const
{
  return m_distrFormats;
}

void 
te::md::MD_Distribution::setURL(const std::string& url)
{
  m_url = url;
}

const std::string& 
te::md::MD_Distribution::getURL() const
{
  return m_url;
}