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
  \file terralib/sa/qt/ProximityMatrixCreatorDialog.cpp

  \brief A dialog for Proximity Matrix Creation
*/

// TerraLib
#include "../../common/Logger.h"
#include "../../common/progress/ProgressManager.h"
#include "../../common/Translator.h"
#include "../../common/STLUtils.h"
#include "../../dataaccess/utils/Utils.h"
#include "../../maptools/DataSetLayer.h"
#include "../core/GPMBuilder.h"
#include "../core/GPMConstructorAbstractStrategy.h"
#include "../core/GPMConstructorAdjacencyStrategy.h"
#include "../core/GPMConstructorDistanceStrategy.h"
#include "../core/GPMWeightsAbstractStrategy.h"
#include "../core/GPMWeightsInverseDistanceStrategy.h"
#include "../core/GPMWeightsNoWeightsStrategy.h"
#include "../core/GPMWeightsSquaredInverseDistanceStrategy.h"
#include "../core/SpatialWeightsExchanger.h"
#include "../Exception.h"
#include "ProximityMatrixCreatorDialog.h"
#include "ui_ProximityMatrixCreatorDialogForm.h"

// Qt
#include <QFileDialog>
#include <QMessageBox>
#include <QValidator>

Q_DECLARE_METATYPE(te::map::AbstractLayerPtr);

te::sa::ProximityMatrixCreatorDialog::ProximityMatrixCreatorDialog(QWidget* parent, Qt::WindowFlags f)
  : QDialog(parent, f),
    m_ui(new Ui::ProximityMatrixCreatorDialogForm)
{
// add controls
  m_ui->setupUi(this);

  m_ui->m_distanceLineEdit->setValidator(new QDoubleValidator(this));
  m_ui->m_nearNeighborLineEdit->setValidator(new QIntValidator(this));

// add icons
  m_ui->m_imgLabel->setPixmap(QIcon::fromTheme("sa-proxmatrixcreator-hint").pixmap(112,48));
  m_ui->m_fileToolButton->setIcon(QIcon::fromTheme("folder-open"));

// connectors
  connect(m_ui->m_inputLayerComboBox, SIGNAL(activated(int)), this, SLOT(onInputLayerComboBoxActivated(int)));
  connect(m_ui->m_fileToolButton, SIGNAL(clicked()), this, SLOT(onFileToolButtonClicked()));
  connect(m_ui->m_okPushButton, SIGNAL(clicked()), this, SLOT(onOkPushButtonClicked()));

// help info
  m_ui->m_helpPushButton->setNameSpace("dpi.inpe.br.plugins"); 
  m_ui->m_helpPushButton->setPageReference("plugins/sa/sa_proxmatrixcreator.html");
}

te::sa::ProximityMatrixCreatorDialog::~ProximityMatrixCreatorDialog()
{
}

void te::sa::ProximityMatrixCreatorDialog::setLayers(std::list<te::map::AbstractLayerPtr> layers)
{
  std::list<te::map::AbstractLayerPtr>::iterator it = layers.begin();

  while(it != layers.end())
  {
    te::map::AbstractLayerPtr l = *it;

    std::auto_ptr<te::da::DataSetType> dsType = l->getSchema();

    te::map::DataSetLayer* dsLayer = dynamic_cast<te::map::DataSetLayer*>(l.get());

    if(dsLayer && dsType->hasGeom())
      m_ui->m_inputLayerComboBox->addItem(it->get()->getTitle().c_str(), QVariant::fromValue(l));

    ++it;
  }

// fill attributes combo
  if(m_ui->m_inputLayerComboBox->count() > 0)
    onInputLayerComboBoxActivated(0);
}

void te::sa::ProximityMatrixCreatorDialog::onInputLayerComboBoxActivated(int index)
{
  QVariant varLayer = m_ui->m_inputLayerComboBox->itemData(index, Qt::UserRole);
  
  te::map::AbstractLayerPtr l = varLayer.value<te::map::AbstractLayerPtr>();

  std::auto_ptr<te::da::DataSetType> dsType = l->getSchema();

  std::vector<te::dt::Property*> propVec = dsType->getProperties();

  for(std::size_t t = 0; t < propVec.size(); ++t)
  {
    m_ui->m_attrIdComboBox->addItem(propVec[t]->getName().c_str());
  }

  m_ui->m_nameLineEdit->setText(l->getTitle().c_str());
}

