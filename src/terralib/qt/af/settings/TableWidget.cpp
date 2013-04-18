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