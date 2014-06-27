#include "HelpTest.h"

//TerraLib help files
#include <terralib/qt/widgets/help/HelpPushButton.h>

//Qt 
#include <QGridLayout>

HelpTest::HelpTest(QWidget* parent) :
QWidget(parent, Qt::Dialog)
{
  m_help_btn = new te::qt::widgets::HelpPushButton(this);
  m_help_btn->setPageReference("qthelp://dpi.inpe.br.help_example/doc/teste1.htm");

//  m_help_btn->setText("Teste 1");

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
