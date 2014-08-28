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
  \file terralib/qt/widgets/connector/cellspace/CreateCellularSpaceDialog.cpp

  \brief ....
*/

// TerraLib
#include "../../../cellspace/CellSpaceOperations.h"
#include "../../../common/Exception.h"
#include "../../../common/StringUtils.h"
#include "../../../common/Translator.h"
#include "../../../common/UnitsOfMeasureManager.h"
#include "../../../dataaccess/datasource/DataSource.h"
#include "../../../dataaccess/datasource/DataSourceFactory.h"
#include "../../../dataaccess/datasource/DataSourceTransactor.h"
#include "../../../dataaccess/utils/Utils.h"
#include "../../../geometry/Utils.h"
#include "../../../qt/widgets/layer/utils/DataSet2Layer.h"
#include "../../../qt/widgets/srs/SRSManagerDialog.h"
#include "../../../raster.h"
#include "../../../srs/SpatialReferenceSystemManager.h"
#include "../../widgets/datasource/selector/DataSourceSelectorDialog.h"
#include "ui_CreateCellularSpaceDialogForm.h"
#include "CreateCellularSpaceDialog.h"

// Qt
#include <QFileDialog>
#include <QMessageBox>
#include <QToolButton>

// Boost
#include <boost/filesystem.hpp>

Q_DECLARE_METATYPE(te::map::AbstractLayerPtr);
Q_DECLARE_METATYPE(te::common::UnitOfMeasurePtr);

te::qt::plugins::cellspace::CreateCellularSpaceDialog::CreateCellularSpaceDialog(QWidget* parent, Qt::WindowFlags f)
  : QDialog(parent, f),
    m_ui(new Ui::CreateCellularSpaceDialogForm),
    m_isFile(false),
    m_outputDataSetName(""),
    m_currentSRID(4618)
{
// add controls
  m_ui->setupUi(this);

// Set icons
  m_ui->m_maskToolButton->setIcon(QIcon::fromTheme("cellspace-mask-hint"));
  m_ui->m_noMaskToolButton->setIcon(QIcon::fromTheme("cellspace-no-mask-hint"));
  m_ui->m_vectorToolButton->setIcon(QIcon::fromTheme("cellspace-vector-hint"));
  m_ui->m_pointsToolButton->setIcon(QIcon::fromTheme("cellspace-points-hint"));
  m_ui->m_rasterToolButton->setIcon(QIcon::fromTheme("cellspace-raster-hint"));
  m_ui->m_targetDatasourceToolButton->setIcon(QIcon::fromTheme("datasource"));
  m_ui->m_srsToolButton->setIcon(QIcon::fromTheme("srs"));

// Set validators
  m_ui->m_llxLineEdit->setValidator(new QDoubleValidator(this));
  m_ui->m_llyLineEdit->setValidator(new QDoubleValidator(this));
  m_ui->m_urxLineEdit->setValidator(new QDoubleValidator(this));
  m_ui->m_uryLineEdit->setValidator(new QDoubleValidator(this));
  m_ui->m_resXLineEdit->setValidator(new QDoubleValidator(0, 99999999, 8, this));
  m_ui->m_resYLineEdit->setValidator(new QDoubleValidator(0, 99999999, 8, this));

  initUnitsOfMeasure();

  connect(m_ui->m_layersComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(onLayersComboBoxChanged(int)));
  connect(m_ui->m_unitComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(onUnitComboBoxChanged(int)));
  connect(m_ui->m_resXLineEdit, SIGNAL(textChanged(const QString &)), this, SLOT(onResXLineEditTextChanged(const QString &)));
  connect(m_ui->m_resYLineEdit, SIGNAL(textChanged(const QString &)), this, SLOT(onResYLineEditTextChanged(const QString &)));
  connect(m_ui->m_llxLineEdit, SIGNAL(textChanged(const QString &)), this, SLOT(onEnvelopeChanged(const QString &)));
  connect(m_ui->m_llyLineEdit, SIGNAL(textChanged(const QString &)), this, SLOT(onEnvelopeChanged(const QString &)));
  connect(m_ui->m_urxLineEdit, SIGNAL(textChanged(const QString &)), this, SLOT(onEnvelopeChanged(const QString &)));
  connect(m_ui->m_uryLineEdit, SIGNAL(textChanged(const QString &)), this, SLOT(onEnvelopeChanged(const QString &)));
  connect(m_ui->m_targetFileToolButton, SIGNAL(clicked()), this, SLOT(onTargetFileToolButtonClicked()));
  connect(m_ui->m_targetDatasourceToolButton, SIGNAL(clicked()), this, SLOT(onTargetDatasourceToolButtonClicked()));
  connect(m_ui->m_createPushButton, SIGNAL(clicked()), this, SLOT(onCreatePushButtonClicked()));
  connect(m_ui->m_vectorToolButton, SIGNAL(toggled(bool)), this, SLOT(onVectorToolButtonToggled(bool)));
  connect(m_ui->m_pointsToolButton, SIGNAL(toggled(bool)), this, SLOT(onPointsToolButtonToggled(bool)));
  connect(m_ui->m_rasterToolButton, SIGNAL(toggled(bool)), this, SLOT(onRasterToolButtonToggled(bool)));
  connect(m_ui->m_srsToolButton, SIGNAL(clicked()), this, SLOT(onSrsToolButtonClicked()));
}

