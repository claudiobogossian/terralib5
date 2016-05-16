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

/*!
  \file terralib/qsci/ceditor/Utils.cpp

  \brief Utility functions for dealing with code editor.
*/


// TerraLib
#include "Utils.h"

// Qt
#include <QFileInfo>
#include <QString>

// QScintilla
//#include <Qsci/qscilexercpp.h>
//#include <Qsci/qscilexerd.h>
//#include <Qsci/qscilexerjava.h>
//#include <Qsci/qscilexerjavascript.h>
#include <Qsci/qscilexerlua.h>
#include <Qsci/qscilexerpython.h>
//#include <Qsci/qscilexerruby.h>

QsciLexer*
te::ce::LexerFactory(const QString& lang, QObject* parent)
{
  const QString ulang = lang.toUpper();

  if(ulang == "LUA")
    return new QsciLexerLua(parent);

  if((ulang == "PY") || (ulang == "PYTHON"))
    return new QsciLexerPython(parent);

  return nullptr;
}

QIcon te::ce::ScriptIconFactory(const QString& lang)
{
  const QString ulang = lang.toUpper();
  
  if(ulang == "LUA")
    return QIcon::fromTheme("lang-lua");

  if((ulang == "PY") || (ulang == "PYTHON"))
    return QIcon::fromTheme("lang-python");

  return QIcon::fromTheme("lang-unknown");
}

QIcon te::ce::CreateLangIcon(const QString& fileName)
{
  QFileInfo in(fileName);

  QString lang(in.suffix()),
      ulang = lang.toUpper();

  return ScriptIconFactory(ulang);
}
