/*  Copyright (C) 2011-2012 National Institute For Space Research (INPE) - Brazil.

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
  \file terralib/vp/qt/BasicGeopraphicOpWizardPage.cpp

  \brief This file defines a class for a Basic Geographic Operation Wizard Page.
*/

// TerraLib
#include "../../common/StringUtils.h"
#include "../../dataaccess/dataset/DataSet.h"
#include "../../dataaccess/dataset/DataSetType.h"
#include "../../dataaccess/utils/Utils.h"
#include "../../maptools/AbstractLayer.h"
#include "../../qt/widgets/datasource/selector/DataSourceSelectorDialog.h"
#include "../../qt/widgets/utils/DoubleListWidget.h"
#include "BasicOpOutputWizardPage.h"
#include "ui_BasicOpOutputWizardPageForm.h"
#include "VectorProcessingConfig.h"

// Qt
#include <QtGui/QFileDialog>
#include <QtGui/QIntValidator>
#include <QtGui/QMessageBox>

// BOOST
#include <boost/filesystem.hpp>

// STL
#include <memory>

te::vp::BasicOpOutputWizardPage::BasicOpOutputWizardPage(QWidget* parent)
  : QWizardPage(parent),
    m_ui(new Ui::BasicOpOutputWizardPageForm),
    m_toFile(false)
{
// setup controls
  m_ui->setupUi(this);

  // configure page
  this->setTitle(tr("Output Layer Attributes"));
  this->setSubTitle(tr("Choose the output parameters that compose the output layer."));

// icons
  QSize iconSize(32, 32);

  m_ui->m_convexHullRadioButton->setIconSize(iconSize);
  m_ui->m_convexHullRadioButton->setIcon(QIcon::fromTheme(VP_IMAGES"/vp-convex-hull-hint"));

  m_ui->m_centroidRadioButton->setIconSize(iconSize);
  m_ui->m_centroidRadioButton->setIcon(QIcon::fromTheme(VP_IMAGES"/vp-centroid-hint"));

  m_ui->m_areaCheckBox->setIconSize(iconSize);
  m_ui->m_areaCheckBox->setIcon(QIcon::fromTheme(VP_IMAGES"/vp-area-hint"));

  m_ui->m_lineCheckBox->setIconSize(iconSize);
  m_ui->m_lineCheckBox->setIcon(QIcon::fromTheme(VP_IMAGES"/vp-line-length-hint"));

  m_ui->m_perimeterCheckBox->setIconSize(iconSize);
  m_ui->m_perimeterCheckBox->setIcon(QIcon::fromTheme(VP_IMAGES"/vp-perimeter-hint"));

  m_ui->m_mbrRadioButton->setIconSize(iconSize);
  m_ui->m_mbrRadioButton->setIcon(QIcon::fromTheme(VP_IMAGES"/vp-mbr-hint"));

  m_ui->m_targetDatasourceToolButton->setIcon(QIcon::fromTheme("datasource"));

  connect(m_ui->m_attributesComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(onAttributeComboBoxChanged(int)));
  connect(m_ui->m_simpleRadioButton, SIGNAL(toggled(bool)), this, SLOT(onSimpleOperationToggled()));
  connect(m_ui->m_byAttributesRadioButton, SIGNAL(toggled(bool)), this, SLOT(onAttributeOperationToggled()));
  connect(m_ui->m_targetDatasourceToolButton, SIGNAL(pressed()), this, SLOT(onTargetDatasourceToolButtonPressed()));
  connect(m_ui->m_targetFileToolButton, SIGNAL(pressed()), this,  SLOT(onTargetFileToolButtonPressed()));

}

te::vp::BasicOpOutputWizardPage::~BasicOpOutputWizardPage()
{
}

bool te::vp::BasicOpOutputWizardPage::hasConvexHull()
{
  return m_ui->m_convexHullRadioButton->isChecked();
}