te::qt::plugins::cellspace::CreateCellularSpaceDialog::~CreateCellularSpaceDialog()
{
}

void te::qt::plugins::cellspace::CreateCellularSpaceDialog::setLayers(std::list<te::map::AbstractLayerPtr> layers)
{
  m_ui->m_layersComboBox->addItem(tr("NONE"));

  std::list<te::map::AbstractLayerPtr>::iterator it = layers.begin();

  while(it != layers.end())
  {
    std::auto_ptr<te::da::DataSetType> dsType = it->get()->getSchema();
    if(dsType->hasGeom())
      m_ui->m_layersComboBox->addItem(QString(it->get()->getTitle().c_str()), QVariant::fromValue(*it));
    ++it;
  }

  // If there is a layer, set as current
  if(m_ui->m_layersComboBox->count() > 2)
  {
    m_ui->m_layersComboBox->setCurrentIndex(1);
    m_ui->m_srsToolButton->setEnabled(false);
    m_currentSRID = getReferenceLayer()->getSRID();
    showSRS();
  }
  else
  {
    m_ui->m_srsToolButton->setEnabled(true);
  }
}

void te::qt::plugins::cellspace::CreateCellularSpaceDialog::onLayersComboBoxChanged(int index)
{
  if(isNone())
  {
    m_ui->m_srsToolButton->setEnabled(true);
    m_ui->m_maskToolButton->setEnabled(false);
    m_ui->m_noMaskToolButton->setChecked(true);
    clearEnvelope();
    clearResolution();
    return;
  }

  m_ui->m_srsToolButton->setEnabled(false);
  m_ui->m_maskToolButton->setEnabled(true);
  m_ui->m_maskToolButton->setChecked(true);

  m_ui->m_srsToolButton->setEnabled(false);

  te::map::AbstractLayerPtr layer = getReferenceLayer();

  if(!layer)
    return;

  te::gm::Envelope env = layer->getExtent();
  showEnvelope(env);

  m_currentSRID = layer->getSRID();

  showSRS();

  te::common::UnitOfMeasurePtr unit = te::srs::SpatialReferenceSystemManager::getInstance().getUnit(m_currentSRID);

  m_ui->m_sridLabel->setText(te::srs::SpatialReferenceSystemManager::getInstance().getName(m_currentSRID).c_str());

  if(!unit.get())
  {
    // METRE
    m_ui->m_unitComboBox->setCurrentIndex(1);
  }
  else
  {
    setCurrentUnit(unit);
  }
}

void te::qt::plugins::cellspace::CreateCellularSpaceDialog::onResXLineEditTextChanged(const QString & text)
{
  if(m_ui->m_resXLineEdit->text().isEmpty())
  {
    m_ui->m_colsLineEdit->clear();
    m_ui->m_rowsLineEdit->clear();
    return;
  }

  updateValues();
}

