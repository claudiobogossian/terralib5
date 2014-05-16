#include "../ApplicationController.h"
#include "../events/TableEvents.h"

#include "ui_TableWidgetForm.h"
#include "TableWidget.h"

// Qt
#include <QtCore/QSettings>
#include <QtGui/QColor>
#include <QtGui/QColorDialog>

QString GetRGBString(QColor color)
{
  QString str("rgb(");
  str += QString::number(color.red());
  str += ", " + QString::number(color.green());
  str += ", " + QString::number(color.blue());
  str += ")";

  return str;
}

QString GetStyleSheetFromColors(QColor primaryColor, QColor secondaryColor)
{
  QString sty("alternate-background-color: ");
  sty += GetRGBString(secondaryColor);
  sty += ";background-color: " + GetRGBString(primaryColor) + ";";

  return sty;
}

te::qt::af::TableWidget::TableWidget(QWidget* parent)
  : AbstractSettingWidget(parent),
    m_ui(new Ui::TableWidgetForm)
{
  m_ui->setupUi(this);

  m_resumeText = tr("");

  m_primaryColor = Qt::white;
  m_secondaryColor = Qt::white;

  QSettings sett(QSettings::IniFormat, QSettings::UserScope, qApp->organizationName(), qApp->applicationName());
  bool isChecked = sett.value("table/tableAlternateColors").toBool();
  QColor pColor;
  pColor.setNamedColor(sett.value("table/primaryColor").toString());
  QColor sColor;
  sColor.setNamedColor(sett.value("table/secondaryColor").toString());

  m_ui->m_alternateRowColorsGroupBox->setChecked(isChecked);

  m_ui->m_previewListWidget->setAlternatingRowColors(isChecked);

  if(pColor.isValid())
    m_primaryColor = pColor;
  if(sColor.isValid())
    m_secondaryColor = sColor;

  if(isChecked)
    m_ui->m_previewListWidget->setStyleSheet(GetStyleSheetFromColors(m_primaryColor, m_secondaryColor));

  connect(m_ui->m_primaryPushButton, SIGNAL(clicked()), SLOT(onPrimaryPushButtonClicked()));
  connect(m_ui->m_secondaryPushButton, SIGNAL(clicked()), SLOT(onSecondaryPushButtonClicked()));
  connect(m_ui->m_alternateRowColorsGroupBox, SIGNAL(clicked()), SLOT(onAlternateRowColorsGroupBoxClicked()));
}

te::qt::af::TableWidget::~TableWidget()
{
  delete m_ui;
}

void te::qt::af::TableWidget::getHelpInformations(QString& ns, QString& helpFile)
{
  ns = "dpi.inpe.br.apf";
  helpFile = "apf/settings/table/TableConfig.html";
}

void te::qt::af::TableWidget::onApplyButtonClicked()
{
  saveChanges();
}

void te::qt::af::TableWidget::saveChanges()
{
  bool isChecked = m_ui->m_alternateRowColorsGroupBox->isChecked();

  QSettings sett(QSettings::IniFormat, QSettings::UserScope, qApp->organizationName(), qApp->applicationName());
  sett.setValue("table/tableAlternateColors", isChecked);
  sett.setValue("table/primaryColor", m_primaryColor.name());
  sett.setValue("table/secondaryColor", m_secondaryColor.name());

  te::qt::af::evt::TableAlternatingColorsChanged tableAlternatingColorsChanged(isChecked, m_primaryColor, m_secondaryColor);
  te::qt::af::ApplicationController::getInstance().broadcast(&tableAlternatingColorsChanged);

  changeApplyButtonState(false);
}

void te::qt::af::TableWidget::resetState()
{
  
}

void te::qt::af::TableWidget::onAlternateRowColorsGroupBoxClicked()
{
  m_ui->m_previewListWidget->setAlternatingRowColors(m_ui->m_alternateRowColorsGroupBox->isChecked());
  
  if(m_ui->m_alternateRowColorsGroupBox->isChecked())
  {
    m_primaryColor = Qt::white;
    m_secondaryColor.setNamedColor("#ebebeb");

    m_ui->m_previewListWidget->setStyleSheet(GetStyleSheetFromColors(m_primaryColor, m_secondaryColor));
  }

  changeApplyButtonState(true);
}

void te::qt::af::TableWidget::onPrimaryPushButtonClicked()
{
  m_primaryColor = QColorDialog::getColor(m_primaryColor, this);

  m_ui->m_previewListWidget->setStyleSheet(GetStyleSheetFromColors(m_primaryColor, m_secondaryColor));
  changeApplyButtonState(true);
}

void te::qt::af::TableWidget::onSecondaryPushButtonClicked()
{
  m_secondaryColor = QColorDialog::getColor(m_secondaryColor, this);

  m_ui->m_previewListWidget->setStyleSheet(GetStyleSheetFromColors(m_primaryColor, m_secondaryColor));

  changeApplyButtonState(true);
}