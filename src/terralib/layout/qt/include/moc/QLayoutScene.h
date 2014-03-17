#ifndef	QLAYOUTSCENE_H
#define QLAYOUTSCENE_H

#include <QGraphicsScene>
#include "LayoutScene.h"
#include <QTransform>
#include "LayoutAbstractType.h"

class QGraphicsSceneMouseEvent;
class QGraphicsItemGroup;
class QGraphicsProxyWidget;

namespace te
{
  namespace layout
  {
    class LayoutItemObserver;

    class QLayoutScene : public QGraphicsScene, public LayoutScene
    {
      Q_OBJECT //for slots/signals

      public:
        QLayoutScene(QWidget* widget = (QWidget*)0);
        ~QLayoutScene();

        virtual void insertItem(LayoutItemObserver* item);
        virtual void insertOutside(LayoutOutsideObserver* widget);
        virtual QGraphicsProxyWidget* insertOutsideProxy(LayoutOutsideObserver* widget);
        
        virtual te::gm::Envelope getSceneBox();

        QGraphicsItemGroup*	createItemGroup ( const QList<QGraphicsItem *> & items );
        void destroyItemGroup(QGraphicsItemGroup *group);

        /*
          params widthMM width of physical screen in millimeters
          params heightMM height of physical screen in millimeters
        */
        virtual void init(double widthMM, double heightMM, double dpix, double dpiy);

        virtual te::gm::Envelope getWorldBox();
        virtual te::gm::Envelope getPaperBox();
        
        virtual QTransform getMatrixViewScene();
        virtual QTransform getMatrixSceneItem();

        virtual QGraphicsItem* getMasterParentItem();

        virtual void printPaper();
        virtual void savePaperAsImage(std::string pathWithFileName);

      protected:
        virtual void mousePressEvent ( QGraphicsSceneMouseEvent* mouseEvent );

        virtual te::gm::Envelope calculateBoxPaper();
        virtual te::gm::Envelope calculateWindow();
        virtual void calculateMatrixViewScene();
        virtual void calculateMatrixSceneItem();
        virtual void createMasterParentItem();

      protected:

        double _paperSizeWMM;
        double _paperSizeHMM;
        LayoutAbstractPaperType _paperType;
        te::gm::Envelope _boxPaperW;
        te::gm::Envelope _boxW;
        QTransform _matrix;
        QTransform _matrixItem;
        double _logicalDpiX;
        double _logicalDpiY;
        double _screenWidthMM;
        double _screenHeightMM;
        QGraphicsItem* _masterParent;
    };
  }
}

#endif