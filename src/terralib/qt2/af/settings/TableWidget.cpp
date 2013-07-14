#include "ui_TableWidgetForm.h"
#include "TableWidget.h"

te::qt::af::TableWidget::TableWidget(QWidget* parent)
  : AbstractSettingWidget(parent),
    m_ui(new Ui::TableWidgetForm)
{
  m_ui->setupUi(this);

  m_resumeText = tr("Maintain data table always visible");
}

te::qt::af::TableWidget::~TableWidget()
{
  delete m_ui;
}

void te::qt::af::TableWidget::getHelpInformations(QString& ns, QString& helpFile)
{
  ns = "dpi.inpe.br.apf";
  helpFile = "/apf/settings/table/TableConfig.html";
}
