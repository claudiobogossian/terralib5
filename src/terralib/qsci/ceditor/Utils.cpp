/*
  Copyright (C) 2008 National Institute For Space Research (INPE) - Brazil.

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

#include "Utils.h"

// Qt
#include <QFileInfo>

QIcon te::ce::GetIcon(QString fileName)
{
  QFileInfo in(fileName);

  QString lang(in.suffix()),
      ulang = lang.toUpper();

  if(ulang == "LUA")
    return QIcon(":/imgs/imgs/lang-lua.svg");

  if((ulang == "PY") || (ulang == "PYTHON"))
    return QIcon(":/imgs/imgs/lang-python.svg");

  return QIcon();
}
