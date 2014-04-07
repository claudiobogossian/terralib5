/*  Copyright (C) 2014-2014 National Institute For Space Research (INPE) - Brazil.

    This file is part of the TerraLib - a Framework for building GIS enabled applications.

    TerraLib is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation, either version 3 of the License,
    or (at your option) any later version.

    TerraLib is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with TerraLib. See COPYING. If not, write to
    TerraLib Team at <terralib-team@terralib.org>.
 */

/*!
  \file QLayoutOutsideFactory.cpp
   
  \brief 

  \ingroup layout
*/

// TerraLib
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

te::layout::LayoutObserver* te::layout::QLayoutOutsideFactory::make( LayoutAbstractObjectType type, LayoutOutsideParamsCreate params /*= LayoutItemParamsCreate()*/ )
{
	LayoutObserver* outside = 0;

	switch (type)
	{

	case TPToolbar:
		{
			QToolbarWindowOutside* toolbar = new QToolbarWindowOutside(params.getController(), params.getModel());		
			outside = (LayoutObserver*)toolbar;
			return outside;
			break;
		}
	case TPPropertiesWindow:
		{
			QPropertiesWindowOutside* window = new QPropertiesWindowOutside(params.getController(), params.getModel());		
			outside = (LayoutObserver*)window;
			return outside;
			break;
		}
  case TPObjectInspectorWindow :
    {
      QObjectInspectorWindowOutside* inspector = new QObjectInspectorWindowOutside(params.getController(), params.getModel());		
      outside = (LayoutObserver*)inspector;
      return outside;
      break;
    }
  case TPDisplayWindow:
    {
      QDisplayWindowOutside* displayWindow = new QDisplayWindowOutside(params.getController(), params.getModel());		
      outside = (LayoutObserver*)displayWindow;
      return outside;
      break;
    }
	}

	return 0;
}
