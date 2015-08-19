#include "WMSLayerPopupHandler.h"

// TerraLib
#include "../../../../maptools/AbstractLayer.h"

#include "../../../widgets/layer/explorer/LayerItem.h"
#include "../../../widgets/layer/explorer/LayerItemView.h"

// Qt
#include <QAction>
#include <QContextMenuEvent>
#include <QMenu>

te::qt::widgets::TreeItem* GetPointedItem(QTreeView* view, QEvent* evt, QPoint& pos)
{
  switch(evt->type())
  {
    case QEvent::ContextMenu:
    {
      QContextMenuEvent* e = static_cast<QContextMenuEvent*>(evt);
      pos = e->globalPos();
    }
      break;

    case QEvent::MouseButtonDblClick:
    {
      QMouseEvent* e = static_cast<QMouseEvent*>(evt);
      pos = e->globalPos();

      throw;
    }
      break;

    default:
      return 0;
  }

  if(pos.isNull())
    return 0;

  QModelIndex idx = view->indexAt(view->viewport()->mapFromGlobal(pos));

  if(idx.isValid())
    return static_cast<te::qt::widgets::TreeItem*>(idx.internalPointer());

  return 0;
}


te::qt::plugins::wms::WMSLayerPopupHandler::WMSLayerPopupHandler(QObject* decorated, QObject* parent) :
  te::common::Decorator<QObject>(decorated)
{
  setParent(parent);
}


te::qt::plugins::wms::WMSLayerPopupHandler::~WMSLayerPopupHandler()
{

}


bool te::qt::plugins::wms::WMSLayerPopupHandler::eventFilter(QObject* watched, QEvent* event)
{
  te::qt::widgets::LayerItemView* view = dynamic_cast<te::qt::widgets::LayerItemView*>(watched);

  if(view == 0)
    return false;

  QModelIndexList ls = view->selectionModel()->selectedIndexes();

  if(ls.size() == 1)
  {
    switch(event->type())
    {
      case QEvent::ContextMenu:
      {
        QPoint pos;

        te::qt::widgets::TreeItem* item = GetPointedItem(view, event, pos);

        if(item != 0)
        {

          if(item->getType() == "LAYER")
          {
            te::map::AbstractLayerPtr l = ((te::qt::widgets::LayerItem*)item)->getLayer();

            if(l->getType() == "WMSLAYER")
            {
              QMenu mnu;

              QAction* act = new QAction(tr("OK! WMS LAYER"), &mnu);

              mnu.addAction(act);

              mnu.exec(pos);

              return false;
            }
          }
        }
      }
        break;

      case QEvent::MouseButtonDblClick :
      {
        throw;

        QPoint pos;

        te::qt::widgets::TreeItem* item = GetPointedItem(view, event, pos);

        if(item != 0)
        {
          if(item->getType() == "COLORMAP")
          {
            te::map::AbstractLayerPtr l = ((te::qt::widgets::LayerItem*)item->getParent())->getLayer();

            if(l->getType() == "WMSLAYER")
              return true;
          }
        }
      }
        break;

      default:
        break;
    }
  }

  if(m_decorated != 0)
    return m_decorated->eventFilter(watched, event);
  else
    return QObject::eventFilter(watched, event);
}
