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
  \file ScriptWidget.cpp

  \brief A widget that can be used to show and control he execution of a script.
*/

// TerraLib
#include "../../../common/StringUtils.h"
#include "LexerFactory.h"
#include "ScriptWidget.h"

//// TerraLib
//#include <terralib/binding/vm/VirtualMachine.h>
//#include <terralib/binding/vm/VirtualMachineFactory.h>
//#include <terralib/binding/vm/VirtualMachineManager.h>

// Boost
#include <boost/filesystem.hpp>
#include <boost/format.hpp>

// Qt
#include <QtCore/QFile>
#include <QtCore/QTextStream>
#include <QtGui/QApplication>
#include <QtGui/QFileDialog>
#include <QtGui/QMessageBox>
#include <QtGui/QVBoxLayout>

// QScintilla
#include <Qsci/qsciscintilla.h>

te::qt::widgets::ScriptWidget::ScriptWidget(QWidget* parent)
  : QWidget(parent),
    m_txtEditor(0),
    m_lexer(0),
    m_fileName(0),
    m_hasChanged(false)
{
  m_txtEditor = new QsciScintilla(this);  
  
  m_txtEditor->setAutoCompletionSource(QsciScintilla::AcsAll);
  
  m_txtEditor->setMarginLineNumbers(1, true);
  
  m_txtEditor->setFolding(QsciScintilla::PlainFoldStyle, 2);

  QVBoxLayout* mainLayout = new QVBoxLayout;
  
  mainLayout->addWidget(m_txtEditor);
  
  setLayout(mainLayout);

  connect(m_txtEditor, SIGNAL(textChanged()), this, SLOT(setTextChanged()));
}

te::qt::widgets::ScriptWidget::~ScriptWidget()
{
// dont't save if close is called, let's wait for application decision!
  //close();

  delete m_fileName;
}

QString te::qt::widgets::ScriptWidget::getScriptType() const
{
  if(m_fileName == 0)
    return QString(tr("lang-unknown"));

  boost::filesystem::path fpath(m_fileName->toStdString());

  //testar: extension() ou inves de stem()
  std::string fext = te::common::Convert2UCase(fpath.stem().string());

  std::string::size_type pos = fext.find('.');

  if(pos != std::string::npos)
    return QString(fext.substr(pos + 1).c_str());
  else
    return QString(fext.c_str());
}

QString te::qt::widgets::ScriptWidget::getScriptName() const
{
  if(m_fileName == 0)
    return QString(tr("New-Script"));

  boost::filesystem::path fpath(m_fileName->toStdString());

  return QString(fpath.filename().string().c_str());
}

void te::qt::widgets::ScriptWidget::open(const QString& fileName)
{
// close any previous opened script
  close();

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

  m_fileName = new QString(fileName);

  setLexer();

  m_hasChanged = false;
}

void te::qt::widgets::ScriptWidget::save()
{
  if(m_fileName == 0)
    saveAs();
  else
    saveFile(*m_fileName);

  m_hasChanged = false;
}

void te::qt::widgets::ScriptWidget::saveAs()
{
  QString fileName = QFileDialog::getSaveFileName(this);

  if(fileName.isEmpty())
    return;

  delete m_fileName;

  m_fileName = new QString(fileName);

  saveFile(fileName);

  setLexer();

  m_hasChanged = false;
}

void te::qt::widgets::ScriptWidget::close()
{
// save changes
  if(m_hasChanged)
    save();

// clear text editor
  m_txtEditor->clear();

// release file name
  delete m_fileName;

  m_fileName = 0;

// set to no-changes state
  m_hasChanged = false;
}

void te::qt::widgets::ScriptWidget::execute()
{
// save changes before executing
  if(m_hasChanged)
    save();
  else if(!m_hasChanged && (m_fileName == 0)) // don't execute if text editor is empty!
    return;

  //try
  //{
  //  te::vm::VirtualMachine* vm = te::vm::VirtualMachineFactory::make("LUA_VM");

  //  vm->build(m_fileName->toStdString());
  //  
  //  vm->execute();
  //}
  //catch(const std::exception& e)
  //{
  //  QMessageBox m(QMessageBox::Critical,
  //                tr("TerraLib Code Editor"),
  //                tr("Could not execute the script due to the following problem: %1.").arg(e.what()),
  //                QMessageBox::Ok,
  //                this);
  //  m.exec();
  //}
  //catch(...)
  //{
  //  QMessageBox m(QMessageBox::Critical,
  //                tr("TerraLib Code Editor"),
  //                tr("Could not execute the script due to an unknown error!"),
  //                QMessageBox::Ok,
  //                this);
  //  m.exec();
  //}
}


void te::qt::widgets::ScriptWidget::stop()
{
}


void te::qt::widgets::ScriptWidget::pause()
{
}

void te::qt::widgets::ScriptWidget::zoomIn()
{
  m_txtEditor->zoomIn();
  m_txtEditor->setMarginWidth(1, QString("0000"));
}

void te::qt::widgets::ScriptWidget::zoomOut()
{
  m_txtEditor->zoomOut();
  m_txtEditor->setMarginWidth(1, QString("0000"));
}

void te::qt::widgets::ScriptWidget::setTextChanged()
{
  m_hasChanged = true;
}

void te::qt::widgets::ScriptWidget::saveFile(const QString& fileName)
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

  //statusBar()->showMessage(tr("File saved"), 2000);
}

void te::qt::widgets::ScriptWidget::setLexer()
{
  //if(m_lexer || (m_fileName == 0))
  //{
    //m_txtEditor->setLexer(0);
    //delete m_lexer;
    //m_lexer = 0;
  //}

  if(m_fileName == 0)
    return;

  QString lang = getScriptType();

  m_lexer = LexerFactory::make(lang, m_txtEditor);
  
  m_txtEditor->setLexer(m_lexer);
}

