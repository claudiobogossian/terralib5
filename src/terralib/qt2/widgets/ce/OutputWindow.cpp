/*  Copyright (C) 2011-20011 National Institute For Space Research (INPE) - Brazil.

    This file is part of TerraLib Code Editor - A widget component for scripting with TerraLib Platform.

    TerraLib Code Editor is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License,
    or (at your option) any later version.

    TerraLib Code Editor is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with TerraLib Code Editor. See COPYING. If not, write to
    Gilberto Ribeiro de Queiroz at <gribeiro@dpi.inpe.br>.
 */

/*!
  \file OutputWindow.cpp

  \brief A widget to output the script execution.
*/

// TerraLib
#include "OutputWindow.h"

// Qt
#include <QtGui/QTextEdit>
#include <QtGui/QVBoxLayout>

tce::OutputWindow::OutputWindow(QWidget* parent)
  : QDockWidget(parent)
{
  setWindowTitle(tr("Output"));

  m_txt = new QTextEdit(this);

  m_txt->setWordWrapMode(QTextOption::NoWrap);

  m_txt->setFontPointSize(4.0 * m_txt->fontPointSize());

  m_txt->setReadOnly(true);

  setWidget(m_txt);
}

tce::OutputWindow::~OutputWindow()
{
}

const QTextEdit* tce::OutputWindow::getText() const
{
  return m_txt;
}

QTextEdit* tce::OutputWindow::getText()
{
  return m_txt;
}

