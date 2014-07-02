/*  Copyright (C) 2011-2013 National Institute For Space Research (INPE) - Brazil.

    This file is part of the TerraLib - a Framework for building GIS enabled applications.

    TerraLib is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation, either version 3 of the License,
    or (at your option) any later version.

    TerraLib is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with TerraLib. See COPYING. If not, write to
    TerraLib Team at <terralib-team@terralib.org>.
 */

/*!
  \file GeneralConfigWidget.cpp

  \brief A frame for setting general options.
*/

// TerraLib
#include "../../../common/PlatformUtils.h"
#include "../../../common/SystemApplicationSettings.h"
#include "../../../common/UserApplicationSettings.h"
#include "../../../qt/widgets/srs/SRSManagerDialog.h"
#include "../../../qt/widgets/utils/ColorPickerToolButton.h"
#include "../../../srs/Config.h"
#include "../ApplicationController.h"
#include "../Utils.h"
#include "GeneralConfigWidget.h"
#include "ui_GeneralConfigWidgetForm.h"

// Qt
#include <QDir>
#include <QPushButton>
#include <QString>

te::qt::af::GeneralConfigWidget::GeneralConfigWidget(QWidget* parent)
  : AbstractSettingWidget(parent),
    m_ui(new Ui::GeneralConfigWidgetForm),
    m_defaultSRID(TE_UNKNOWN_SRS),
    m_needRestart(false)
{
  m_resumeText = tr("Changes the general configurations of the application.");

  m_ui->setupUi(this);

  initialize();
}

te::qt::af::GeneralConfigWidget::~GeneralConfigWidget()
{
  if(m_needRestart)
  {
    QMessageBox msgBox(this);

    msgBox.setText(tr("The system must be restarted for the changes to take effect."));
    msgBox.setInformativeText(tr("Do you want to do it now?"));
    msgBox.setWindowTitle(tr("Restart system"));

    std::auto_ptr<QPushButton> restartButton;
    restartButton.reset(msgBox.addButton(tr("Restart now"), QMessageBox::ActionRole));

    std::auto_ptr<QPushButton> laterResButton;
    laterResButton.reset(msgBox.addButton(tr("Restart later"), QMessageBox::DestructiveRole));
    msgBox.addButton(QMessageBox::NoButton);

    msgBox.setDefaultButton(restartButton.get());

    msgBox.exec();

    if(msgBox.clickedButton() == restartButton.get())
      qApp->exit(1000);
  }
}

void te::qt::af::GeneralConfigWidget::saveChanges()
{
  QSettings sett(QSettings::IniFormat, QSettings::UserScope, qApp->organizationName(), qApp->applicationName());

  sett.setValue("srs/default_srid", m_defaultSRID);
  sett.setValue("color/selection_color", m_colorPicker->getColor().name());
  sett.setValue("toolbars/icon_size", m_ui->m_toolBarIconSizeSpinBox->value());

  ApplicationController::getInstance().setSelectionColor(m_colorPicker->getColor());

  changeApplyButtonState(false);
}

void te::qt::af::GeneralConfigWidget::resetState()
{
}

void te::qt::af::GeneralConfigWidget::getHelpInformations(QString& ns, QString& helpFile)
{
  ns = "dpi.inpe.br.apf";
  helpFile = "apf/settings/generalconfig/GeneralConfig.html";
}

void te::qt::af::GeneralConfigWidget::initialize()
{
  // Setup UI icons
  m_ui->m_defaultSRSToolButton->setIcon(QIcon::fromTheme("srs"));

  // Create Color Picker to selection color
  m_colorPicker = new te::qt::widgets::ColorPickerToolButton(this);
  m_colorPicker->setFixedSize(70, 24);

  // Adjusting color picker...
  QGridLayout* layout = new QGridLayout(m_ui->m_selectionColorFrame);
  layout->setContentsMargins(0, 0, 0, 0);
  layout->setSizeConstraint(QLayout::SetFixedSize);
  layout->addWidget(m_colorPicker);

  // Get user settings
  QSettings settings(QSettings::IniFormat, QSettings::UserScope, QApplication::instance()->organizationName(), QApplication::instance()->applicationName());

  // SRID
  m_defaultSRID = settings.value("srs/default_srid", te::common::SystemApplicationSettings::getInstance().getValue("Application.DefaultSRID").c_str()).toInt();
  setupSRSUi();

  // Selection Color
  QString selectionColorName = settings.value("color/selection_color", te::common::SystemApplicationSettings::getInstance().getValue("Application.DefaultSelectionColor").c_str()).toString();
  QColor selectionColor = QColor(selectionColorName);
  m_colorPicker->setColor(selectionColor);

  // ToolBar Icon Size
  int iconSize = settings.value("toolbars/icon_size", te::common::SystemApplicationSettings::getInstance().getValue("Application.ToolBarDefaultIconSize").c_str()).toInt();
  m_ui->m_toolBarIconSizeSpinBox->setValue(iconSize);

  // Icon Theme
  std::string iconThemesDirPath = te::common::FindInTerraLibPath(te::common::SystemApplicationSettings::getInstance().getValue("Application.IconThemeInfo.BaseDirectory.<xmlattr>.xlink:href"));
  QDir iconThemesDir(iconThemesDirPath.c_str());

  // Find themes
  QStringList themes = iconThemesDir.entryList(QDir::Dirs | QDir::NoDotAndDotDot);
  m_ui->m_iconThemeComboBox->clear();
  m_ui->m_iconThemeComboBox->addItems(themes);

  // Signals & slots
  connect(m_ui->m_defaultSRSToolButton, SIGNAL(pressed()), SLOT(onDefaultSRSToolButtonPressed()));
  connect(m_colorPicker, SIGNAL(colorChanged(const QColor&)), SLOT(onSelectionColorChanged(const QColor&)));
  connect(m_ui->m_toolBarIconSizeSpinBox, SIGNAL(valueChanged(int)), SLOT(onToolBarIconSizeValueChanged(int)));
}

void te::qt::af::GeneralConfigWidget::setupSRSUi()
{
  m_defaultSRID != TE_UNKNOWN_SRS ? m_ui->m_defaultSRSLineEdit->setText("EPSG:" + QString::number(m_defaultSRID)) : m_ui->m_defaultSRSLineEdit->setText(tr("Unknown SRS"));
}

void te::qt::af::GeneralConfigWidget::onDefaultSRSToolButtonPressed()
{
  te::qt::widgets::SRSManagerDialog srsDialog(this);
  srsDialog.setWindowTitle(tr("Choose the Default SRS"));

  if(srsDialog.exec() == QDialog::Rejected)
    return;

  m_defaultSRID = srsDialog.getSelectedSRS().first;

  setupSRSUi();

  changeApplyButtonState(true);
}

void te::qt::af::GeneralConfigWidget::onSelectionColorChanged(const QColor& /*color*/)
{
  changeApplyButtonState(true);
}

void te::qt::af::GeneralConfigWidget::onToolBarIconSizeValueChanged(int value)
{
  m_needRestart = true;
  changeApplyButtonState(true);
}
