#ifndef LAYOUTCONTROLLER_H
#define LAYOUTCONTROLLER_H

namespace te
{
  namespace layout
  {
    class LayoutController 
    {
      public:
        virtual ~LayoutController(void) {}
        virtual void createNewItem(const double& centerX, const double& centerY) = 0;
    };
  }
}

#endif