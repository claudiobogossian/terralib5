#ifndef LAYOUTOUTSIDECONTROLLER_H
#define LAYOUTOUTSIDECONTROLLER_H

namespace te
{
  namespace layout
  {
    class LayoutOutsideModelObservable;
    class LayoutOutsideObserver;

    class LayoutOutsideController 
    {
    public:

      LayoutOutsideController(LayoutOutsideModelObservable* o);
      virtual ~LayoutOutsideController();

      virtual void setPosition(const double& x, const double& y) = 0;
      const LayoutOutsideModelObservable* getModel();
      const LayoutOutsideObserver* getView();

    protected:
      LayoutOutsideModelObservable* _model;
      LayoutOutsideObserver* _view;
    };
  }
}

#endif