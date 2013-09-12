#include "../ApplicationController.h"
#include "../events/TableEvents.h"

#include "ui_TableWidgetForm.h"
#include "TableWidget.h"

// Qt
#include <QtCore/QSettings>

te::qt::af::TableWidget::TableWidget(QWidget* parent)
  : AbstractSettingWidget(parent),
    m_ui(new Ui::TableWidgetForm)
{
  m_ui->setupUi(this);

  m_resumeText = tr("");

  QSettings sett(QSettings::IniFormat, QSettings::UserScope, qApp->organizationName(), qApp->applicationName());
  bool isChecked = sett.value("projects/tableAlternateColors").toBool();

  m_ui->m_alternateColorsCheckBox->setChecked(isChecked);

  connect(m_ui->m_alternateColorsCheckBox, SIGNAL(clicked()), SLOT(onAlternateColorsCheckBoxClicked()));
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

void te::qt::af::TableWidget::onApplyButtonClicked()
{
  saveChanges();
}

void te::qt::af::TableWidget::saveChanges()
{
  bool isChecked = false;
  m_ui->m_alternateColorsCheckBox->isChecked() ? isChecked = true : isChecked = false;
  
  QSettings sett(QSettings::IniFormat, QSettings::UserScope, qApp->organizationName(), qApp->applicationName());
  sett.setValue("projects/tableAlternateColors", isChecked);

  te::qt::af::evt::TableAlternatingColorsChanged tableAlternatingColorsChanged(isChecked);

  te::qt::af::ApplicationController::getInstance().broadcast(&tableAlternatingColorsChanged);

  changeApplyButtonState(false);
}

void te::qt::af::TableWidget::resetState()
{
  
}

void te::qt::af::TableWidget::onAlternateColorsCheckBoxClicked()
{
  changeApplyButtonState(true);
}
