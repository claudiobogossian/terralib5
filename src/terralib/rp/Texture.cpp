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
  \file terralib/rp/Texture.cpp
 
  \brief A structure to hold the set of GLCM metrics.
*/

// TerraLib
#include "Texture.h"

te::rp::Texture::Texture()
  : m_contrast(0.0),
    m_dissimilarity(0.0),
    m_energy(0.0),
    m_entropy(0.0),
    m_homogeneity(0.0)
{
}

te::rp::Texture::Texture(const Texture& rhs)
{
  m_contrast = rhs.m_contrast;
  m_dissimilarity = rhs.m_dissimilarity;
  m_energy = rhs.m_energy;
  m_entropy = rhs.m_entropy;
  m_homogeneity = rhs.m_homogeneity;
}

te::rp::Texture::~Texture()
{
  clear();
}

te::rp::Texture& te::rp::Texture::operator=(const Texture& rhs)
{
  if(this != &rhs)
  {
    clear();

    m_contrast = rhs.m_contrast;
    m_dissimilarity = rhs.m_dissimilarity;
    m_energy = rhs.m_energy;
    m_entropy = rhs.m_entropy;
    m_homogeneity = rhs.m_homogeneity;
  }

  return *this;
}

void te::rp::Texture::clear()
{
  m_contrast = 0.0;
  m_dissimilarity = 0.0;
  m_energy = 0.0;
  m_entropy = 0.0;
  m_homogeneity = 0.0;
}

