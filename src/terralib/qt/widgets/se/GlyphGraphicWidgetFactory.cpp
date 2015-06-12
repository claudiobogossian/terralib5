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
  \file terralib/qt/widgets/se/GlyphGraphicWidgetFactory.cpp

  \brief A concrete factory that builds a graphic widget used to configure a graphic associate with a glyph mark element.
*/

// TerraLib
#include "GlyphGraphicWidget.h"
#include "GlyphGraphicWidgetFactory.h"

// Factory key
std::string te::qt::widgets::GlyphGraphicWidgetFactory::sm_factoryKey("GlyphMarker");

// Global factory
te::qt::widgets::GlyphGraphicWidgetFactory* te::qt::widgets::GlyphGraphicWidgetFactory::sm_factory(0);

void te::qt::widgets::GlyphGraphicWidgetFactory::initialize()
{
  finalize();
  sm_factory = new GlyphGraphicWidgetFactory;
}

void te::qt::widgets::GlyphGraphicWidgetFactory::finalize()
{
  delete sm_factory;
  sm_factory = 0;
}

te::qt::widgets::GlyphGraphicWidgetFactory::~GlyphGraphicWidgetFactory()
{
}

te::qt::widgets::AbstractGraphicWidget* te::qt::widgets::GlyphGraphicWidgetFactory::build()
{
  return new GlyphGraphicWidget;
}

te::qt::widgets::GlyphGraphicWidgetFactory::GlyphGraphicWidgetFactory()
  : te::qt::widgets::AbstractGraphicWidgetFactory(sm_factoryKey)
{
}
