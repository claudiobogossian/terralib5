/*  Copyright (C) 2008-2011 National Institute For Space Research (INPE) - Brazil.

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
  \file terralib/qt/widgets/ExternalGraphicRenderer.cpp

  \brief A concrete renderer based on Qt4 for conversion of Symbology Enconding External Graphic elements to an image pattern.
*/

// TerraLib
#include "../../../se/ExternalGraphic.h"
#include "../../../xlink/SimpleLink.h"
#include "../Utils.h"
#include "ExternalGraphicRenderer.h"

// Qt
#include <QImage>

te::qt::widgets::ExternalGraphicRenderer::ExternalGraphicRenderer()
  : te::map::AbstractExternalGraphicRenderer()
{
}

te::qt::widgets::ExternalGraphicRenderer::~ExternalGraphicRenderer()
{
}

te::color::RGBAColor** te::qt::widgets::ExternalGraphicRenderer::render(const te::se::ExternalGraphic* eg, std::size_t height, std::size_t& width)
{
  /* For while this method only looks for URI on OnlineResource
   and consider that the URI references a local image file. (Uba, Feb 2013) */

  const te::xl::SimpleLink* link = eg->getOnlineResource();
  if(link == 0)
    return 0;

  const std::string uri = link->getHref();
  if(uri.empty())
    return 0;

  // Try load the image
  QImage img;
  if(!img.load(uri.c_str()))
    return 0;

  // Verifies the requested size
  if(img.height() != height)
    img = img.scaledToHeight(height, Qt::SmoothTransformation);

  width = img.width();

  // Converts QImage to te::color::RGBA**
  te::color::RGBAColor** rgba = te::qt::widgets::GetImage(&img);

  return rgba;
}
