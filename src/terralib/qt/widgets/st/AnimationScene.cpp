// TerraLib
#include "AnimationScene.h"
#include "TrajectoryItem.h"
#include "Animation.h"
#include "AnimationItem.h"
#include "../canvas/MapDisplay.h"

// Qt
#include <QThread>
#include <QGraphicsView>

te::qt::widgets::AnimationScene::AnimationScene(te::qt::widgets::MapDisplay* display, QObject* parent)
  : QGraphicsScene(parent),
  m_display(display),
  m_trajectoryPixmap(0),
  m_numberOfTrajectories(0),
  m_numberOfCoverages(0)
{
  createNewPixmap();
}

te::qt::widgets::AnimationScene::~AnimationScene()
{
  delete m_trajectoryPixmap;
}

void te::qt::widgets::AnimationScene::createNewPixmap()
{
  bool createNew = false;

  if(m_trajectoryPixmap == 0)
      createNew = true;

  if(createNew == false && m_trajectoryPixmap)
  {
    if(m_trajectoryPixmap->rect().width() != m_display->width() || m_trajectoryPixmap->rect().height() != m_display->height())
      createNew = true;
  }

  if(createNew)
  {
    m_mutex.lock();
    delete m_trajectoryPixmap;
    m_trajectoryPixmap = new QPixmap(m_display->width(), m_display->height());
    m_trajectoryPixmap->fill(Qt::transparent);
    m_mutex.unlock();

    setSceneRect(0, 0, m_display->width(), m_display->height());  
    if(views().isEmpty() == false)
    {
      QGraphicsView* view = *(views().begin());
      view->resize(m_display->width(), m_display->height());
    }
  }
  else
  {
    m_mutex.lock();
    m_trajectoryPixmap->fill(Qt::transparent);
    m_mutex.unlock();
  }
}

void te::qt::widgets::AnimationScene::addItem(te::qt::widgets::AnimationItem* item)
{
  QGraphicsScene::addItem(item);
  if(item->pixmap().isNull())
    m_numberOfCoverages++;
  else
    m_numberOfTrajectories++;
}

void te::qt::widgets::AnimationScene::removeItem(te::qt::widgets::AnimationItem* item)
{
  QGraphicsScene::removeItem(item);
  if(item->pixmap().isNull())
    m_numberOfCoverages--;
  else
    m_numberOfTrajectories--;
}

void te::qt::widgets::AnimationScene::clear()
{
  QGraphicsScene::clear();
  m_numberOfCoverages = 0;
  m_numberOfTrajectories = 0;
}

void te::qt::widgets::AnimationScene::setDuration(const int& duration)
{
  QList<QGraphicsItem*> list = items();
  QList<QGraphicsItem*>::iterator it;
  for(it = list.begin(); it != list.end(); ++it)
  {
    AnimationItem* ai = dynamic_cast<AnimationItem*>(*it);
    ai->setDuration(duration);
  }
}

void te::qt::widgets::AnimationScene::draw(const int& curTime)
{
  QList<QGraphicsItem*> list = items();
  QList<QGraphicsItem*>::iterator it;
  for(it = list.begin(); it != list.end(); ++it)
  {
    AnimationItem* ai = dynamic_cast<AnimationItem*>(*it);
    ai->m_curTimeDuration = curTime;
    ai->draw();
  }
}
