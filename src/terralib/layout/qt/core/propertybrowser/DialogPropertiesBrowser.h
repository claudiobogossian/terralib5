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
  \file DialogPropertiesBrowser.h
   
   \brief Manage properties dialogs values. 

  \ingroup layout
*/

#ifndef __TERRALIB_LAYOUT_INTERNAL_DIALOG_PROPERTIES_BROWSER_H 
#define __TERRALIB_LAYOUT_INTERNAL_DIALOG_PROPERTIES_BROWSER_H

// TerraLib
#include "AbstractPropertiesBrowser.h"
#include "../../../core/property/Property.h"
#include "../../../core/Config.h"

// STL
#include <map>
#include <string>

// Qt
#include <QObject>

// QtPropertyBrowser
class QtStringPropertyManager;
class QtDlgEditorFactory;
class QtProperty;

class QGraphicsItem;
class QWidget;

namespace te
{
  namespace layout
  {
    class Properties;
    class EnumDataType;
    class EnumType;

    /*!
    \brief Manage properties dialogs values. 
	  
	  \ingroup layout
	  */
    class TELAYOUTEXPORT DialogPropertiesBrowser : public AbstractPropertiesBrowser
    {
	    Q_OBJECT //for slots/signals

      public:

	      DialogPropertiesBrowser(QObject *parent = 0);

        DialogPropertiesBrowser(QtDlgEditorFactory* factory, QtStringPropertyManager* manager, QObject *parent = 0);

	      virtual ~DialogPropertiesBrowser();

        QtStringPropertyManager* getStringPropertyManager();

        QtDlgEditorFactory* getDlgEditorFactory();

        std::map<std::string, Property> getDlgProps();

        virtual void closeAllWindows();
        
        virtual bool changeQtPropertyValue(QtProperty* pproperty, Property property);

        virtual QtProperty* addProperty(Property property);

        virtual bool updateProperty(Property property);

        virtual bool checkDlgType(Property prop);

        virtual Property findDlgProperty(std::string name);

        virtual Property findDlgProperty(EnumType* dataType);
        
        virtual Property getProperty(std::string name);

        virtual EnumType* getLayoutType(QVariant::Type type, std::string name = "");

        virtual int getVariantType(EnumType* dataType);
            
      protected slots:

        /*
          \brief By default it is connected with the internalDlg method of the class QtDlgEditorFactory. 
            The internalDlg method is called when an item of property browser tree is clicked.
        */

        virtual void onSetDlg(QWidget *parent, QtProperty * prop);

        virtual void updateOutside(Property prop);

        virtual void onShowGridSettingsDlg();

        virtual void onShowImageDlg();

        virtual void onShowTextGridSettingsDlg();

      signals:

        void changeDlgProperty(Property property);

      protected:

        virtual void createManager();

        virtual void changeValueQtPropertyDlg(std::string name, QVariant variant);

        virtual QWidget* createOutside(EnumType* enumType);
      
      protected:

        QtStringPropertyManager*        m_strDlgManager;
        QtDlgEditorFactory*             m_dlgEditorFactory;
        std::map<std::string, Property> m_dlgProps;
        Property                        m_currentPropertyClicked;
    };
  }
}

#endif



