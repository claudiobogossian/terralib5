#ifndef LAYOUTITEMCONTROLLER_H
#define LAYOUTITEMCONTROLLER_H

namespace te
{
  namespace layout
  {
    class LayoutObservable;
    class LayoutObserver;

    class LayoutItemController 
    {
      public:

        LayoutItemController(LayoutObservable* o);
        virtual ~LayoutItemController();

        virtual void setPosition(const double& x, const double& y) = 0;
        virtual void redraw(const double& zoomFactor);
        const LayoutObservable* getModel();
        const LayoutObserver* getView();

      protected:
        LayoutObservable* _model;
        LayoutObserver* _view;
    };
  }
}

#endif