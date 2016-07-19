/*  Copyright (C) 2008 National Institute For Space Research (INPE) - Brazil.

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
\file terralib/qt/widgets/utils/CheckGeomValidityDialog.cpp

\brief A dialog used to check geometries validity.
*/

// TerraLib
#include "../../../common/progress/ProgressManager.h"
#include "../../../dataaccess/utils/Utils.h"
#include "../../../geometry/Geometry.h"
#include "../../../geometry/Utils.h"
#include "../canvas/Canvas.h"
#include "../progress/ProgressViewerDialog.h"
#include "../Utils.h"
#include "ui_CheckGeomValidityDialogForm.h"
#include "CheckGeomValidityDialog.h"

// Boost
#include <boost/lexical_cast.hpp>

// Qt
#include <QMessageBox>
#include <QPointF>
#include <QTableWidget>
#include <QTableWidgetItem>

Q_DECLARE_METATYPE(te::map::AbstractLayerPtr);
Q_DECLARE_METATYPE(te::gm::TopologyValidationError);

te::qt::widgets::CheckGeomValidityDialog::CheckGeomValidityDialog(QWidget* parent, Qt::WindowFlags f)
  : QDialog(parent, f),
  m_ui(new Ui::CheckGeomValidityDialogForm),
  m_mapDisplay(0)
{
  m_ui->setupUi(this);
  
  m_ui->m_invalidTableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
  m_ui->m_invalidTableWidget->setSelectionMode(QAbstractItemView::SingleSelection);

  connect(m_ui->m_verifyPushButton, SIGNAL(clicked()), this, SLOT(onVerifyPushButtonClicked()));
  connect(m_ui->m_invalidTableWidget, SIGNAL(itemDoubleClicked(QTableWidgetItem*)), this, SLOT(onTableWidgetItemDoubleClicked(QTableWidgetItem*)));

  m_ui->m_helpPushButton->setPageReference("widgets/utils/check_geometry_validity.html");
}

te::qt::widgets::CheckGeomValidityDialog::~CheckGeomValidityDialog()
{
  m_mapDisplay->getDraftPixmap()->fill(Qt::transparent);
}

void te::qt::widgets::CheckGeomValidityDialog::setLayers(std::list<te::map::AbstractLayerPtr> layers, te::map::AbstractLayerPtr selectedLayer)
{
  for (std::list<te::map::AbstractLayerPtr>::iterator it = layers.begin(); it != layers.end(); ++it)
  {
    if(it->get()->isValid() && !it->get()->getSchema()->hasRaster())
      m_ui->m_layersComboBox->addItem(it->get()->getTitle().c_str(), QVariant::fromValue(*it));
  }

  if (selectedLayer)
  {
    m_ui->m_layersComboBox->setCurrentText(selectedLayer->getTitle().c_str());
  }
}

void te::qt::widgets::CheckGeomValidityDialog::onVerifyPushButtonClicked()
{
  if (m_ui->m_layersComboBox->count() < 1)
    return;

  m_ui->m_invalidTableWidget->setRowCount(0);

  QVariant var = m_ui->m_layersComboBox->currentData(Qt::UserRole);
  m_currentLayer =  var.value<te::map::AbstractLayerPtr>();

  std::auto_ptr<te::da::DataSetType> schema = m_currentLayer->getSchema();
  std::auto_ptr<te::da::DataSet> data = m_currentLayer->getData();

  te::da::PrimaryKey* pk = schema->getPrimaryKey();

  QStringList list;

  if (pk)
  {
    list.append(pk->getProperties()[0]->getName().c_str());
  }

  list.append(tr("Message"));

  m_ui->m_invalidTableWidget->setColumnCount(list.size());
  m_ui->m_invalidTableWidget->setHorizontalHeaderLabels(list);

  std::size_t pos = te::da::GetFirstSpatialPropertyPos(data.get());

  //progress
  std::size_t size = data->size();
  te::qt::widgets::ProgressViewerDialog v(this);
  int id = te::common::ProgressManager::getInstance().addViewer(&v);

  te::common::TaskProgress task("Checking...");
  task.setTotalSteps((int)size);

  data->moveBeforeFirst();
  
  int count = 0;

  while (data->moveNext())
  {
    if (task.isActive())
    {
      task.pulse();
    }
    else
    {
      te::common::ProgressManager::getInstance().removeViewer(id);
      break;
    }

    std::auto_ptr<te::gm::Geometry> geom = data->getGeometry(pos);

    te::gm::TopologyValidationError err;
    bool isValid = te::gm::CheckValidity(geom.get(), err);

    if (!isValid)
    {
      m_ui->m_invalidTableWidget->insertRow(count);
      
      QTableWidgetItem *msgItem = new QTableWidgetItem(err.m_message.c_str());
      msgItem->setData(Qt::UserRole, QVariant::fromValue(err));
      msgItem->setFlags(Qt::ItemIsEnabled);

      if (pk)
      {
        QTableWidgetItem *idItem = new QTableWidgetItem(data->getAsString(pk->getProperties()[0]->getName()).c_str());
        idItem->setFlags(Qt::ItemIsEnabled);

        m_ui->m_invalidTableWidget->setItem(count, 0, idItem);
        m_ui->m_invalidTableWidget->setItem(count, 1, msgItem);
      }
      else
      {
        m_ui->m_invalidTableWidget->setItem(count, 1, msgItem);
      }

      ++count;
    }
  }

  te::common::ProgressManager::getInstance().removeViewer(id);
}

