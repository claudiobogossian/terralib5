#ifndef LAYOUTSCENE_H
#define LAYOUTSCENE_H

#include "../../../../geometry/Envelope.h"

namespace te
{
  namespace layout
  {
    class LayoutItemObserver;
    class LayoutOutsideObserver;

    class LayoutScene
    {
      public:
        virtual ~LayoutScene(void) {};
        virtual void insertItem(LayoutItemObserver* item) = 0;
        virtual void insertOutside(LayoutOutsideObserver* widget) = 0;
        virtual te::gm::Envelope getSceneBox() = 0;
        virtual te::gm::Envelope getWorldBox() = 0;
        virtual te::gm::Envelope getPaperBox() = 0;
        /*
          params widthMM width of physical screen in millimeters
          params heightMM height of physical screen in millimeters
        */
        virtual void init(double widthMM, double heightMM, double dpix, double dpiy) = 0;

    protected:
        
        virtual te::gm::Envelope calculateBoxPaper() = 0;
        virtual te::gm::Envelope calculateWindow() = 0;
        virtual void calculateMatrixViewScene() = 0;
        virtual void calculateMatrixSceneItem() = 0;
        virtual void createMasterParentItem() = 0;
        
    };
  }
}

#endif