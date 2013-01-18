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
  \file terralib/qt/af/settings/GridFrameFactory.cpp

  \brief A factory to build the grid frame object.
*/

#include "ComponentWidgetFactory.h"
#include "GridFrame.h"
#include "GridFrameFactory.h"

te::qt::af::GridFrameFactory* te::qt::af::GridFrameFactory::sm_factory(0);


void te::qt::af::GridFrameFactory::initialize()
{
  finalize();
  sm_factory = new GridFrameFactory;
}

void te::qt::af::GridFrameFactory::finalize()
{
  delete sm_factory;
  sm_factory = 0;
}

const std::string& te::qt::af::GridFrameFactory::getType() const
{
  return "GridFrame";
}

te::qt::af::GridFrameFactory::~GridFrameFactory()
{

}

te::qt::af::ComponentWidget* te::qt::af::GridFrameFactory::build()
{
  return new GridFrame;
}

te::qt::af::GridFrameFactory::GridFrameFactory()
  : te::qt::af::ComponentWidgetFactory("GridFrame")
{

}

te::qt::af::ComponentWidget* te::qt::af::GridFrameFactory::create(QWidget* parent)
{
  return new GridFrame(parent);
}