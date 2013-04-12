#include "ui_DisplayWidgetForm.h"
#include "DisplayWidget.h"

te::qt::af::DisplayWidget::DisplayWidget(QWidget* parent) :
AbstractSettingWidget(parent),
m_ui(new Ui::DisplayWidgetForm)
{
  m_ui->setupUi(this);

  m_resumeText + tr("Show symbol of north on display.");
}

te::qt::af::DisplayWidget::~DisplayWidget()
{
  delete m_ui;
}