#include "HelpTest2.h"

//TerraLib help files
#include <terralib/qt/widgets/help/HelpPushButton.h>

//Qt 
#include <QtGui/QGridLayout>

HelpTest2::HelpTest2(QWidget* parent) :
QWidget(parent, Qt::Dialog)
{
  m_help_btn = new te::qt::widgets::HelpPushButton(this);
  m_help_btn->setPageReference("qthelp://dpi.inpe.br.terraview.5.0/doc/animation.htm");

  m_help_btn->setText("Animação");

  createGUI();
}

HelpTest2::~HelpTest2()
{
}

void HelpTest2::createGUI()
{
  QGridLayout* l = new QGridLayout(this);

  l->addWidget(m_help_btn);
}
