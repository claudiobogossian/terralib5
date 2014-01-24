#ifndef LAYOUTSCENE_H
#define LAYOUTSCENE_H

namespace te
{
  namespace layout
  {
    class LayoutItem;
    class LayoutOutside;

    class LayoutScene
    {
      public:
        virtual ~LayoutScene(void) {}
        virtual void insertItem(LayoutItem* item) = 0;
        virtual void insertOutside(LayoutOutside* widget) = 0;
    };
  }
}

#endif