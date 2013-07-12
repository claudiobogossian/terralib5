/*  Copyright (C) 2011-2011 National Institute For Space Research (INPE) - Brazil.

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
  \file ScriptIconFactory.cpp

  \brief A factory method for language icons.
*/

// TerraLib
#include "ScriptIconFactory.h"

// Qt
#include <QtCore/QString.h>

QIcon te::qt::widgets::ScriptIconFactory::make(const QString& lang)
{
  const QString ulang = lang.toUpper();

  if(ulang == "LUA")
  {
    return QIcon::fromTheme("lang-lua");
  }
  else if((ulang == "JS") || (ulang == "JAVASCRIPT") || (ulang == "JSCRIPT"))
  {
    return QIcon::fromTheme("lang-javascript");
  }
  else if((ulang == "H") || (ulang == "HPP") || (ulang == "C") || (ulang == "CPP") || (ulang == "CXX"))
  {
    return QIcon::fromTheme("lang-cpp");
  }
  else if((ulang == "J") || (ulang == "JAVA"))
  {
    return QIcon::fromTheme("lang-java");
  }
  else if((ulang == "D"))
  {
    return QIcon::fromTheme("lang-d");
  }
  else if((ulang == "PY") || (ulang == "PYTHON"))
  {
    return QIcon::fromTheme("lang-python");
  }
  else if((ulang == "RUBY") || (ulang == "RBY"))
  {
    return QIcon::fromTheme("lang-ruby");
  }
  else
  {
    return QIcon::fromTheme("lang-unknown");
  }
}

