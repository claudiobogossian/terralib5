#ifndef LAYOUTVIEW_H
#define LAYOUTVIEW_H

namespace te
{
  namespace layout
  {
    class LayoutController;

    class LayoutView 
    {
      public:
        virtual ~LayoutView(void) {}
        virtual void changeMode() = 0;

      protected:
        LayoutController* _layoutController;
    };
  }
}

#endif