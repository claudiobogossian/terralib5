#include "HelpTest.h"

//TerraLib help files
#include <terralib/qt/widgets/help/HelpPushButton.h>

//Qt 
#include <QtGui/QGridLayout>

HelpTest::HelpTest(QWidget* parent) :
QWidget(parent, Qt::Dialog)
{
  m_help_btn = new te::qt::widgets::HelpPushButton(this);
  m_help_btn->setPageReference("qthelp://dpi.inpe.br.terraview.5.0/doc/associando_arquivos_sites.htm");

  m_help_btn->setText("Associando arquivos");

  createGUI();
}

HelpTest::~HelpTest()
{
}

void HelpTest::createGUI()
{
  QGridLayout* l = new QGridLayout(this);

  l->addWidget(m_help_btn);
}
