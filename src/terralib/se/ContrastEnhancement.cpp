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
  \file terralib/se/ContrastEnhancement.cpp
  
  \brief ContrastEnhancement defines the 'stretching' of contrast for a channel of a false-color image or for a whole grey/color image.
*/

// TerraLib
#include "ContrastEnhancement.h"

te::se::ContrastEnhancement::ContrastEnhancement()
  : m_contrast(ENHANCEMENT_NONE),
    m_gammaValue(TE_SE_DEFAULT_GAMMA_VALUE)
{
}

te::se::ContrastEnhancement::~ContrastEnhancement()
{
}

void te::se::ContrastEnhancement::setContrastEnhancementType(ContrastEnhancementType t)
{
  m_contrast = t;
}

void te::se::ContrastEnhancement::setGammaValue(const double& v)
{
  m_gammaValue = v;
}

 te::se::ContrastEnhancement* te::se::ContrastEnhancement::clone() const
{
  ContrastEnhancement* ce = new ContrastEnhancement;
  ce->setContrastEnhancementType(m_contrast);
  ce->setGammaValue(m_gammaValue);

  return ce;
}
