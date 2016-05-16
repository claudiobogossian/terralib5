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
  \file OutputWindow.cpp

  \brief A widget to output the script execution.
*/

// TerraLib
#include "OutputWindow.h"

// Qt
#include <QTextEdit>
#include <QVBoxLayout>

te::ce::OutputWindow::OutputWindow(QWidget* parent)
  : QDockWidget(parent)
{
  setWindowTitle(tr("Output"));

  m_txt = new QTextEdit(this);

  m_txt->setWordWrapMode(QTextOption::NoWrap);

  m_txt->setFontPointSize(4.0 * m_txt->fontPointSize());

  m_txt->setReadOnly(true);

  setWidget(m_txt);
}

te::ce::OutputWindow::~OutputWindow()
{
}

const QTextEdit* te::ce::OutputWindow::getText() const
{
  return m_txt;
}

QTextEdit* te::ce::OutputWindow::getText()
{
  return m_txt;
}

