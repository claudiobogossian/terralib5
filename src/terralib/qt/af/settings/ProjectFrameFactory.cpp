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
  \file terralib/qt/af/settings/ProjectFrameFactory.cpp

  \brief A factory to build the project frame object.
*/

#include "ComponentWidgetFactory.h"
#include "ProjectFrame.h"
#include "ProjectFrameFactory.h"

te::qt::af::ProjectFrameFactory* te::qt::af::ProjectFrameFactory::sm_factory(0);


void te::qt::af::ProjectFrameFactory::initialize()
{
  finalize();
  sm_factory = new ProjectFrameFactory;
}

void te::qt::af::ProjectFrameFactory::finalize()
{
  delete sm_factory;
  sm_factory = 0;
}

const std::string& te::qt::af::ProjectFrameFactory::getType() const
{
  return "ProjectFrame";
}

te::qt::af::ProjectFrameFactory::~ProjectFrameFactory()
{

}

te::qt::af::ComponentWidget* te::qt::af::ProjectFrameFactory::build()
{
  return new ProjectFrame;
}

te::qt::af::ProjectFrameFactory::ProjectFrameFactory()
  : te::qt::af::ComponentWidgetFactory("ProjectFrame")
{

}

te::qt::af::ComponentWidget* te::qt::af::ProjectFrameFactory::create(QWidget* parent)
{
  return new ProjectFrame(parent);
}