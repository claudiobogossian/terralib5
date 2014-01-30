#include <ui_SettingsDialogForm.h>

#include "AbstractSettingWidget.h"
#include "SettingsDialog.h"
#include "DisplayWidgetFactory.h"
#include "ProjectWidgetFactory.h"
#include "TableWidgetFactory.h"
#include "ToolbarsWidgetFactory.h"
#include "GeneralConfigWidgetFactory.h"

// Qt
#include <QtGui/QScrollArea>

void GetFactoriesNames (QVector<QString>& facNames)
{
  std::vector<std::string> vec;
  const te::qt::af::SettingsWidgetsFactory::dictionary_type& d = te::qt::af::SettingsWidgetsFactory::getDictionary();
  te::qt::af::SettingsWidgetsFactory::dictionary_type::const_iterator it;

  for(it = d.begin(); it != d.end(); ++it)
    facNames.append(it->first.c_str());
}

QWidget* GetWidget(const QString& facName, QWidget* parent)
{
  QScrollArea* scr = new QScrollArea(parent);
  std::string value = facName.toStdString();

  QWidget* wid = te::qt::af::SettingsWidgetsFactory::make(value, scr);

  QGridLayout* lay = new QGridLayout(scr);

  scr->setWidget(wid);
  lay->addWidget(wid);

  scr->setFrameShape(QFrame::NoFrame);

  return scr;
}

te::qt::af::SettingsDialog::SettingsDialog(QWidget* parent) :
QDialog(parent),
m_ui(new Ui::SettingsDialogForm)
{
  m_ui->setupUi(this);

  DisplayWidgetFactory::initialize();
  TableWidgetFactory::initialize();
  ProjectWidgetFactory::initialize();
  ToolbarsWidgetFactory::initialize();
  GeneralConfigWidgetFactory::initialize();

  QVector<QString> facNames;
  QVector<QString>::Iterator it;

  GetFactoriesNames(facNames);

  for(it = facNames.begin(); it != facNames.end(); ++it)
    m_ui->m_settingsListView->addItem(*it);

  // Signal/Slots connections
  connect (m_ui->m_closePushButton, SIGNAL(pressed()), SLOT(close()));
  connect (m_ui->m_settingsListView, SIGNAL(currentItemChanged(QListWidgetItem*, QListWidgetItem*)), SLOT(settingsChanged (QListWidgetItem*, QListWidgetItem*)));

  m_ui->m_settingsListView->setCurrentRow(0);
}

te::qt::af::SettingsDialog::~SettingsDialog()
{
  DisplayWidgetFactory::finalize();
  TableWidgetFactory::finalize();
  ProjectWidgetFactory::finalize();
  ToolbarsWidgetFactory::finalize();
  GeneralConfigWidgetFactory::finalize();

  delete m_ui;
}

void te::qt::af::SettingsDialog::settingsChanged (QListWidgetItem* current, QListWidgetItem* previous)
{
  QString sett = current->text();
  int pos = -1;

  if(m_widPos.find(sett) == m_widPos.end())
  {
    QWidget* wid = GetWidget(sett, this);
    pos =  m_ui->m_widgetStack->addWidget(wid);

    m_widPos[sett] = pos;
  }
  else
    pos = m_widPos[sett];

  if(pos == -1)
    throw;

  if(pos != m_ui->m_widgetStack->currentIndex())
  {
    // Disconnecting old
    QWidget* w = ((QScrollArea*)m_ui->m_widgetStack->currentWidget())->widget();

    if(!w->disconnect(SIGNAL(updateApplyButtonState(const bool&)), this, SLOT(updateApplyButtonState(const bool&))))
      throw;

    if(!m_ui->m_applyPushButton->disconnect(SIGNAL(clicked()), w, SLOT(onApplyButtonClicked())))
      throw;
  }

  // Connecting new
  QWidget* w = ((QScrollArea*)m_ui->m_widgetStack->widget(pos))->widget();
  connect(w, SIGNAL(updateApplyButtonState(const bool&)), SLOT(updateApplyButtonState(const bool&)));
  w->connect(m_ui->m_applyPushButton, SIGNAL(clicked()), SLOT(onApplyButtonClicked()));

  // Update dialog
  m_ui->m_widgetStack->setCurrentIndex(pos);
  m_ui->m_customGroupBox->setTitle(sett + tr(" settings"));

  AbstractSettingWidget* w2 = ((AbstractSettingWidget*)w);

  QString lbl = w2->getResumeText(),
          ns,
          page;

  w2->getHelpInformations(ns, page);

  m_ui->m_helpPushButton->setNameSpace(ns);
  m_ui->m_helpPushButton->setPageReference(page);

  m_ui->m_resumeLabel->setText(lbl);

  m_ui->m_applyPushButton->setEnabled(false);
}

void te::qt::af::SettingsDialog::updateApplyButtonState(const bool& state)
{
  m_ui->m_applyPushButton->setEnabled(state);
}
