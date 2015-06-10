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
  \file OpenModules.cpp
   
  \brief The main entry function for registering TerraLib API in a V8 environment.
 */

// TerraLib
#include "../dataaccess/DataAccess.h"
#include "../geometry/Geometry.h"
#include "OpenModules.h"

void te::v8::jsi::RegisterModule(::v8::Local<::v8::Object>& global)
{
// TODO: checar a necessidade de inicializar os plugins como feito em Lua e Java

// Geometry
  RegisterCoord2D(global);
  RegisterEnvelope(global);
  RegisterGeometryFactory(global);
  RegisterGeometry(global);
  RegisterGeometryCollection(global);
  RegisterLineString(global);
  RegisterLinearRing(global);
  RegisterMultiLineString(global);
  RegisterMultiPoint(global);
  RegisterMultiPolygon(global);
  RegisterPoint(global);
  RegisterPolygon(global);

// DataAccess
  RegisterDataSetType(global);
  RegisterDataSourceManager(global);
  RegisterDataSourceFactory(global);
  RegisterDataSource(global);
}