void te::qt::plugins::cellspace::CreateCellularSpaceDialog::onResYLineEditTextChanged(const QString & text)
{
  if(m_ui->m_resYLineEdit->text().isEmpty())
  {
    m_ui->m_colsLineEdit->clear();
    m_ui->m_rowsLineEdit->clear();
    return;
  }

  updateValues();
}

void te::qt::plugins::cellspace::CreateCellularSpaceDialog::onEnvelopeChanged(const QString & text)
{
  updateValues();
}

void te::qt::plugins::cellspace::CreateCellularSpaceDialog::updateValues()
{
  if(!isBasicInfoSet())
    return;

  double resX = m_ui->m_resXLineEdit->text().toDouble();
  double resY = m_ui->m_resYLineEdit->text().toDouble();

  te::map::AbstractLayerPtr layer = getReferenceLayer();

  te::common::UnitOfMeasurePtr layerUnit;
  te::common::UnitOfMeasurePtr currentUnit = getCurrentUnit();

  if(layer)
    layerUnit = te::srs::SpatialReferenceSystemManager::getInstance().getUnit(layer->getSRID());
  else
    layerUnit = currentUnit;

  if(layerUnit != currentUnit)
  {
    try
    {
      double factorX = te::common::UnitsOfMeasureManager::getInstance().getConversion(layerUnit->getName(), currentUnit->getName());
      double factorY = te::common::UnitsOfMeasureManager::getInstance().getConversion(layerUnit->getName(), currentUnit->getName());
      resX = resX/factorX;
      resY = resY/factorY;
    }
    catch(te::common::Exception& e)
    {
      QMessageBox::warning(this, tr("Cellular Spaces"), e.what());
      clearResolution();
    }
  }

  te::gm::Envelope env = getOutputEnvelope();

  if(!env.isValid())
  {
    // MESSAGE ERROR!!!  TODO
    return;
  }

  double lWidth = env.getWidth();
  double lHeight = env.getHeight();

  te::gm::Envelope newEnv = te::gm::AdjustToCut(env, resX, resY);

  int maxCols = (int)((newEnv.m_urx - newEnv.m_llx)/resX);
  int maxRows = (int)((newEnv.m_ury - newEnv.m_lly)/resY);

  m_ui->m_colsLineEdit->setText(QString::number(maxCols));
  m_ui->m_rowsLineEdit->setText(QString::number(maxRows));
}

void te::qt::plugins::cellspace::CreateCellularSpaceDialog::onCreatePushButtonClicked()
{
  std::string errors;
  if(!checkList(errors))
  {
    QMessageBox::warning(this, tr("Cellular Spaces"), errors.c_str());
    return;
  }

  m_outputDataSetName = m_ui->m_newLayerNameLineEdit->text().toStdString();
  

  te::map::AbstractLayerPtr referenceLayer;
  
  if(!isNone())
    referenceLayer = getReferenceLayer();
  
  double resX = m_ui->m_resXLineEdit->text().toDouble();
  double resY = m_ui->m_resYLineEdit->text().toDouble();
  bool isPolygonsAsMask = m_ui->m_maskToolButton->isChecked();

  std::auto_ptr<te::cellspace::CellularSpacesOperations> cellSpaceOp(new te::cellspace::CellularSpacesOperations());

  te::cellspace::CellularSpacesOperations::CellSpaceType type;

  if(m_ui->m_vectorToolButton->isChecked())
    type = te::cellspace::CellularSpacesOperations::CELLSPACE_POLYGONS;
  else if(m_ui->m_pointsToolButton->isChecked())
    type = te::cellspace::CellularSpacesOperations::CELLSPACE_POINTS;
  else if(m_ui->m_rasterToolButton->isChecked())
    type = te::cellspace::CellularSpacesOperations::CELLSPACE_RASTER;

  if(isNone())
    cellSpaceOp->createCellSpace(m_outputDataSetName, resX, resY, getEnvelope(), m_currentSRID, type);
  else
    cellSpaceOp->createCellSpace(m_outputDataSetName, referenceLayer, resX, resY, isPolygonsAsMask, type);

  std::auto_ptr<te::da::DataSetType> dst(cellSpaceOp->getDataSetType());

  if(type == te::cellspace::CellularSpacesOperations::CELLSPACE_RASTER)
  {
    std::auto_ptr<te::rst::Raster> raster(cellSpaceOp->getRaster());

    te::rst::CreateCopy(*raster.release(), m_ui->m_repositoryLineEdit->text().toStdString(), "GDAL");
  }

  std::auto_ptr<te::da::DataSet> ds(cellSpaceOp->getDataSet());

  std::string accessDriver;
  std::map<std::string, std::string> connInfo;

  te::da::DataSourcePtr source;

  if(m_isFile)
  {
    accessDriver = "OGR";
    connInfo["URI"] = m_ui->m_repositoryLineEdit->text().toStdString();

    source.reset(te::da::DataSourceFactory::make("OGR").release());
    source->setConnectionInfo(connInfo);
    source->open();

    dst->setName(m_ui->m_newLayerNameLineEdit->text().toStdString());
  }
  else
  {
    accessDriver = m_outDataSourceInfo->getAccessDriver();
    connInfo = m_outDataSourceInfo->getConnInfo();

    source = te::da::GetDataSource(m_outDataSourceInfo->getId(), true);
  }

  if(source.get() == 0)
  {
    // ERROR MESSAGE
    return;
  }

  std::auto_ptr<te::da::DataSourceTransactor> t = source->getTransactor();

  try
  {
    t->begin();

    std::map<std::string, std::string> op;

    t->createDataSet(dst.get(), op);

    t->add(m_outputDataSetName, ds.get(), op);

    t->commit();
  }
  catch(te::common::Exception& e)
  {
    t->rollBack();

    QMessageBox::warning(this, tr("Cellular Spaces"), e.what());
  }

  accept();
}

