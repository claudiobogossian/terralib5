#ifndef LAYOUT_OUTSIDE_PARAMS_CREATE_H
#define LAYOUT_OUTSIDE_PARAMS_CREATE_H

#include "LayoutParams.h"

namespace te
{
  namespace layout
  {
    class LayoutOutsideController;
    class LayoutOutsideModelObservable;

    class LayoutOutsideParamsCreate : public LayoutParams 
    {
    public:

      LayoutOutsideParamsCreate();
      LayoutOutsideParamsCreate(LayoutOutsideController* controller, LayoutOutsideModelObservable* model);
      virtual ~LayoutOutsideParamsCreate();

      LayoutOutsideController*	getController();
      LayoutOutsideModelObservable*		getModel();

    protected:
      LayoutOutsideController*	_controller;
      LayoutOutsideModelObservable*		_model;
    };

    /*
    ** ---------------------------------------------------------------
    ** Inline methods:
    */
    inline LayoutOutsideParamsCreate::LayoutOutsideParamsCreate(LayoutOutsideController* controller, LayoutOutsideModelObservable* model) :
	    _controller(controller),
	    _model(model)
    {
    }

    inline LayoutOutsideParamsCreate::~LayoutOutsideParamsCreate() 
    {
    }

    inline LayoutOutsideController* LayoutOutsideParamsCreate::getController()
    {
	    return _controller;
    }

    inline LayoutOutsideModelObservable* LayoutOutsideParamsCreate::getModel()
    {
	    return _model;
    }
  }
}

#endif