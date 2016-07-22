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
  \file ScriptWidget.cpp

  \brief A widget that can be used to show and control he execution of a script.
*/

#include "ScriptWidget.h"
#include "Utils.h"

// TerraLib
#include "../../vm/core/VirtualMachine.h"
#include "../../vm/core/VirtualMachineManager.h"

// Qt
#include <QFileDialog>
#include <QFile>
#include <QFileInfo>
#include <QTextStream>
#include <QApplication>
#include <QMessageBox>
#include <QVBoxLayout>

// QScintilla
#include <Qsci/qsciscintilla.h>

#include <iostream>

te::ce::ScriptWidget::ScriptWidget(QWidget* parent)
  : QWidget(parent),
    m_txtEditor(nullptr),
    m_lexer(nullptr),
    m_unsaved(false)
{
  m_txtEditor = new QsciScintilla(this);
  
  m_txtEditor->setAutoCompletionSource(QsciScintilla::AcsAll);
  
  m_txtEditor->setMarginLineNumbers(1, true);
  
  m_txtEditor->setFolding(QsciScintilla::PlainFoldStyle, 2);

  QVBoxLayout* mainLayout = new QVBoxLayout;
  
  mainLayout->addWidget(m_txtEditor);
  
  setLayout(mainLayout);

  connect(m_txtEditor, SIGNAL(textChanged()), SLOT(setTextChanged()));
}

te::ce::ScriptWidget::~ScriptWidget()
{
}

QString te::ce::ScriptWidget::getScriptType() const
{
  if(m_fileName.isEmpty())
    return QString(tr("lang-unknown"));

  QFileInfo info(m_fileName);

  return info.suffix();
}

QString te::ce::ScriptWidget::getScriptName() const
{
  return QString(tr("New-Script"));
}

void te::ce::ScriptWidget::open(const QString& fileName)
{
// close any previous opened script
//  close();

  QFile file(fileName);

  if(!file.open(QFile::ReadOnly))
  {
    QMessageBox::warning(this,
                         tr("TerraLib"),
                         tr("Cannot read file %1:\n%2.").arg(fileName).arg(file.errorString()));
    return;
  }

  QTextStream ifile(&file);

  QApplication::setOverrideCursor(Qt::WaitCursor);

  m_txtEditor->setText(ifile.readAll());

  QApplication::restoreOverrideCursor();

  m_fileName = fileName;

  setLexer();
}

void te::ce::ScriptWidget::save(const QString& fileName)
{
  m_fileName = fileName;

  save();
}

bool te::ce::ScriptWidget::hasChanged() const
{
  return m_unsaved;
}

void te::ce::ScriptWidget::save()
{
  if(m_fileName == 0)
    saveAs();
  else
    saveFile(m_fileName);

  setUnsaved(false);
}

void te::ce::ScriptWidget::saveAs()
{
  QString fileName = QFileDialog::getSaveFileName(this);

  if(fileName.isEmpty())
    return;

  m_fileName = fileName;

  saveFile(fileName);

  setLexer();

  setUnsaved(false);
}

void te::ce::ScriptWidget::close()
{
// save changes
  save();

// clear text editor
  m_txtEditor->clear();

  m_fileName = "";

// set to no-changes state
  setUnsaved(false);
}

void te::ce::ScriptWidget::execute()
{
// save changes before executing
  //if(m_hasChanged)
  //  save();
  //else if(!m_hasChanged && (m_fileName.isEmpty())) // don't execute if text editor is empty!
  //  return;

  try
  {
    te::vm::core::VirtualMachine* vm = te::vm::core::VirtualMachineManager::instance().get(getScriptType().toStdString());

    vm->build(m_fileName.toStdString());

    vm->execute();
  }
  catch(const std::exception& e)
  {
    QMessageBox m(QMessageBox::Critical,
                  tr("TerraLib Code Editor"),
                  tr("Could not execute the script due to the following problem: %1.").arg(e.what()),
                  QMessageBox::Ok,
                  this);
    m.exec();
  }
  catch(...)
  {
    QMessageBox m(QMessageBox::Critical,
                  tr("TerraLib Code Editor"),
                  tr("Could not execute the script due to an unknown error!"),
                  QMessageBox::Ok,
                  this);
    m.exec();
  }
}


void te::ce::ScriptWidget::stop()
{
}


void te::ce::ScriptWidget::pause()
{
}

void te::ce::ScriptWidget::zoomIn()
{
  m_txtEditor->zoomIn();
  m_txtEditor->setMarginWidth(1, QString("0000"));
}

void te::ce::ScriptWidget::zoomOut()
{
  m_txtEditor->zoomOut();
  m_txtEditor->setMarginWidth(1, QString("0000"));
}

void te::ce::ScriptWidget::setTextChanged()
{
  setUnsaved(true);
}

void te::ce::ScriptWidget::saveFile(const QString& fileName)
{
  QFile file(fileName);

  if(!file.open(QFile::WriteOnly))
  {
    QMessageBox::warning(this,
                         tr("TerraLib Code Editor"),
                         tr("Cannot write file %1:\n%2.").arg(fileName).arg(file.errorString()));
    return;
  }

  QTextStream ofile(&file);

  QApplication::setOverrideCursor(Qt::WaitCursor);

  ofile << m_txtEditor->text();

  QApplication::restoreOverrideCursor();
}

void te::ce::ScriptWidget::setLexer()
{
  if(m_lexer || (m_fileName.isEmpty()))
  {
    m_txtEditor->setLexer(0);

    delete m_lexer;
    m_lexer = 0;
  }

  if(m_fileName.isEmpty())
    return;

  QString lang = getScriptType();

  m_lexer = LexerFactory(lang, m_txtEditor);
  
  m_txtEditor->setLexer(m_lexer);
}

void te::ce::ScriptWidget::setUnsaved(bool unsaved)
{
  if(m_unsaved == unsaved)
    return;

  m_unsaved = unsaved;
  m_txtEditor->setModified(m_unsaved);

  emit codeChanged();
}
