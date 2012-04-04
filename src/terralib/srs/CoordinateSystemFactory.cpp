/*  Copyright (C) 2001-2009 National Institute For Space Research (INPE) - Brazil.

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
  \file CoordinateSystemFactory.cpp

  \brief An abstract factory for creating dataset coordinate system objects.
 */

// TerraLib
#include "../common/StringUtils.h"
#include "CoordinateSystemFactory.h"
#include "CoordinateSystem.h"


te::srs::CoordinateSystemFactory::~CoordinateSystemFactory()
{
}

te::srs::CoordinateSystemFactory::CoordinateSystemFactory(const std::string& factoryKey)
: te::common::AbstractFactory<CoordinateSystem, std::string>(factoryKey)
{
}

te::srs::CoordinateSystem* te::srs::CoordinateSystemFactory::make(const std::string& entity, int code)
{
  std::string ucase = te::common::Convert2UCase(entity);

  te::common::AbstractFactory<CoordinateSystem, std::string>::dictionary_type& d = te::common::AbstractFactory<CoordinateSystem, std::string>::getDictionary();

  CoordinateSystemFactory* f = static_cast<CoordinateSystemFactory*>(d.find(ucase));

  std::string desc = f->getDescription(code);

  if (desc.empty())
    return 0;

  return f->build(desc);
}


te::srs::CoordinateSystem* te::srs::CoordinateSystemFactory::make(const std::string& entity, const std::string& description)
{
  std::string ucase = te::common::Convert2UCase(entity);

  te::common::AbstractFactory<CoordinateSystem, std::string>::dictionary_type& d = te::common::AbstractFactory<CoordinateSystem, std::string>::getDictionary();

  CoordinateSystemFactory* f = static_cast<CoordinateSystemFactory*>(d.find(ucase));

  return f->build(description);
}

void te::srs::CoordinateSystemFactory::add(const std::string& entity, const std::string& description, int code)
{
  std::string ucase = te::common::Convert2UCase(entity);

  te::common::AbstractFactory<CoordinateSystem, std::string>::dictionary_type& d = te::common::AbstractFactory<CoordinateSystem, std::string>::getDictionary();

  CoordinateSystemFactory* f = static_cast<CoordinateSystemFactory*>(d.find(ucase));

  f->add(description,code);
}

std::string te::srs::CoordinateSystemFactory::getDescription(const std::string& entity, int code)
{
  std::string ucase = te::common::Convert2UCase(entity);

  te::common::AbstractFactory<CoordinateSystem, std::string>::dictionary_type& d = te::common::AbstractFactory<CoordinateSystem, std::string>::getDictionary();

  CoordinateSystemFactory* f = static_cast<CoordinateSystemFactory*>(d.find(ucase));

  return f->getDescription(code);
}

int te::srs::CoordinateSystemFactory::add(const std::string& entity, const std::string& description)
{
  std::string ucase = te::common::Convert2UCase(entity);
  
  te::common::AbstractFactory<CoordinateSystem, std::string>::dictionary_type& d = te::common::AbstractFactory<CoordinateSystem, std::string>::getDictionary();
  
  CoordinateSystemFactory* f = static_cast<CoordinateSystemFactory*>(d.find(ucase));
  
  int code = f->suggestCode();
  
  f->add(description,code);
  
  return code;
}
