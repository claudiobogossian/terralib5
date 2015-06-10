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
  \file terralib/maptools/ExternalGraphicRendererManager.cpp

  \brief This is a singleton for managing all external graphic renderers instances available in the system.
*/

// TerraLib
#include "../common/STLUtils.h"
#include "../common/Translator.h"
#include "AbstractExternalGraphicRenderer.h"
#include "Exception.h"
#include "ExternalGraphicRendererManager.h"

// STL
#include <cassert>
#include <vector>

te::map::ExternalGraphicRendererManager::ExternalGraphicRendererManager()
{
}

te::map::ExternalGraphicRendererManager::~ExternalGraphicRendererManager()
{
  te::common::FreeContents(m_renderers);
}

te::color::RGBAColor** te::map::ExternalGraphicRendererManager::render(const te::se::ExternalGraphic* eg, std::size_t height, std::size_t& width)
{
  LockWrite l;

  if(height == 0)
    throw Exception(TE_TR("Requested size is invalid!"));

  if(eg == 0)
    throw Exception(TE_TR("The given external graphic is invalid!"));

  if(m_renderers.empty())
    throw Exception(TE_TR("There are not external graphic renderers registered!"));

  for(std::size_t i = 0; i < m_renderers.size(); ++i)
  {
    te::color::RGBAColor** image = m_renderers[i]->render(eg, height, width);
    if(image != 0)
      return image;
  }

  throw Exception(TE_TR("The given external graphic cannot be processed by any registered external graphic renderer!"));
}

void te::map::ExternalGraphicRendererManager::add(AbstractExternalGraphicRenderer* renderer)
{
  m_renderers.push_back(renderer);
}
