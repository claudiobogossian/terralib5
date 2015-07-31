#include "LayerViewMenuManager.h"

#include "LayerItemView.h"
#include "LayerItem.h"
#include "../../../../common/GenericQueue.h"

// TerraLib
#include "../../../../maptools/AbstractLayer.h"

// Qt
#include <QContextMenuEvent>
#include <QMenu>


void GetMenu(QMenu* mnu, te::qt::widgets::QueueAction* acts, te::qt::widgets::QueueAction* allActs)
{
  if(acts != 0 && acts->getSize() == 0 && allActs != 0 && allActs->getSize() == 0)
    return;

  unsigned int s = (allActs != 0) ? allActs->getSize() : 0;

  for(unsigned int i = 0; i < s; i++)
    mnu->addAction(allActs->getValue(i));

  if(acts == 0)
    return;

  if(s > 0)
    mnu->addSeparator();

  s = acts->getSize();

  for(unsigned int i = 0; i < s; i++)
    mnu->addAction(acts->getValue(i));
}

bool IsRasterLayer(te::qt::widgets::TreeItem* item)
{
  te::map::AbstractLayerPtr layer = (static_cast<te::qt::widgets::LayerItem*>(item))->getLayer();

  return layer->getSchema()->hasRaster();
}

bool IsValidLayer(te::qt::widgets::TreeItem* item)
{
  te::map::AbstractLayerPtr layer = (static_cast<te::qt::widgets::LayerItem*>(item))->getLayer();

  return layer->isValid();
}

te::qt::widgets::LayerViewMenuManager::LayerViewMenuManager(LayerItemView* view):
  QObject(view),
  m_view(view)
{
  m_VL_actions.reset(new QueueAction);
  m_RL_actions.reset(new QueueAction);
  m_FL_actions.reset(new QueueAction);
  m_ML_actions.reset(new QueueAction);
  m_NL_actions.reset(new QueueAction);
  m_AL_actions.reset(new QueueAction);
  m_IL_actions.reset(new QueueAction);
}

te::qt::widgets::LayerViewMenuManager::~LayerViewMenuManager()
{
}

void te::qt::widgets::LayerViewMenuManager::addAction(LayerViewMenuManager::LMENUACTIONS mnu, QAction* act, int pos)
{
  QueueAction* q;

  switch(mnu)
  {
    case VECTOR_LAYER:
      q = m_VL_actions.get();
      break;

    case RASTER_LAYER:
      q = m_RL_actions.get();
      break;

    case FOLDER_LAYER:
      q = m_FL_actions.get();
      break;

    case MULTI_LAYERS:
      q = m_ML_actions.get();
      break;

    case ALL_LAYERS:
      q = m_AL_actions.get();
      break;

    case INVALID_LAYERS:
      q = m_IL_actions.get();
      break;

    default:
      q = m_NL_actions.get();
      break;
  }

  if(pos == -1)
    q->add(act);
  else
  {
    try
    {
      q->insert(act, (unsigned int) pos);
    }
    catch(te::common::Exception&)
    {
      q->add(act);
    }
  }
}

bool te::qt::widgets::LayerViewMenuManager::eventFilter(QObject* watched, QEvent* event)
{
  switch(event->type())
  {
    case QEvent::ContextMenu:
    {
      QContextMenuEvent* evt = static_cast<QContextMenuEvent*>(event);
      QPoint pos = evt->globalPos();

      int rows = m_view->model()->rowCount();
      QModelIndex idx = m_view->indexAt(m_view->viewport()->mapFromGlobal(pos));

      if(rows == 0) // List is empty
      {
        QMenu mnu;
        GetMenu(&mnu, m_NL_actions.get(), 0);

        mnu.exec(pos);
      }
      else         // List not empty
      {
        QModelIndexList ls = m_view->selectionModel()->selectedIndexes();
        int si = ls.size();

        if(!idx.isValid())
        {
          QMenu mnu;

          GetMenu(&mnu, (si > 1) ? m_ML_actions.get() : m_NL_actions.get(), 0);
          mnu.exec(pos);
        }
        else
        {
          QModelIndexList ls = m_view->selectionModel()->selectedIndexes();

          if(ls.isEmpty())
            return true;

          QMenu mnu;
          int si = ls.size();

          if(si > 1)
            GetMenu(&mnu, m_ML_actions.get(), m_AL_actions.get());
          else
          {
            TreeItem* item = static_cast<TreeItem*>(ls.at(0).internalPointer());

            if(item->getType() == "FOLDER")
              GetMenu(&mnu, m_FL_actions.get(), m_AL_actions.get());
            else if(item->getType() == "LAYER" )
              GetMenu(&mnu, (!IsValidLayer(item)) ? m_IL_actions.get() : (IsRasterLayer(item)) ? m_RL_actions.get() : m_VL_actions.get(), m_AL_actions.get());
            else
              GetMenu(&mnu, 0, m_AL_actions.get());
          }

          mnu.exec(pos);
        }
      }
    }
      break;

    default:
      break;
  }

  return QObject::eventFilter(watched, event);
}
