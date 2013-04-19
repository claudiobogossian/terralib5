#include "ui_ProjectWidgetForm.h"
#include "ProjectWidget.h"

#include "../Utils.h"

te::qt::af::ProjectWidget::ProjectWidget(QWidget* parent)
  : AbstractSettingWidget(parent),
    m_ui(new Ui::ProjectWidgetForm)
{
  m_ui->setupUi(this);

  m_resumeText = tr("Default author for new projects.");

  connect(m_ui->m_authorLineEdit, SIGNAL(textChanged(const QString&)), SLOT(authorChanged(const QString&)));
  connect(m_ui->m_numSavedProjectsSpinBox, SIGNAL(valueChanged(int)), SLOT(maxSavedChanged()));

  resetState();
}

te::qt::af::ProjectWidget::~ProjectWidget()
{
  delete m_ui;
}

void te::qt::af::ProjectWidget::saveChanges()
{
  QString author = m_ui->m_authorLineEdit->text();
  int maxSaved = m_ui->m_numSavedProjectsSpinBox->value();

  SaveProjectInformationsOnSettings(author, maxSaved);

  changeApplyButtonState(false);
}

void te::qt::af::ProjectWidget::resetState()
{
  QString author;
  int maxSaved;

  GetProjectInformationsFromSettings(author, maxSaved);

  m_ui->m_authorLineEdit->setText(author);
  m_ui->m_numSavedProjectsSpinBox->setValue(maxSaved);

  changeApplyButtonState(false);
}

void te::qt::af::ProjectWidget::authorChanged(const QString& text)
{
  changeApplyButtonState(true);
}

void te::qt::af::ProjectWidget::maxSavedChanged(int i)
{
  changeApplyButtonState(true);
}