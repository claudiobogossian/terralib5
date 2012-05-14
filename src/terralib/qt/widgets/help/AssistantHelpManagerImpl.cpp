#include "AssistantHelpManagerImpl.h"

//Qt
#include <QDir>
#include <QProcess>
#include <QLibraryInfo>
#include <QMessageBox>

#include <iostream>

void initAssistant(QProcess* proc)
{
  QStringList args;
  QString app;

#if !defined(Q_OS_MAC)
  app += QLatin1String("assistant");
#else
  app += QLatin1String("Assistant.app/Contents/MacOS/Assistant");    
#endif
  args << QLatin1String("-collectionFile")
    << QLatin1String("terraview.qhc")
    << QLatin1String("-enableRemoteControl");

  proc->start(app, args);

  if (!proc->waitForStarted())
    QMessageBox::critical(0, QObject::tr("Remote Control"), QObject::tr("Could not start Qt Assistant from local \"%1\".").arg(app));
}


te::qt::widgets::AssistantHelpManagerImpl::AssistantHelpManagerImpl(QObject* parent) :
QObject(parent),
  m_proc(0)
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
  }

  delete m_proc;
}

void te::qt::widgets::AssistantHelpManagerImpl::showHelp (const QString& htmRef)
{
  if(m_proc == 0)
    m_proc = new QProcess;

  QStringList args;
  QString app;
  QByteArray ba;
  QStringList::iterator it;

  switch(m_proc->state())
  {
  case QProcess::NotRunning:
    initAssistant(m_proc);

    ba.append("setSource " + htmRef.toLocal8Bit() + "\n");
    m_proc->write(ba);

    break;

  case QProcess::Running:
    ba.append("setSource " + htmRef + ";");
    ba.append("syncContents\n");

    m_proc->write(ba);
    break;

  default:
    break;
  }
}

void te::qt::widgets::AssistantHelpManagerImpl::appendDoc(const QString& docRef)
{
  if(m_regDocs.contains(docRef))
    return;

  m_regDocs.append(docRef);

  QString app;
  QStringList args;
  QByteArray ba;
  int r;

  if ((m_proc == 0) || m_proc->state() == QProcess::NotRunning)
  {
  #if !defined(Q_OS_MAC)
    app += QLatin1String("assistant");
  #else
    app += QLatin1String("Assistant.app/Contents/MacOS/Assistant");    
  #endif
    args << QLatin1String("-collectionFile")
      << QLatin1String("terraview.qhc");

    args << QLatin1String("-register")
      << docRef
      << QLatin1String("-quiet");

    r = QProcess::execute(app, args);
  }
  else if(m_proc->state() == QProcess::Running)
  {
    ba.append(QLatin1String("register ") + docRef.toLocal8Bit() + '\n');
    m_proc->write(ba);
  }
}