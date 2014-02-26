#include "QMapLayoutItem.h"
#include "LayoutContext.h"
#include "LayoutScene.h"
#include <QCursor>
#include <QPixmap>
#include <QMessageBox>
#include <QWidget>
#include <QGraphicsProxyWidget>

#include "../../../color/RGBAColor.h"
#include "../../../../qt/widgets/Utils.h"
#include "../../../../geometry/Envelope.h"
#include "../../../../common/STLUtils.h"
#include "../../../../qt/widgets/canvas/Canvas.h"
#include "../../../../dataaccess/utils/Utils.h"
#include "../../../../dataaccess/dataset/ObjectIdSet.h"
#include "../../../../qt/widgets/canvas/MultiThreadMapDisplay.h"
#include "../../../../maptools/AbstractLayer.h"

#include "../../../../qt/af/ApplicationController.h"
#include "../../../../qt/af/Project.h"
#include "../../../../qt/af/Utils.h"
#include "../../../../qt/af/events/LayerEvents.h"
#include "LayoutItemModelObservable.h"
#include "LayoutItemObserver.h"
#include "LayoutItemController.h"

te::layout::QMapLayoutItem::QMapLayoutItem( LayoutItemController* controller, LayoutItemModelObservable* o ) :
  QObjectLayoutItem(controller, o),
  m_mapDisplay(0)
{
  this->setFlags(QGraphicsItem::ItemIsMovable
    | QGraphicsItem::ItemIsSelectable
    | QGraphicsItem::ItemSendsGeometryChanges);

  QGraphicsItem* item = this;
  LayoutContext::getInstance()->getScene()->insertItem((LayoutItemObserver*)item);

  _proxyWidgetDisplay = new QGraphicsProxyWidget(this);

  m_mapDisplay = new te::qt::widgets::MultiThreadMapDisplay(QSize(30, 20), true);
  m_mapDisplay->setAcceptDrops(true);
  m_mapDisplay->setBackgroundColor(Qt::gray);
  m_mapDisplay->setResizeInterval(0);
  m_mapDisplay->setMouseTracking(true);
  m_mapDisplay->installEventFilter(this);
  
  _proxyWidgetDisplay->setWidget(m_mapDisplay);

  m_mapDisplay->show();
  connect(m_mapDisplay, SIGNAL(drawLayersFinished(const QMap<QString, QString>&)), this, SLOT(onDrawLayersFinished(const QMap<QString, QString>&)));
}

te::layout::QMapLayoutItem::~QMapLayoutItem()
{

}

void te::layout::QMapLayoutItem::updateObserver( ContextLayoutItem context )
{
  te::color::RGBAColor** rgba = context.getPixmap();

  LayoutItemModelObservable* model = (LayoutItemModelObservable*)_controller->getModel();
  LayoutUtils* utils = LayoutContext::getInstance()->getUtils();
  te::map::Canvas* canvas = LayoutContext::getInstance()->getCanvas();

  te::gm::Envelope box = utils->viewportBox(canvas, model->getBox());

  QPixmap pixmap;
  QImage* img = 0;

  if(rgba)
  {
    img = te::qt::widgets::GetImage(rgba, box.getWidth(), box.getHeight());
    pixmap = QPixmap::fromImage(*img);
  }

  te::common::Free(rgba, box.getHeight());
  if(img)
    delete img;

  setPixmap(pixmap);
  update();
}


void te::layout::QMapLayoutItem::mouseDoubleClickEvent( QGraphicsSceneMouseEvent * event )
{
  _controller->redraw(1.);
}

QVariant te::layout::QMapLayoutItem::itemChange( GraphicsItemChange change, const QVariant &value )
{
  return QGraphicsItem::itemChange(change, value);
}

void te::layout::QMapLayoutItem::onDrawLayersFinished(const QMap<QString, QString>& /*errors*/)
{
  // Stores the clean pixmap!
  m_lastDisplayContent = QPixmap(*m_mapDisplay->getDisplayPixmap());
  drawLayerSelection();
}

void te::layout::QMapLayoutItem::drawLayerSelection()
{
  std::list<te::map::AbstractLayerPtr>::iterator it;
  std::list<te::map::AbstractLayerPtr> layers;
  
  //layers = te::qt::af::ApplicationController::getInstance().getProject()->getSelectedLayers();
  
  for(it = layers.begin(); it != layers.end(); ++it)
  {
    te::map::AbstractLayer* layer = (*it).get();
    {
      if(layer->getVisibility() != te::map::VISIBLE)
        continue;

      const te::da::ObjectIdSet* oids = layer->getSelected();
      if(oids == 0 || oids->size() == 0)
      {
        m_mapDisplay->repaint();
        return;
      }

      bool needRemap = false;

      if((layer->getSRID() != TE_UNKNOWN_SRS) && (m_mapDisplay->getSRID() != TE_UNKNOWN_SRS) && (layer->getSRID() != m_mapDisplay->getSRID()))
        needRemap = true;

      // Try retrieves the layer selection
      std::auto_ptr<te::da::DataSet> selected;
      try
      {
        selected = layer->getData(oids);
      }
      catch(std::exception& e)
      {
        QMessageBox::critical(m_mapDisplay, tr("Error"), QString(tr("The layer selection cannot be drawn. Details:") + " %1.").arg(e.what()));
        return;
      }

      std::size_t gpos = te::da::GetFirstPropertyPos(selected.get(), te::dt::GEOMETRY_TYPE);

      QPixmap* content = m_mapDisplay->getDisplayPixmap();

      const te::gm::Envelope& displayExtent = m_mapDisplay->getExtent();

      te::qt::widgets::Canvas canvas(content);
      canvas.setWindow(displayExtent.m_llx, displayExtent.m_lly, displayExtent.m_urx, displayExtent.m_ury);

      te::gm::GeomType currentGeomType = te::gm::UnknownGeometryType;

      while(selected->moveNext())
      {
        std::auto_ptr<te::gm::Geometry> g(selected->getGeometry(gpos));

        if(needRemap)
        {
          g->setSRID(layer->getSRID());
          g->transform(m_mapDisplay->getSRID());
        }

        if(currentGeomType != g->getGeomTypeId())
        {
          currentGeomType = g->getGeomTypeId();
          te::qt::widgets::Config2DrawLayerSelection(&canvas, m_selectionColor, currentGeomType);
        }

        canvas.draw(g.get());
      }
    }
  }
  m_mapDisplay->repaint();
}
