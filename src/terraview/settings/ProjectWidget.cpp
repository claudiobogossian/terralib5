//TerraLib Includes
#include "../Utils.h"
#include "ui_ProjectWidgetForm.h"
#include "ProjectWidget.h"


#include <terralib/qt/af/Utils.h>

ProjectWidget::ProjectWidget(QWidget* parent)
  : AbstractSettingWidget(parent),
    m_ui(new Ui::ProjectWidgetForm)
{
  m_ui->setupUi(this);

  m_resumeText = tr("Default author for new projects.");

  connect(m_ui->m_authorLineEdit, SIGNAL(textChanged(const QString&)), SLOT(authorChanged(const QString&)));
  connect(m_ui->m_numRecentProjectsSpinBox, SIGNAL(valueChanged(int)), SLOT(maxSavedChanged(int)));
  connect(m_ui->m_lastProjectRadioButton, SIGNAL(clicked()), SLOT(onOpeningOptionChanged()));
  connect(m_ui->m_noneProjectRadioButton, SIGNAL(clicked()), SLOT(onOpeningOptionChanged()));

  resetState();
}

void ProjectWidget::getHelpInformations(QString& ns, QString& helpFile)
{
  ns = "dpi.inpe.br.apf";
  helpFile = "apf/settings/project/ProjectConfig.html";
}

ProjectWidget::~ProjectWidget()
{
  delete m_ui;
}

void ProjectWidget::saveChanges()
{
  QString author = m_ui->m_authorLineEdit->text();
  int maxSaved = m_ui->m_numRecentProjectsSpinBox->value();

  SaveProjectInformationsOnSettings(author, maxSaved);

  SaveOpenLastProjectOnSettings(m_ui->m_lastProjectRadioButton->isChecked());

  changeApplyButtonState(false);
}

void ProjectWidget::resetState()
{
  QString author;
  int maxSaved;

  GetProjectInformationsFromSettings(author, maxSaved);

  m_ui->m_authorLineEdit->setText(author);
  m_ui->m_numRecentProjectsSpinBox->setValue(maxSaved);

  changeApplyButtonState(false);
}

void ProjectWidget::authorChanged(const QString& text)
{
  changeApplyButtonState(true);
}

void ProjectWidget::maxSavedChanged(int i)
{
  changeApplyButtonState(true);
}

void ProjectWidget::onOpeningOptionChanged()
{
  changeApplyButtonState(true);
}