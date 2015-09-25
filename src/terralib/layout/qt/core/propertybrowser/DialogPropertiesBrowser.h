/*  Copyright (C) 2008 National Institute For Space Research (INPE) - Brazil.

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
#include "../../../core/Config.h"
#include "../../../core/property/Property.h"

// STL
#include <map>
#include <string>
#include <vector>

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
        
        virtual bool changeQtPropertyValue(QtProperty* pproperty, const Property& property);

        virtual QtProperty* addProperty(const Property& property);

        virtual bool updateProperty(const Property& property);

        virtual bool checkDlgType(const Property& prop);

        virtual Property findDlgProperty(const std::string& name);

        virtual Property findDlgProperty(EnumType* dataType);
        
        virtual Property getProperty(const std::string& name);

        virtual EnumType* getLayoutType(QVariant::Type type, const std::string& name = "");

        virtual int getVariantType(EnumType* dataType);

        virtual void clearAll();
            
      protected slots:

         /*
          \brief Call when one of the Windows was close.
        */
        virtual void onDestroyed(QObject* obj = 0);

        /*
          \brief By default it is connected with the internalDlg method of the class QtDlgEditorFactory. 
            The internalDlg method is called when an item of property browser tree is clicked.
        */

        virtual void onSetDlg(QWidget *parent, QtProperty * prop);

        virtual void updateOutside(const Property& prop);

        virtual void updateOutside(const std::vector<Property>& props);

        virtual void onShowGridSettingsDlg();

        virtual void onShowImageDlg();

        virtual void onShowTextGridSettingsDlg();

        virtual void onShowFontDlg();

        virtual void onShowColorDlg();

        virtual void onShowMapLayerChoiceDlg();

        virtual void onShowLegendChoiceDlg();

        virtual void onShowViewDlg();

      signals:

        void changeDlgProperty(Property property);

        void changeDlgProperty(std::vector<Property> props);

      protected:

        virtual void createManager();

        virtual void changeValueQtPropertyDlg(const std::string& name, const QVariant& variant);

        virtual QWidget* createOutside(EnumType* enumType);

        virtual void appendDialog(QWidget* widget);
      
      protected:

        QtStringPropertyManager*        m_strDlgManager;
        QtDlgEditorFactory*             m_dlgEditorFactory;
        std::map<std::string, Property> m_dlgProps;
        Property                        m_currentPropertyClicked;
        QList<QWidget*>                 m_dialogs;

    };
  }
}

#endif



