
//TerraLib
#include "../../../common/STLUtils.h"
#include "../../../geometry.h"
#include "../../../dataaccess/dataset/ObjectId.h"
#include "../../../dataaccess/dataset/ObjectIdSet.h"
#include "../../../dataaccess/utils/Utils.h"
#include "../../../qt/af/events/LayerEvents.h"
#include "../../../qt/af/events/MapEvents.h"
#include "../../../qt/widgets/canvas/MapDisplay.h"
#include "../../../qt/widgets/Utils.h"
#include "../../Feature.h"
#include "../../RepositoryManager.h"
#include "../../Utils.h"
#include "../Renderer.h"
#include "../Utils.h"
#include "../core/command/UpdateCommand.h"
#include "EditInfoTool.h"

// Qt
#include <QMessageBox>
#include <QMouseEvent>
#include <QPainter>
#include <QPixmap>
#include <QDebug>

// STL
#include <cassert>
#include <memory>
#include <iostream>

te::edit::EditInfoTool::EditInfoTool(te::qt::widgets::MapDisplay* display, const te::map::AbstractLayerPtr& layer, QObject* parent)
: GeometriesUpdateTool(display, layer.get(), parent),
  m_updateWatches(0) ,
  m_infoWidget(new QTreeWidget(display))
{
  // Signals & slots
  //connect(m_display, SIGNAL(extentChanged()), SLOT(onExtentChanged()));

  // Setup the widget that will be used to show the informations
  m_infoWidget->setWindowTitle(tr("Information"));
  m_infoWidget->setWindowFlags(Qt::Tool);
  m_infoWidget->setAlternatingRowColors(true);
  m_infoWidget->setMinimumSize(300, 300);
  m_infoWidget->setColumnCount(2);

  QStringList labels;
  labels << tr("Property") << tr("Value");
  m_infoWidget->setHeaderLabels(labels);

  connect(m_infoWidget, SIGNAL(itemDoubleClicked(QTreeWidgetItem*, int)), this, SLOT(onAttributesTreeWidgetItemDoubleClicked(QTreeWidgetItem*, int)));
}

te::edit::EditInfoTool::~EditInfoTool()
{
  reset();

  m_infoWidget->close();

  delete m_infoWidget;
}

bool te::edit::EditInfoTool::mouseReleaseEvent(QMouseEvent* e)
{
  if (e->button() != Qt::LeftButton)
    return false;

  // Clear info widget!
  m_infoWidget->clear();

  pickFeature(m_layer, GetPosition(e));

  // Try finds the geometry centroid
  te::gm::Coord2D coord = m_feature->getGeometry()->getMBR()->getCenter();

  // Build the search envelope
  te::gm::Envelope env(coord.getX(), coord.getY(), coord.getX(), coord.getY());

  // Retrieves the data from layer
  //std::auto_ptr<te::da::DataSet> m_dataset(m_layer->getData(t, m_feature->getGeometry()->getEnvelope()/*&buildEnvelope(GetPosition(e))*/, te::gm::INTERSECTS).release());

  m_dataset = m_layer->getData(te::da::GetFirstGeomProperty(m_layer->getSchema().get())->getName(), &env/*&buildEnvelope(GetPosition(e))*/, te::gm::INTERSECTS).release();

  if (m_feature)
    getInfo(env);

  m_infoWidget->show();

  return true;
}

void te::edit::EditInfoTool::draw()
{
  const te::gm::Envelope& env = m_display->getExtent();
  if (!env.isValid())
    return;

  // Clear!
  QPixmap* draft = m_display->getDraftPixmap();
  draft->fill(Qt::transparent);

  // Initialize the renderer
  Renderer& renderer = Renderer::getInstance();
  renderer.begin(draft, env, m_display->getSRID());

  // Draw the layer edited geometries
  renderer.drawRepository(m_layer->getId(), env, m_display->getSRID());

  renderer.end();

  m_display->repaint();

}


te::gm::Envelope te::edit::EditInfoTool::buildEnvelope(const QPointF& pos)
{
  QPointF pixelOffset(4.0, 4.0);

  QRectF rect(pos - pixelOffset, pos + pixelOffset);

  // Converts rect boundary to world coordinates
  QPointF ll(rect.left(), rect.bottom());
  QPointF ur(rect.right(), rect.top());
  ll = m_display->transform(ll);
  ur = m_display->transform(ur);

  te::gm::Envelope env(ll.x(), ll.y(), ur.x(), ur.y());

  return env;
}

void te::edit::EditInfoTool::reset()
{
  delete m_feature;
  m_feature = 0;
}

void te::edit::EditInfoTool::storeEditedFeature()
{
  RepositoryManager::getInstance().addFeature(m_layer->getId(), m_feature->clone());
}

void te::edit::EditInfoTool::storeUndoCommand()
{
  m_updateWatches.push_back(m_feature->clone());

  QUndoCommand* command = new UpdateCommand(m_updateWatches, m_display, m_layer);
  UndoStackManager::getInstance().addUndoStack(command);
}