void te::qt::widgets::CheckGeomValidityDialog::onTableWidgetItemDoubleClicked(QTableWidgetItem* item)
{
  const te::gm::Envelope& currEnv = m_mapDisplay->getExtent();

  double auxX = (currEnv.m_urx - currEnv.m_llx) / 2;
  double auxY = (currEnv.m_ury - currEnv.m_lly) / 2;

  int row = item->row();

  QVariant var;

  if(m_ui->m_invalidTableWidget->columnCount() == 1)
    var = m_ui->m_invalidTableWidget->item(row, 0)->data(Qt::UserRole);
  else
    var = m_ui->m_invalidTableWidget->item(row, 1)->data(Qt::UserRole);

  te::gm::TopologyValidationError err = var.value<te::gm::TopologyValidationError>();

  m_currentCoord.reset(new te::gm::Point(err.m_coordinate.x, err.m_coordinate.y, m_currentLayer->getSRID()));

  if (m_mapDisplay->getSRID() == 0)
    m_mapDisplay->setSRID(m_currentLayer->getSRID());

  try
  {
    if (m_currentLayer->getSRID() != m_mapDisplay->getSRID())
      m_currentCoord->transform(m_mapDisplay->getSRID());
  }
  catch (const std::exception & e)
  {
    m_currentCoord.reset(0);
    QMessageBox::warning(this, tr("Check Geometry Validity"), e.what());
    return;
  }

  te::gm::Envelope env;
  env.m_llx = m_currentCoord->getX() - auxX;
  env.m_lly = m_currentCoord->getY() - auxY;
  env.m_urx = m_currentCoord->getX() + auxX;
  env.m_ury = m_currentCoord->getY() + auxY;

  m_mapDisplay->setExtent(env, true);

  drawMark();
}

void te::qt::widgets::CheckGeomValidityDialog::setMapDisplay(te::qt::widgets::MapDisplay* md)
{
  m_mapDisplay = md;

  connect(m_mapDisplay, SIGNAL(extentChanged()), this, SLOT(onExtentChanged()));
}

void te::qt::widgets::CheckGeomValidityDialog::onExtentChanged()
{
  drawMark();
}

void te::qt::widgets::CheckGeomValidityDialog::drawMark()
{
  if (!m_currentCoord)
    return;

  const te::gm::Envelope& env = m_mapDisplay->getExtent();

  te::qt::widgets::Canvas* canvas = new te::qt::widgets::Canvas(m_mapDisplay->getDraftPixmap()->width(), m_mapDisplay->getDraftPixmap()->height());
  canvas->setDevice(m_mapDisplay->getDraftPixmap(), false);
  canvas->setWindow(env.m_llx, env.m_lly, env.m_urx, env.m_ury);

  QMatrix matrix = canvas->getMatrix();
  QPointF pointCanvas = matrix.map(QPointF(m_currentCoord->getX(), m_currentCoord->getY()));

  QPointF llCanvas;
  llCanvas.setX(pointCanvas.x() - 10);
  llCanvas.setY(pointCanvas.y() - 10);

  QPointF urCanvas;
  urCanvas.setX(pointCanvas.x() + 10);
  urCanvas.setY(pointCanvas.y() + 10);

  QPointF llGeo = matrix.inverted().map(llCanvas);
  QPointF urGeo = matrix.inverted().map(urCanvas);

  te::gm::Envelope mark(llGeo.x(), llGeo.y(), urGeo.x(), urGeo.y());

  te::gm::Geometry* newGeom = te::gm::GetGeomFromEnvelope(&mark, m_currentLayer->getSRID());

  te::qt::widgets::Config2DrawPolygons(canvas, QColor(0, 0, 0, 0), QColor(0, 255, 0, 255), 4);

  canvas->draw(newGeom);

  m_mapDisplay->repaint();

  delete newGeom;
  delete canvas;
}