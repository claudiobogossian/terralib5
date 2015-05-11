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
  \file terralib/qt/widgets/help/AssistantHelpManagerImpl.cpp

  \brief An implementation of HelpManager that uses QAssistant to show help files.
*/

// TerraLib
#include "AssistantHelpManagerImpl.h"

// STL
#include <cassert>

// Qt
#include <QApplication>
#include <QDir>
#include <QFileInfo>
#include <QLibraryInfo>
#include <QMainWindow>
#include <QMessageBox>
#include <QProcess>

QWidget* GetMainWindow()
{
  QWidgetList lst = qApp->topLevelWidgets();
  QWidgetList::iterator it;

  for(it=lst.begin(); it!=lst.end(); ++it)
  {
    QMainWindow* main = qobject_cast<QMainWindow*>(*it);
    
    if(main != 0)
      return main;
  }

  return 0;
}


te::qt::widgets::AssistantHelpManagerImpl::AssistantHelpManagerImpl(const QString& collectionFile, QObject* parent)
  : QObject(parent),
    m_proc(0),
    m_collectionFile(collectionFile)
{
}

te::qt::widgets::AssistantHelpManagerImpl::~AssistantHelpManagerImpl()
{
  if(m_proc != 0)
  {
    QByteArray ba;

    bool tag = m_regDocs.size() > 1;

    for(int i = 0; i < m_regDocs.size();i++)
    {
      ba.append("unregister " + m_regDocs.value(i));

      if(tag)
        ba.append((i == (m_regDocs.size()-1)) ? "\n" : ";"); 
      else
        ba.append('\n');
    }

    m_proc->write(ba);

    m_proc->close();

    delete m_proc;
  }
}

bool te::qt::widgets::AssistantHelpManagerImpl::startAssistant()
{
  QFileInfo info(m_collectionFile);

  if(!info.exists())
  {
    QMessageBox::warning(GetMainWindow(), QObject::tr("Help failure"), QObject::tr("Could not find help files"));
    return false;
  }

  if (!m_proc)
    m_proc = new QProcess();

  if (m_proc->state() != QProcess::Running) 
  {
    QString app = QLibraryInfo::location(QLibraryInfo::BinariesPath) + QDir::separator();
 #if !defined(Q_OS_MAC)
    app += QLatin1String("assistant");
 #else
    app += QLatin1String("Assistant.app/Contents/MacOS/Assistant");
 #endif

    QStringList args;
    args << QLatin1String("-collectionFile")
        << m_collectionFile
        << QLatin1String("-enableRemoteControl");

    m_proc->start(app, args);

    if (!m_proc->waitForStarted()) 
    {
      QMessageBox::critical(GetMainWindow(), QObject::tr("Simple Text Viewer"), QObject::tr("Unable to launch Qt Assistant (%1)").arg(app));
      return false;
    }
  }

  return true;
}

void te::qt::widgets::AssistantHelpManagerImpl::showHelp(const QString& htmRef, const QString& nSpace)
{
  if (!startAssistant())
    return;

  QString nS = (nSpace.isEmpty()) ? "dpi.inpe.br.qtwidgets" : nSpace;

  QByteArray ba;
  ba.append("setSource qthelp://" + nS + "/doc/" + htmRef.toLocal8Bit() + '\n');

  m_proc->write(ba);
}

void te::qt::widgets::AssistantHelpManagerImpl::appendDoc(const QString& docRef)
{
  if(m_regDocs.contains(docRef))
    return;

  m_regDocs.append(docRef);

  QString app;
  QStringList args;
  QByteArray ba;

  if(m_proc == 0)
    m_proc = new QProcess;

  ba.append(QLatin1String("register ") + docRef.toLocal8Bit() + '\n');
  m_proc->write(ba);
}

