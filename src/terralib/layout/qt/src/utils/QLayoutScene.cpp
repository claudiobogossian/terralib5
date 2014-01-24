#include <QLayoutScene.h>

#include <QGraphicsScene.h>
#include <QGraphicsPixmapItem>
#include <QGraphicsItem>
#include <QtGui>
#include "LayoutItem.h"
#include <QWidget>
#include "LayoutContext.h"
#include "LayoutMode.h"

te::layout::QLayoutScene::QLayoutScene( QWidget* widget): 
  QGraphicsScene(widget)
{

}

te::layout::QLayoutScene::~QLayoutScene()
{

}

void te::layout::QLayoutScene::insertItem( LayoutItem* item )
{
  QGraphicsItem* qitem = ((QGraphicsItem*)item);
  this->addItem(qitem);
}

void te::layout::QLayoutScene::insertOutside( LayoutOutside* widget )
{
  QWidget* qWidget = ((QWidget*)widget);
  this->addWidget(qWidget);
}

void te::layout::QLayoutScene::mousePressEvent ( QGraphicsSceneMouseEvent * mouseEvent )
{
  QGraphicsScene::mousePressEvent(mouseEvent);
}
