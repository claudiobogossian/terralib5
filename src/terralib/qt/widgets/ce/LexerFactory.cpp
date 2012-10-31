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
  \file LexerFactory.cpp

  \brief A factory method for language lexers.
*/

// TerraLib
#include "LexerFactory.h"

// Qt
#include <QtCore/QString.h>

// QScintilla
#include <Qsci/qscilexercpp.h>
#include <Qsci/qscilexerd.h>
#include <Qsci/qscilexerjava.h>
#include <Qsci/qscilexerjavascript.h>
#include <Qsci/qscilexerlua.h>
#include <Qsci/qscilexerpython.h>
#include <Qsci/qscilexerruby.h>

QsciLexer* te::qt::widgets::LexerFactory::make(const QString& lang, QObject* parent)
{
  const QString ulang = lang.toUpper();

  if(ulang == "LUA")
  {
    return new QsciLexerLua(parent);
  }
  else if((ulang == "JS") || (ulang == "JAVASCRIPT") || (ulang == "JSCRIPT"))
  {
    return new QsciLexerJavaScript(parent);
  }
  else if((ulang == "H") || (ulang == "HPP") || (ulang == "C") || (ulang == "CPP") || (ulang == "CXX"))
  {
    return new QsciLexerCPP(parent);
  }
  else if((ulang == "J") || (ulang == "JAVA"))
  {
    return new QsciLexerJava(parent);
  }
  else if((ulang == "D"))
  {
    return new QsciLexerD(parent);
  }
  else if((ulang == "PY") || (ulang == "PYTHON"))
  {
    return new QsciLexerPython(parent);
  }
  else if((ulang == "RUBY") || (ulang == "RBY"))
  {
    return new QsciLexerRuby(parent);
  }
  else
  {
    return 0;
  }
}

