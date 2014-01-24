#ifndef LAYOUT_ITEM_PARAMS_CREATE_H
#define LAYOUT_ITEM_PARAMS_CREATE_H

#include "LayoutParams.h"

namespace te
{
  namespace layout
  {
    class LayoutItemController;
    class LayoutObservable;

    class LayoutItemParamsCreate : public LayoutParams 
    {
      public:
        LayoutItemParamsCreate();
        LayoutItemParamsCreate(LayoutItemController* controller, LayoutObservable* model);
        virtual ~LayoutItemParamsCreate();

        LayoutItemController*	getController();
        LayoutObservable*		getModel();

        protected:
        LayoutItemController*	_controller;
        LayoutObservable*		_model;
    };

    /*
    ** ---------------------------------------------------------------
    ** Inline methods:
    */
    inline LayoutItemParamsCreate::LayoutItemParamsCreate(LayoutItemController* controller, LayoutObservable* model) :
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

    inline LayoutObservable* LayoutItemParamsCreate::getModel()
    {
      return _model;
    }
  }
}
#endif