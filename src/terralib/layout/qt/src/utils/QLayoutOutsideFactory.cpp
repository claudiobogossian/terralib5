#include "QLayoutOutsideFactory.h"
#include "LayoutAbstractType.h"
#include "LayoutOutsideParamsCreate.h"
#include "QToolbarWindowOutside.h"
#include "QPropertiesWindowOutside.h"
#include "QDisplayWindowOutside.h"
#include "QObjectInspectorWindowOutside.h"

te::layout::QLayoutOutsideFactory::QLayoutOutsideFactory()
{

}

te::layout::QLayoutOutsideFactory::~QLayoutOutsideFactory()
{

}

te::layout::LayoutOutsideObserver* te::layout::QLayoutOutsideFactory::make( LayoutAbstractType type, LayoutOutsideParamsCreate params /*= LayoutItemParamsCreate()*/ )
{
	LayoutOutsideObserver* outside = 0;

	switch (type)
	{

	case TPToolbar:
		{
			QToolbarWindowOutside* toolbar = new QToolbarWindowOutside(params.getController(), params.getModel());		
			outside = (LayoutOutsideObserver*)toolbar;
			return outside;
			break;
		}
	case TPPropertiesWindow:
		{
			QPropertiesWindowOutside* window = new QPropertiesWindowOutside(params.getController(), params.getModel());		
			outside = (LayoutOutsideObserver*)window;
			return outside;
			break;
		}
  case TPObjectInspectorWindow :
    {
      QObjectInspectorWindowOutside* inspector = new QObjectInspectorWindowOutside(params.getController(), params.getModel());		
      outside = (LayoutOutsideObserver*)inspector;
      return outside;
      break;
    }
  case TPDisplayWindow:
    {
      QDisplayWindowOutside* displayWindow = new QDisplayWindowOutside(params.getController(), params.getModel());		
      outside = (LayoutOutsideObserver*)displayWindow;
      return outside;
      break;
    }
	}

	return 0;
}
