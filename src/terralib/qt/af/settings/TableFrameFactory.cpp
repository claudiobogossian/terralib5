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
  \file terralib/qt/af/settings/TableFrameFactory.cpp

  \brief A factory to build the Table frame object.
*/

#include "ComponentWidgetFactory.h"
#include "TableFrame.h"
#include "TableFrameFactory.h"

te::qt::af::TableFrameFactory* te::qt::af::TableFrameFactory::sm_factory(0);


void te::qt::af::TableFrameFactory::initialize()
{
  finalize();
  sm_factory = new TableFrameFactory;
}

void te::qt::af::TableFrameFactory::finalize()
{
  delete sm_factory;
  sm_factory = 0;
}

const std::string& te::qt::af::TableFrameFactory::getType() const
{
  return "TableFrame";
}

te::qt::af::TableFrameFactory::~TableFrameFactory()
{

}

te::qt::af::ComponentWidget* te::qt::af::TableFrameFactory::build()
{
  return new TableFrame;
}

te::qt::af::TableFrameFactory::TableFrameFactory()
  : te::qt::af::ComponentWidgetFactory("TableFrame")
{

}

te::qt::af::ComponentWidget* te::qt::af::TableFrameFactory::create(QWidget* parent)
{
  return new TableFrame(parent);
}