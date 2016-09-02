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

// Example
#include "MainWindow.h"
#include <ui_CodeEditorForm.h>

// TerraLib
#include <terralib/core/utils/Platform.h>
#include <terralib/qsci/ceditor/ScriptWidget.h>
#include <terralib/qsci/ceditor/Utils.h>

// Qt
#include <QFileDialog>

// STL
#include <iostream>

class OutPutStream : public std::basic_streambuf<char>
{
  public:

    OutPutStream(std::ostream &stream, QTextEdit* text_edit): m_stream(stream)
    {
      log_window = text_edit;
      m_old_buf = stream.rdbuf();
      stream.rdbuf(this);
    }
  
    ~OutPutStream()
    {
// output anything that is left
      if(!m_string.empty())
        log_window->append(m_string.c_str());

      m_stream.rdbuf(m_old_buf);
    }

    static void registerQDebugMessageHandler()
    {
      qInstallMessageHandler(myQDebugMessageHandler);
    }

  protected:

    virtual int_type overflow(int_type v)
    {
      if(v == '\n')
      {
        log_window->append("");
      }
      return v;
    }

  virtual std::streamsize xsputn(const char *p, std::streamsize n)
  {
    QString str(p);
    if(str.contains("\n"))
    {
      QStringList strSplitted = str.split("\n");

      log_window->moveCursor(QTextCursor::End);
      log_window->insertPlainText(strSplitted.at(0)); //Index 0 is still on the same old line

      for(int i = 1; i < strSplitted.size(); i++)
      {
        log_window->append(strSplitted.at(i));
      }
    }
    else
    {
      log_window->moveCursor(QTextCursor::End);
      log_window->insertPlainText(str);
    }
    return n;
  }

private:

  static void myQDebugMessageHandler(QtMsgType, const QMessageLogContext &, const QString &msg)
  {
    std::cout << msg.toUtf8().data();
  }

  std::ostream &m_stream;
  std::streambuf *m_old_buf;
  std::string m_string;

  QTextEdit* log_window;
};

MainWindow::MainWindow(QWidget *parent):
QMainWindow(parent)
{
  m_ui = new Ui::MainWindow;

  m_ui->setupUi(this);

  m_out = new OutPutStream(std::cout, m_ui->m_txt);

  OutPutStream::registerQDebugMessageHandler();

  m_ui->m_txt->setWordWrapMode(QTextOption::NoWrap);

  m_ui->m_txt->setReadOnly(true);
}

MainWindow::~MainWindow()
{
  delete m_out;
}

void MainWindow::open()
{
  std::string exPath = te::core::FindInTerraLibPath("share/terralib/examples/lua");

  m_fileName = QFileDialog::getOpenFileName(this, tr("Open Script File"), QString::fromUtf8(exPath.c_str()), tr("Lua files (*.lua *.LUA);; Pyhton files (*.py *.PY)"), 0);

  if(!m_fileName.isEmpty())
  {
    QFileInfo info(m_fileName);

    QString scName = info.fileName();

    te::ce::ScriptWidget* editor = new te::ce::ScriptWidget(this);

    m_ui->m_codeTab->insertTab(0, editor, te::ce::CreateLangIcon(m_fileName), scName);

    editor->open(m_fileName);

    connect(editor, SIGNAL(codeChanged()), SLOT(codeChanged()));
  }
}

void MainWindow::newScript()
{
  m_fileName.clear();
}

void MainWindow::save()
{
  QString selFilter;

  std::map<QString, QString> ext;

  QString luaFilter = tr("Lua files (*.lua *.LUA)"),
    pyFilter = tr("Pyhton files (*.py *.PY)");

  ext[luaFilter] = ".lua";
  ext[pyFilter] = ".py";

  QString file = QFileDialog::getSaveFileName(this, tr("Save Script File"), "", luaFilter + ";;" + pyFilter, &selFilter);

  if(!file.isEmpty())
  {
    QFileInfo info(file);

    if(info.suffix().isEmpty())
      file.append(ext[selFilter]);

    te::ce::ScriptWidget* editor = (te::ce::ScriptWidget*)m_ui->m_codeTab->currentWidget();

    editor->save(file);

    m_ui->m_codeTab->setTabText(m_ui->m_codeTab->currentIndex(), info.fileName());
  }
}

void MainWindow::run()
{
  te::ce::ScriptWidget* editor = (te::ce::ScriptWidget*)m_ui->m_codeTab->currentWidget();

  m_ui->m_txt->clear();

  editor->execute();
}

void MainWindow::codeChanged()
{
  QString title = m_ui->m_codeTab->tabText(m_ui->m_codeTab->currentIndex());
  te::ce::ScriptWidget* editor = (te::ce::ScriptWidget*)m_ui->m_codeTab->currentWidget();

  if(editor->hasChanged())
    title.append(" *");
  else
    title.remove(" *");

  m_ui->m_codeTab->setTabText(m_ui->m_codeTab->currentIndex(), title);
}
