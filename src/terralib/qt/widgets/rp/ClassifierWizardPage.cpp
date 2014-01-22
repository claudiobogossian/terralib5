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
  \file terralib/qt/widgets/rp/ClassifierWizardPage.cpp

  \brief This file defines a class for a Classifier Wizard page.
*/

// TerraLib
#include "../../../dataaccess/dataset/DataSet.h"
#include "../../../dataaccess/dataset/DataSetType.h"
#include "../../../dataaccess/utils/Utils.h"
#include "../../../geometry/GeometryProperty.h"
#include "../../../raster/Raster.h"
#include "../../../rp/ClassifierKMeansStrategy.h"
#include "../../../rp/ClassifierISOSegStrategy.h"
#include "../classification/ROIManagerDialog.h"
#include "ClassifierWizardPage.h"
#include "ui_ClassifierWizardPageForm.h"

// Qt
#include <QGridLayout>
#include <QtGui/QDoubleValidator>
#include <QtGui/QIntValidator>
#include <QtGui/QMessageBox>

// stl
#include <memory>

Q_DECLARE_METATYPE(te::map::AbstractLayerPtr);

te::qt::widgets::ClassifierWizardPage::ClassifierWizardPage(QWidget* parent)
  : QWizardPage(parent),
    m_ui(new Ui::ClassifierWizardPageForm),
    m_layer(0)
{
// setup controls
  m_ui->setupUi(this);

  fillClassifierTypes();

  QIntValidator* intMaxPoints = new QIntValidator(this);
  intMaxPoints->setBottom(0);
  m_ui->m_kMeansMaxPointsLineEdit->setValidator(intMaxPoints);

  QDoubleValidator* doubleStopCriteria = new QDoubleValidator(this);
  doubleStopCriteria->setBottom(0.0);
  m_ui->m_kMeansStopCriteriaLineEdit->setValidator(doubleStopCriteria);

//configure page
  this->setTitle(tr("Classifier"));
  this->setSubTitle(tr("Select the type of classifier and set their specific parameters."));

  //configure roi manager
  m_roiMngDlg.reset(new te::qt::widgets::ROIManagerDialog(this, Qt::Tool));

  //connects
  connect(m_ui->m_acquireToolButton, SIGNAL(toggled(bool)), this, SLOT(showROIManager(bool)));
  connect(m_roiMngDlg.get(), SIGNAL(roiManagerClosed()), this, SLOT(onROIManagerClosed()));

  m_ui->m_acquireToolButton->setIcon(QIcon::fromTheme("wand"));
}

te::qt::widgets::ClassifierWizardPage::~ClassifierWizardPage()
{

}

void te::qt::widgets::ClassifierWizardPage::set(te::map::AbstractLayerPtr layer)
{
  m_layer = layer;

  m_roiMngDlg->set(m_layer);

  listBands();
}

void te::qt::widgets::ClassifierWizardPage::setList(std::list<te::map::AbstractLayerPtr>& layerList)
{
  m_roiMngDlg->setList(layerList);

  m_ui->m_isosegLayersComboBox->clear();

  std::list<te::map::AbstractLayerPtr>::iterator it = layerList.begin();

  while(it != layerList.end())
  {
    te::map::AbstractLayerPtr l = *it;

    std::auto_ptr<te::da::DataSetType> dsType = l->getSchema();

    if(dsType->hasGeom())
      m_ui->m_isosegLayersComboBox->addItem(it->get()->getTitle().c_str(), QVariant::fromValue(l));

    ++it;
  }
}

