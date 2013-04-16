#include "ui_ProjectWidgetForm.h"
#include "ProjectWidget.h"

te::qt::af::ProjectWidget::ProjectWidget(QWidget* parent)
  : AbstractSettingWidget(parent),
    m_ui(new Ui::ProjectWidgetForm)
{
  m_ui->setupUi(this);

  m_resumeText = tr("Default author for new projects.");
}

te::qt::af::ProjectWidget::~ProjectWidget()
{
  delete m_ui;
}