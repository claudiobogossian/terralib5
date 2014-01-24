#ifndef	QLAYOUTSCENE_H
#define QLAYOUTSCENE_H

#include <QGraphicsScene>
#include "LayoutScene.h"

class QGraphicsSceneMouseEvent;

namespace te
{
  namespace layout
  {
    class QLayoutScene : public QGraphicsScene, LayoutScene
    {
      Q_OBJECT //for slots/signals

      public:
        QLayoutScene(QWidget* widget = (QWidget*)0);
        ~QLayoutScene();

        virtual void insertItem(LayoutItem* item);

        virtual void insertOutside(LayoutOutside* widget);

      protected:
        virtual void mousePressEvent ( QGraphicsSceneMouseEvent* mouseEvent );
    };
  }
}

#endif