bool te::vp::BasicOpOutputWizardPage::hasCentroid()
{
  return m_ui->m_centroidRadioButton->isChecked();
}

bool te::vp::BasicOpOutputWizardPage::hasMBR()
{
  return m_ui->m_mbrRadioButton->isChecked();
}

bool te::vp::BasicOpOutputWizardPage::hasArea()
{
  return m_ui->m_areaCheckBox->isChecked();
}

bool te::vp::BasicOpOutputWizardPage::hasLine()
{
  return m_ui->m_lineCheckBox->isChecked();
}

bool te::vp::BasicOpOutputWizardPage::hasPerimeter()
{
  return m_ui->m_perimeterCheckBox->isChecked();
}

std::string te::vp::BasicOpOutputWizardPage::getAttribute()
{
  return m_attribute;
}

void te::vp::BasicOpOutputWizardPage::setAttributes(std::vector<std::string> attributes)
{
  m_ui->m_attributesComboBox->clear();

  for(std::size_t i = 0; i < attributes.size(); ++i)
  {
    m_ui->m_attributesComboBox->addItem(QString::fromStdString(attributes[i]));
  }
}

std::string te::vp::BasicOpOutputWizardPage::getOutDsName()
{
  return m_ui->m_newLayerNameLineEdit->text().toStdString();
}

bool te::vp::BasicOpOutputWizardPage::getToFile()
{
  return m_toFile;
}

te::da::DataSourceInfoPtr te::vp::BasicOpOutputWizardPage::getDsInfoPtr()
{
  return m_outputDatasource;
}

std::string te::vp::BasicOpOutputWizardPage::getPath()
{
  return m_ui->m_repositoryLineEdit->text().toStdString();
}

void te::vp::BasicOpOutputWizardPage::onAttributeComboBoxChanged(int index)
{
  if(m_ui->m_byAttributesRadioButton->isChecked())
    m_attribute = m_ui->m_attributesComboBox->itemText(index).toStdString();
  else
    m_attribute = "";
}

void te::vp::BasicOpOutputWizardPage::onSimpleOperationToggled()
{
  m_ui->m_attributesComboBox->setEnabled(false);
  onAttributeComboBoxChanged(0);
}

void te::vp::BasicOpOutputWizardPage::onAttributeOperationToggled()
{
  m_ui->m_attributesComboBox->setEnabled(true);
}

void te::vp::BasicOpOutputWizardPage::onTargetDatasourceToolButtonPressed()
{
  m_ui->m_newLayerNameLineEdit->clear();
  m_ui->m_newLayerNameLineEdit->setEnabled(true);
  te::qt::widgets::DataSourceSelectorDialog dlg(this);
  dlg.exec();

  std::list<te::da::DataSourceInfoPtr> dsPtrList = dlg.getSelecteds();

  if(dsPtrList.size() <= 0)
    return;

  std::list<te::da::DataSourceInfoPtr>::iterator it = dsPtrList.begin();

  m_ui->m_repositoryLineEdit->setText(QString(it->get()->getTitle().c_str()));

  m_outputDatasource = *it;
  
  m_toFile = false;
}

void te::vp::BasicOpOutputWizardPage::onTargetFileToolButtonPressed()
{
  m_ui->m_newLayerNameLineEdit->clear();
  m_ui->m_repositoryLineEdit->clear();
  
  QString fileName = QFileDialog::getSaveFileName(this, tr("Save as..."),
                                                        QString(), tr("Shapefile (*.shp *.SHP);;"),0, QFileDialog::DontConfirmOverwrite);
  
  if (fileName.isEmpty())
    return;
  
  boost::filesystem::path outfile(fileName.toStdString());
  std::string aux = outfile.leaf().string();
  m_ui->m_newLayerNameLineEdit->setText(aux.c_str());
  aux = outfile.string();
  m_ui->m_repositoryLineEdit->setText(aux.c_str());
  
  m_toFile = true;
  m_ui->m_newLayerNameLineEdit->setEnabled(false);
}