void te::sa::ProximityMatrixCreatorDialog::onFileToolButtonClicked()
{
  if(!m_ui->m_dsRadioButton->isChecked())
  {
    QString fileName = QFileDialog::getExistingDirectory(this, tr("Save To File"), "");

    if(fileName.isEmpty())
      return;

    m_ui->m_locationLineEdit->setText(fileName);
  }
}

void te::sa::ProximityMatrixCreatorDialog::onOkPushButtonClicked()
{
  // check interface parameters
  if(m_ui->m_nameLineEdit->text().isEmpty())
  {
    QMessageBox::warning(this, tr("Warning"), tr("Output name not defined."));
    return;
  }

  if(m_ui->m_locationLineEdit->text().isEmpty())
  {
    QMessageBox::warning(this, tr("Warning"), tr("Output location not defined."));
    return;
  }

  if(m_ui->m_inputLayerComboBox->currentText().isEmpty() || m_ui->m_attrIdComboBox->currentText().isEmpty())
  {
    QMessageBox::warning(this, tr("Warning"), tr("Input information not defined."));
    return;
  }

  //get input parameters
  std::string attrName = m_ui->m_attrIdComboBox->currentText().toStdString();

  QVariant varLayer = m_ui->m_inputLayerComboBox->itemData(m_ui->m_inputLayerComboBox->currentIndex(), Qt::UserRole);
  
  te::map::AbstractLayerPtr l = varLayer.value<te::map::AbstractLayerPtr>();

  te::map::DataSetLayer* dsLayer = dynamic_cast<te::map::DataSetLayer*>(l.get());

  if(!dsLayer)
  {
    QMessageBox::warning(this, tr("Warning"), tr("Error getting input layer."));
    return;
  }

  te::da::DataSourcePtr ds = te::da::GetDataSource(dsLayer->getDataSourceId(), true);

  //get constructor strategy
  te::sa::GPMConstructorAbstractStrategy* constructor = 0;

  if(m_ui->m_buildStratContiguityRadioButton->isChecked())
  {
    constructor = new te::sa::GPMConstructorAdjacencyStrategy(m_ui->m_calcDistCheckBox->isChecked());
  }
  else if(m_ui->m_buildStratDistanceRadioButton->isChecked())
  {
    if(m_ui->m_distanceLineEdit->text().isEmpty())
    {
      QMessageBox::warning(this, tr("Warning"), tr("Distance Value not Defined."));
      return;
    }

    constructor = new te::sa::GPMConstructorDistanceStrategy(m_ui->m_distanceLineEdit->text().toDouble());
  }

  //get weights strategy
  te::sa::GPMWeightsAbstractStrategy* weights = 0;

  if(m_ui->m_weightNoWeightRadioButton->isChecked())
  {
    weights = new te::sa::GPMWeightsNoWeightsStrategy(m_ui->m_normalizeWeightCheckBox->isChecked());
  }
  else if(m_ui->m_weightInverseDistRadioButton->isChecked())
  {
    weights = new te::sa::GPMWeightsInverseDistanceStrategy(m_ui->m_normalizeWeightCheckBox->isChecked()); // using default parameters
  }
  else if(m_ui->m_weightSquareInverseDistRadioButton->isChecked())
  {
    weights = new te::sa::GPMWeightsSquaredInverseDistanceStrategy(m_ui->m_normalizeWeightCheckBox->isChecked()); // using default parameters
  }

  //run operation
  te::sa::GPMBuilder builder(constructor, weights);

  std::auto_ptr<te::sa::GeneralizedProximityMatrix> gpm;

  try
  {
    builder.setGPMInfo(ds, dsLayer->getDataSetName(), attrName);

    gpm = builder.build();
  }
  catch(...)
  {
    QMessageBox::warning(this, tr("Warning"), tr("Internal Error."));
    return;
  }

  //save gpm
  if(!m_ui->m_dsRadioButton->isChecked())
  {
    //define the file name
    std::string ext = "";

    if(m_ui->m_galRadioButton->isChecked())
      ext = ".gal";
    else if(m_ui->m_gwtRadioButton->isChecked())
      ext = ".gwt";

    std::string name = m_ui->m_nameLineEdit->text().toStdString();

    std::string fullFileName = m_ui->m_locationLineEdit->text().toStdString();

    fullFileName += "/" + name + ext;

    //export
    te::sa::SpatialWeightsExchanger exchanger;

    if(m_ui->m_galRadioButton->isChecked())
      exchanger .exportToGAL(gpm.get(), fullFileName);
    else if(m_ui->m_gwtRadioButton->isChecked())
      exchanger .exportToGWT(gpm.get(), fullFileName, 1); // use 1 pos to export the weight information
  }

  accept();
}

