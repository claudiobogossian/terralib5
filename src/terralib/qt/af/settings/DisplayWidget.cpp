#include "../ApplicationController.h"
#include "../events/MapEvents.h"
#include "../Utils.h"

#include "ui_DisplayWidgetForm.h"
#include "DisplayWidget.h"

// Qt
#include <QtGui/QColor>
#include <QtGui/QColorDialog>
#include <QtGui/QFrame>
#include <QtGui/QPalette>

// Boost
#include <boost/lexical_cast.hpp>

te::qt::af::DisplayWidget::DisplayWidget(QWidget* parent) :
AbstractSettingWidget(parent),
m_ui(new Ui::DisplayWidgetForm)
{
  m_ui->setupUi(this);

  m_resumeText = tr("Configurations of the display");

  QSettings sett(QSettings::IniFormat, QSettings::UserScope, qApp->organizationName(), qApp->applicationName());
  QString hexColor = sett.value("display/defaultDisplayColor").toString();
  QColor dColor;
  dColor.setNamedColor(hexColor);

  QPalette palette = m_ui->m_backColor->palette();

  if(!dColor.isValid())
  {
    palette.setColor(QPalette::Window, Qt::white);
  }
  else
  {
    palette.setColor(QPalette::Window, dColor);
  }

  m_ui->m_backColor->setPalette(palette);
  m_ui->m_backColor->setAutoFillBackground(true);

  connect(m_ui->m_changeBackgroundPushButton, SIGNAL(clicked()), SLOT(onChangeBackgroundPushButton()));
}

te::qt::af::DisplayWidget::~DisplayWidget()
{
  delete m_ui;
}

void te::qt::af::DisplayWidget::onChangeBackgroundPushButton()
{
  QColor color = QColorDialog::getColor(m_ui->m_backColor->palette().color(QPalette::Window), this);

  if(!color.isValid())
    return;

  QPalette palette = m_ui->m_backColor->palette();
  palette.setColor(QPalette::Window, color);
  m_ui->m_backColor->setPalette(palette);
  m_ui->m_backColor->setAutoFillBackground(true);

  changeApplyButtonState(true);
}

void te::qt::af::DisplayWidget::saveChanges()
{
  QSettings sett(QSettings::IniFormat, QSettings::UserScope, qApp->organizationName(), qApp->applicationName());

  sett.setValue("display/defaultDisplayColor", m_ui->m_backColor->palette().color(QPalette::Window).name());

  te::qt::af::evt::MapColorChanged mapColorChanged(m_ui->m_backColor->palette().color(QPalette::Window));

  te::qt::af::ApplicationController::getInstance().broadcast(&mapColorChanged);

  changeApplyButtonState(false);
}

void te::qt::af::DisplayWidget::resetState()
{
  QSettings sett(QSettings::IniFormat, QSettings::UserScope, qApp->organizationName(), qApp->applicationName());
  QString hexColor = sett.value("display/defaultDisplayColor").toString();
  QColor dColor;
  dColor.setNamedColor(hexColor);

  QPalette palette = m_ui->m_backColor->palette();

  if(!dColor.isValid())
  {
    palette.setColor(QPalette::Window, Qt::white);
  }
  else
  {
    palette.setColor(QPalette::Window, dColor);
  }

  m_ui->m_backColor->setPalette(palette);
  m_ui->m_backColor->setAutoFillBackground(true);
}

void te::qt::af::DisplayWidget::onApplyButtonClicked()
{
  saveChanges();
}

void te::qt::af::DisplayWidget::getHelpInformations(QString& ns, QString& helpFile)
{
  ns = "dpi.inpe.br.apf";
  helpFile = "apf/settings/display/DisplayConfig.html";
}
