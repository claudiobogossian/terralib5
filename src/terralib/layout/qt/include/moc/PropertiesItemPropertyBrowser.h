/*  Copyright (C) 2001-2014 National Institute For Space Research (INPE) - Brazil.

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
  \file PropertyBrowser.h
   
  \brief 

  \ingroup layout
*/

#ifndef __TERRALIB_LAYOUT_INTERNAL_PROPERTIES_ITEM_PROPERTY_BROWSER_H 
#define __TERRALIB_LAYOUT_INTERNAL_PROPERTIES_ITEM_PROPERTY_BROWSER_H

// TerraLib
#include "PropertyBrowser.h"

class QGraphicsItem;
class QWidget;

class QtVariantProperty;
class QtProperty;
class QtVariantPropertyManager;
class QtBrowserIndex;

namespace te
{
  namespace layout
  {
    class GridSettingsOutside;

    class PropertiesItemPropertyBrowser : public PropertyBrowser
    {
	    Q_OBJECT //for slots/signals

    public:

	    PropertiesItemPropertyBrowser(QObject *parent = 0);
	    virtual ~PropertiesItemPropertyBrowser();
      
      virtual bool addProperty(Property property);

      virtual void clearAll();
           
      private slots:

        void onShowGridSettingsDlg();
        void onSetDlg(QWidget *parent, QtProperty * prop);
        void onUpdateGridSettingsProperty();

      signals:

        void updateOutside(Property property);
      
    protected:

      GridSettingsOutside* m_gridSettings;
      Property             m_dlgProperty;
    };
  }
}

#endif