te::rp::Classifier::InputParameters te::qt::widgets::ClassifierWizardPage::getInputParams()
{
  int idx = m_ui->m_classifierTypeComboBox->currentIndex();
  int type = m_ui->m_classifierTypeComboBox->itemData(idx).toInt();

  te::rp::Classifier::InputParameters algoInputParams;

  if(type == CLASSIFIER_ISOSEG)
  {
    algoInputParams.m_strategyName = "isoseg";

    te::rp::ClassifierISOSegStrategy::Parameters classifierparameters;
    classifierparameters.m_acceptanceThreshold = m_ui->m_acceptanceThresholdComboBox->currentText().toDouble();

    //get layer
    int idxLayer = m_ui->m_isosegLayersComboBox->currentIndex();
    QVariant varLayer = m_ui->m_isosegLayersComboBox->itemData(idxLayer, Qt::UserRole);
    te::map::AbstractLayerPtr layer = varLayer.value<te::map::AbstractLayerPtr>();

    //get polygons
    std::auto_ptr<te::da::DataSetType> dsType = layer->getSchema();
    te::gm::GeometryProperty* gp = te::da::GetFirstGeomProperty(dsType.get());

    if(gp && gp->getGeometryType() == te::gm::PolygonType)
    {
      std::auto_ptr<te::da::DataSet> ds = layer->getData();
      ds->moveBeforeFirst();

      while(ds->moveNext())
      {
        te::gm::Polygon* poly = (te::gm::Polygon*)ds->getGeometry(gp->getName()).release();

        algoInputParams.m_inputPolygons.push_back(poly);
      }
    }

    algoInputParams.setClassifierStrategyParams(classifierparameters);
  }
  else  if(type == CLASSIFIER_KMEANS)
  {
    algoInputParams.m_strategyName = "kmeans";

    te::rp::ClassifierKMeansStrategy::Parameters classifierparameters;
    classifierparameters.m_K = m_ui->m_kMeansClusterSpinBox->value();
    classifierparameters.m_maxIterations = m_ui->m_kMeansIterationsSpinBox->value();
    classifierparameters.m_maxInputPoints = m_ui->m_kMeansMaxPointsLineEdit->text().toInt();
    classifierparameters.m_epsilon = m_ui->m_kMeansStopCriteriaLineEdit->text().toDouble();

    algoInputParams.setClassifierStrategyParams(classifierparameters);
  }

  //get bands
  QList<QListWidgetItem*> selectedBands = m_ui->m_inputRasterBandsListWidget->selectedItems();

  QList<QListWidgetItem*>::const_iterator it = selectedBands.begin();
  QList<QListWidgetItem*>::const_iterator itend = selectedBands.end();

  while(it != itend)
  {
    algoInputParams.m_inputRasterBands.push_back((*it)->text().toUInt());

    ++it;
  }

  return algoInputParams;
}

te::rp::Classifier::OutputParameters te::qt::widgets::ClassifierWizardPage::getOutputParams()
{
  te::rp::Classifier::OutputParameters algoOutputParams;

  return algoOutputParams;
}

void te::qt::widgets::ClassifierWizardPage::fillClassifierTypes()
{
  m_ui->m_classifierTypeComboBox->clear();

  m_ui->m_classifierTypeComboBox->addItem(tr("ISOSeg"), CLASSIFIER_ISOSEG);
  m_ui->m_classifierTypeComboBox->addItem(tr("KMeans"), CLASSIFIER_KMEANS);
}

void te::qt::widgets::ClassifierWizardPage::listBands()
{
  m_ui->m_inputRasterBandsListWidget->clear();

  assert(m_layer.get());

  //get input raster
  std::auto_ptr<te::da::DataSet> ds = m_layer->getData();

  if(ds.get())
  {
    std::size_t rpos = te::da::GetFirstPropertyPos(ds.get(), te::dt::RASTER_TYPE);

    std::auto_ptr<te::rst::Raster> inputRst = ds->getRaster(rpos);

    if(inputRst.get())
    {
      for(unsigned int i = 0; i < inputRst->getNumberOfBands(); ++i)
      {
        m_ui->m_inputRasterBandsListWidget->addItem(QString::number(i));
      }
    }
  }
}

void te::qt::widgets::ClassifierWizardPage::showROIManager(bool show)
{
  if(show)
    m_roiMngDlg->show();
  else
    m_roiMngDlg->hide();
}

void te::qt::widgets::ClassifierWizardPage::onROIManagerClosed()
{
  m_ui->m_acquireToolButton->setChecked(false);
}

