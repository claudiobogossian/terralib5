#include "Plugin.h"

//TerraLib help files
#include <terralib/qt/widgets/help/HelpManager.h>
#include <terralib/qt/widgets/help/HelpManagerImpl.h>
#include <terralib/qt/widgets/help/HelpPushButton.h>

//Qt 
#include <QGridLayout>

Plugin::Plugin(QWidget* parent) :
QWidget(parent, Qt::Dialog)
{
  m_help_btn = new te::qt::widgets::HelpPushButton(this);
  m_help_btn->setPageReference("qthelp://dpi.inpe.br.help_example_plugin/doc/teste3.htm");

//  m_help_btn->setText("Plug-in test");

  te::qt::widgets::HelpManager::getInstance().appendDoc("docplugin.qch");
}

Plugin::~Plugin()
{
}

void Plugin::createGUI()
{
  QGridLayout* l = new QGridLayout(this);

  l->addWidget(m_help_btn);
}
