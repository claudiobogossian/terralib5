/*  Copyright (C) 2011-2013 National Institute For Space Research (INPE) - Brazil.

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
  \file terralib/qt/af/settings/DisplayFrameFactory.cpp

  \brief A factory to build the display frame object.
*/

#include "DisplayFrame.h"
#include "DisplayFrameFactory.h"

te::qt::af::DisplayFrameFactory* te::qt::af::DisplayFrameFactory::sm_factory(0);

void te::qt::af::DisplayFrameFactory::initialize()
{
  finalize();
  sm_factory = new DisplayFrameFactory;
}

void te::qt::af::DisplayFrameFactory::finalize()
{
  delete sm_factory;
  sm_factory = 0;
}

const std::string& te::qt::af::DisplayFrameFactory::getType() const
{
  return "DisplayFrame";
}

te::qt::af::DisplayFrameFactory::~DisplayFrameFactory()
{

}

te::qt::af::ComponentWidget* te::qt::af::DisplayFrameFactory::build()
{
  return new DisplayFrame;
}

te::qt::af::DisplayFrameFactory::DisplayFrameFactory()
  : te::qt::af::ComponentWidgetFactory("DisplayFrame")
{

}

te::qt::af::ComponentWidget* te::qt::af::DisplayFrameFactory::create(QWidget* parent)
{
  return new DisplayFrame(parent);
}