void te::qt::plugins::cellspace::CreateCellularSpaceDialog::onTargetFileToolButtonClicked()
{
  m_ui->m_newLayerNameLineEdit->clear();
  m_ui->m_repositoryLineEdit->clear();

  QString extension;

  if(m_ui->m_rasterToolButton->isChecked())
    extension = tr("TIF (*.tif *.Tif);;");
  else
    extension = tr("Shapefile (*.shp *.SHP);;");

  QString fileName = QFileDialog::getSaveFileName(this, tr("Save as..."),
                     QString(), extension, 0, QFileDialog::DontConfirmOverwrite);

  if (fileName.isEmpty())
    return;

  boost::filesystem::path outfile(fileName.toStdString());
  std::string aux = outfile.leaf().string();
  m_ui->m_newLayerNameLineEdit->setText(aux.c_str());
  aux = outfile.string();
  m_ui->m_repositoryLineEdit->setText(aux.c_str());

  m_isFile = true;

  m_ui->m_newLayerNameLineEdit->setEnabled(false);
}

void te::qt::plugins::cellspace::CreateCellularSpaceDialog::onTargetDatasourceToolButtonClicked()
{
  m_ui->m_newLayerNameLineEdit->clear();
  m_ui->m_repositoryLineEdit->clear();

  te::qt::widgets::DataSourceSelectorDialog dlg(this);
  dlg.exec();

  std::list<te::da::DataSourceInfoPtr> dsPtrList = dlg.getSelecteds();

  if(dsPtrList.size() <= 0)
    return;

  std::list<te::da::DataSourceInfoPtr>::iterator it = dsPtrList.begin();

  m_outDataSourceInfo = *it;

  m_ui->m_repositoryLineEdit->setText(QString(it->get()->getTitle().c_str()));

  m_isFile = false;

  m_ui->m_newLayerNameLineEdit->setEnabled(true);
}

void te::qt::plugins::cellspace::CreateCellularSpaceDialog::onUnitComboBoxChanged(int index)
{
  updateValues();
}

bool te::qt::plugins::cellspace::CreateCellularSpaceDialog::isBasicInfoSet()
{
  if( m_ui->m_llxLineEdit->text().isEmpty() ||
      m_ui->m_llyLineEdit->text().isEmpty() ||
      m_ui->m_urxLineEdit->text().isEmpty() ||
      m_ui->m_uryLineEdit->text().isEmpty() ||
      m_ui->m_resXLineEdit->text().isEmpty() ||
      m_ui->m_resYLineEdit->text().isEmpty())
  {
    return false;
  }

  return true;
}

