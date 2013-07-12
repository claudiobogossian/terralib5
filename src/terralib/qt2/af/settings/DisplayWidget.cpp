#include "ui_DisplayWidgetForm.h"
#include "DisplayWidget.h"

te::qt::af::DisplayWidget::DisplayWidget(QWidget* parent) :
AbstractSettingWidget(parent),
m_ui(new Ui::DisplayWidgetForm)
{
  m_ui->setupUi(this);

  m_resumeText = tr("Configurations of the display");
}

te::qt::af::DisplayWidget::~DisplayWidget()
{
  delete m_ui;
}

void te::qt::af::DisplayWidget::getHelpInformations(QString& ns, QString& helpFile)
{
  ns = "dpi.inpe.br.apf";
  helpFile = "/apf/settings/display/DisplayConfig.html";
}