void te::edit::EditInfoTool::pickFeature(const te::map::AbstractLayerPtr& layer, const QPointF& pos)
{
  reset();

  te::gm::Envelope env = buildEnvelope(pos);

  try
  {
    m_feature = PickFeature(layer, env, m_display->getSRID(), te::edit::GEOMETRY_UPDATE);

  }
  catch (std::exception& e)
  {
    QMessageBox::critical(m_display, tr("Error"), QString(tr("The geometry cannot be selected from the layer. Details:") + " %1.").arg(e.what()));
  }
}

void te::edit::EditInfoTool::getInfo(const te::gm::Envelope& e)
{
  // Get the geometry property position
  m_restrictivePropertyPos.push_back(te::da::GetFirstSpatialPropertyPos(m_dataset));

  if (m_feature->getData().size() == 0)
  {
    // Generates a geometry from the given extent. It will be used to refine the results
    std::auto_ptr<te::gm::Geometry> geometryFromEnvelope(te::gm::GetGeomFromEnvelope(&e, m_layer->getSRID()));

    // The restriction point. It will be used to refine the results
    te::gm::Coord2D center = m_feature->getGeometry()->getMBR()->getCenter();
    te::gm::Point point(center.x, center.y, m_layer->getSRID());

    std::size_t gpos;
    gpos = te::da::GetPropertyPos(m_dataset, te::da::GetFirstGeomProperty(m_layer->getSchema().get())->getName());

    // Fills the QTreeWidgetItem
    while (m_dataset->moveNext())
    {
      std::auto_ptr<te::gm::Geometry> g(m_dataset->getGeometry(gpos));
      g->setSRID(m_layer->getSRID());

      if (g->contains(&point) || g->crosses(geometryFromEnvelope.get()) || geometryFromEnvelope->contains(g.get()))
      {
        for (std::size_t i = 0; i < m_dataset->getNumProperties(); ++i)
        {
          QTreeWidgetItem* propertyItem = new QTreeWidgetItem;

          int pos = te::da::GetPropertyPos(m_layer->getSchema().get(), m_dataset->getPropertyName(i));
          propertyItem->setText(0, m_dataset->getPropertyName(pos).c_str());

          if (m_dataset->getPropertyDataType(pos) == te::dt::GEOMETRY_TYPE)
            propertyItem->setIcon(0, QIcon::fromTheme("geometry"));

          if (!m_dataset->isNull(pos))
          {
            QString qvalue;

            if (m_dataset->getPropertyDataType(pos) == te::dt::STRING_TYPE)
            {
              std::string value = m_dataset->getString(pos);
              te::common::CharEncoding encoding = m_dataset->getPropertyCharEncoding(pos);
              qvalue = te::qt::widgets::Convert2Qt(value, encoding);
            }
            else
              qvalue = m_dataset->getAsString(pos, 3).c_str();

            propertyItem->setText(1, qvalue);
          }
          else // property null value!
            propertyItem->setText(1, "");

          m_infoWidget->addTopLevelItem(propertyItem);

          // fill the m_data
          std::auto_ptr<te::dt::AbstractData> data(m_dataset->getValue(pos));
          m_data[pos] = data.release();
        }
      }
    }
  }
  else
  {
    while (m_dataset->moveNext())
    {
      std::map<std::size_t, te::dt::AbstractData* > ::iterator it;

      for (std::size_t i = 0; i < m_dataset->getNumProperties(); ++i)
      {
        QTreeWidgetItem* propertyItem = new QTreeWidgetItem;

        int pos = te::da::GetPropertyPos(m_layer->getSchema().get(), m_dataset->getPropertyName(i));
        propertyItem->setText(0, m_dataset->getPropertyName(pos).c_str());

        it = m_data.find(pos);
        if (it != m_data.end())
          propertyItem->setText(1, QString(it->second->toString().c_str()));
        else
        {
          if (m_dataset->getPropertyDataType(pos) == te::dt::GEOMETRY_TYPE)
            propertyItem->setIcon(0, QIcon::fromTheme("geometry"));

          propertyItem->setText(1, QString(m_dataset->getAsString(pos, 3).c_str()));

        }

        m_infoWidget->addTopLevelItem(propertyItem);
      }
    }
  }
}

// Hack from http://stackoverflow.com/a/13374558 to making only one column of a QTreeWidgetItem editable
void te::edit::EditInfoTool::onAttributesTreeWidgetItemDoubleClicked(QTreeWidgetItem* item, int column)
{

  bool isrestrictive = false;

  for (std::size_t i = 0; i < m_restrictivePropertyPos.size(); i++)
  {
    if ((int)m_restrictivePropertyPos[i] == m_infoWidget->currentIndex().row())
    {
      isrestrictive = true;
      break;
    }
  }

  Qt::ItemFlags tmp = item->flags();
  if (column == 1 && !isrestrictive)
    item->setFlags(tmp | Qt::ItemIsEditable);
  else if (tmp & Qt::ItemIsEditable)
    item->setFlags(tmp ^ Qt::ItemIsEditable);

}