void te::qt::plugins::cellspace::CreateCellularSpaceDialog::initUnitsOfMeasure()
{
  te::common::UnitsOfMeasureManager::const_iterator it = te::common::UnitsOfMeasureManager::getInstance().begin();
  te::common::UnitsOfMeasureManager::const_iterator itend = te::common::UnitsOfMeasureManager::getInstance().end();
  while (it != itend)
  {
    te::common::UnitOfMeasurePtr uptr = it->second;
    m_ui->m_unitComboBox->addItem(uptr->getName().c_str(), QVariant::fromValue(uptr));
    ++it;
  }

  m_ui->m_unitComboBox->setCurrentIndex(1);
}

te::gm::Envelope te::qt::plugins::cellspace::CreateCellularSpaceDialog::getOutputEnvelope()
{
  te::gm::Envelope env;

  env.m_llx = m_ui->m_llxLineEdit->text().toDouble();
  env.m_lly = m_ui->m_llyLineEdit->text().toDouble();
  env.m_urx = m_ui->m_urxLineEdit->text().toDouble();
  env.m_ury = m_ui->m_uryLineEdit->text().toDouble();

  return env;
}

void te::qt::plugins::cellspace::CreateCellularSpaceDialog::showEnvelope(const te::gm::Envelope env)
{
  char txt[40];
  sprintf(txt,"%.8g",env.m_llx);
  m_ui->m_llxLineEdit->setText(QString(txt));
  sprintf(txt,"%.8g",env.m_lly);
  m_ui->m_llyLineEdit->setText(QString(txt));
  sprintf(txt,"%.8g",env.m_urx);
  m_ui->m_urxLineEdit->setText(QString(txt));
  sprintf(txt,"%.8g",env.m_ury);
  m_ui->m_uryLineEdit->setText(QString(txt));
}

te::gm::Envelope te::qt::plugins::cellspace::CreateCellularSpaceDialog::getEnvelope()
{
  te::gm::Envelope env;

  env.m_llx = m_ui->m_llxLineEdit->text().toDouble();
  env.m_lly = m_ui->m_llyLineEdit->text().toDouble();
  env.m_urx = m_ui->m_urxLineEdit->text().toDouble();
  env.m_ury = m_ui->m_uryLineEdit->text().toDouble();

  return env;
}

te::map::AbstractLayerPtr te::qt::plugins::cellspace::CreateCellularSpaceDialog::getReferenceLayer()
{
  QVariant varLayer = m_ui->m_layersComboBox->itemData(m_ui->m_layersComboBox->currentIndex(), Qt::UserRole);
  return varLayer.value<te::map::AbstractLayerPtr>();
}

te::common::UnitOfMeasurePtr te::qt::plugins::cellspace::CreateCellularSpaceDialog::getCurrentUnit()
{
  QVariant varUnit = m_ui->m_unitComboBox->itemData(m_ui->m_unitComboBox->currentIndex(), Qt::UserRole);
  return varUnit.value<te::common::UnitOfMeasurePtr>();
}

void te::qt::plugins::cellspace::CreateCellularSpaceDialog::setCurrentUnit(te::common::UnitOfMeasurePtr unit)
{
  for(int i = 0; i < m_ui->m_unitComboBox->count(); ++i)
  {
    std::string name = m_ui->m_unitComboBox->itemText(i).toStdString();

    if(unit->getName() == name)
    {
      m_ui->m_unitComboBox->setCurrentIndex(i);
      break;
    }
  }
}

