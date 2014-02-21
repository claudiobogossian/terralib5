#ifndef LAYOUT_ITEM_PARAMS_CREATE_H
#define LAYOUT_ITEM_PARAMS_CREATE_H

#include "LayoutParams.h"

namespace te
{
  namespace layout
  {
    class LayoutItemController;
    class LayoutItemModelObservable;

    class LayoutItemParamsCreate : public LayoutParams 
    {
      public:
        LayoutItemParamsCreate();
        LayoutItemParamsCreate(LayoutItemController* controller, LayoutItemModelObservable* model);
        virtual ~LayoutItemParamsCreate();

        LayoutItemController*	getController();
        LayoutItemModelObservable*		getModel();

        protected:
        LayoutItemController*	_controller;
        LayoutItemModelObservable*		_model;
    };

    /*
    ** ---------------------------------------------------------------
    ** Inline methods:
    */
    inline LayoutItemParamsCreate::LayoutItemParamsCreate(LayoutItemController* controller, LayoutItemModelObservable* model) :
    _controller(controller),
    _model(model)
    {
    }

    inline LayoutItemParamsCreate::~LayoutItemParamsCreate() 
    {
    }

    inline LayoutItemController* LayoutItemParamsCreate::getController()
    {
      return _controller;
    }

    inline LayoutItemModelObservable* LayoutItemParamsCreate::getModel()
    {
      return _model;
    }
  }
}
#endif