bool te::qt::plugins::cellspace::CreateCellularSpaceDialog::checkList(std::string& errors)
{
  bool noErrors = true;

  std::string name = m_ui->m_newLayerNameLineEdit->text().toStdString();
  te::map::AbstractLayerPtr referenceLayer = getReferenceLayer();
  double resX = m_ui->m_resXLineEdit->text().toDouble();
  double resY = m_ui->m_resYLineEdit->text().toDouble();

  errors = tr("Some information is missing:").toStdString();

  if(name.empty())
  {
    errors += "\n - " + tr("Output name").toStdString();
    noErrors = false;
  }

  if(m_currentSRID <= 0)
  {
    errors += "\n - " + tr("Projection").toStdString();
    noErrors = false;
  }

  te::gm::Envelope env = getOutputEnvelope();

  if(!env.isValid())
  {
    errors += "\n - " + tr("Invalid envelope").toStdString();
    noErrors = false;
  }

  double lWidth = env.getWidth();
  double lHeight = env.getHeight();

  if (resX < 0. || resX >= lWidth)
  {
    char txt[40];
    sprintf(txt,"%.8g",lWidth);
    errors += "\n - " + tr("Resolution in X must be > 0 and < ").toStdString() + txt;
    noErrors = false;
  }

  if (resY < 0. || resY >= lHeight)
  {
    char txt[40];
    sprintf(txt,"%.8g",lHeight);
    errors += "\n - " + tr("Resolution in X must be > 0 and < ").toStdString() + txt;
    noErrors = false;
  }

  return noErrors;
}

te::map::AbstractLayerPtr te::qt::plugins::cellspace::CreateCellularSpaceDialog::getLayer()
{
  te::da::DataSourcePtr outDataSource = te::da::GetDataSource(m_outDataSourceInfo->getId());

  te::qt::widgets::DataSet2Layer converter(m_outDataSourceInfo->getId());

  te::da::DataSetTypePtr dt(outDataSource->getDataSetType(m_outputDataSetName).release());

  return converter(dt);
}

void te::qt::plugins::cellspace::CreateCellularSpaceDialog::onVectorToolButtonToggled(bool isToggled)
{
  m_ui->m_targetDatasourceToolButton->setEnabled(isToggled);
  m_ui->m_targetFileToolButton->setEnabled(isToggled);
}

void te::qt::plugins::cellspace::CreateCellularSpaceDialog::onPointsToolButtonToggled(bool isToggled)
{
  m_ui->m_targetDatasourceToolButton->setEnabled(isToggled);
  m_ui->m_targetFileToolButton->setEnabled(isToggled);
}

void te::qt::plugins::cellspace::CreateCellularSpaceDialog::onRasterToolButtonToggled(bool isToggled)
{
  m_ui->m_targetDatasourceToolButton->setEnabled(!isToggled);
  m_ui->m_targetFileToolButton->setEnabled(isToggled);
}

void te::qt::plugins::cellspace::CreateCellularSpaceDialog::onSrsToolButtonClicked()
{
  te::qt::widgets::SRSManagerDialog srsDialog(this);
  srsDialog.setWindowTitle(tr("Choose the SRS"));

  if(srsDialog.exec() == QDialog::Rejected)
    return;

  m_currentSRID = srsDialog.getSelectedSRS().first;

  showSRS();

  te::common::UnitOfMeasurePtr unit = te::srs::SpatialReferenceSystemManager::getInstance().getUnit(m_currentSRID);

  setCurrentUnit(unit);

  clearEnvelope();
  clearResolution();
}

void te::qt::plugins::cellspace::CreateCellularSpaceDialog::clearEnvelope()
{
  m_ui->m_llxLineEdit->clear();
  m_ui->m_llyLineEdit->clear();
  m_ui->m_urxLineEdit->clear();
  m_ui->m_uryLineEdit->clear();
}

void te::qt::plugins::cellspace::CreateCellularSpaceDialog::clearResolution()
{
  m_ui->m_resXLineEdit->clear();
  m_ui->m_resYLineEdit->clear();
  m_ui->m_colsLineEdit->clear();
  m_ui->m_rowsLineEdit->clear();
}

void te::qt::plugins::cellspace::CreateCellularSpaceDialog::showSRS()
{
  std::string name = te::srs::SpatialReferenceSystemManager::getInstance().getName(m_currentSRID);

  m_ui->m_sridLabel->setText(name.c_str());
}

bool te::qt::plugins::cellspace::CreateCellularSpaceDialog::isNone()
{
  if(m_ui->m_layersComboBox->currentIndex() == 0)
    return true;
  else
    return false;
}