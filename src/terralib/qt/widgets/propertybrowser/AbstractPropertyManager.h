/*  Copyright (C) 2011-2012 National Institute For Space Research (INPE) - Brazil.

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
  \file terralib/qt/widgets/AbstractPropertyManager.h

  \brief A singleton for holding the property browser 
    items factories.
*/

#ifndef __TERRALIB_QT_WIDGETS_INTERNAL_ABSTRACTPROPERTYMANAGER_H
#define __TERRALIB_QT_WIDGETS_INTERNAL_ABSTRACTPROPERTYMANAGER_H

// TerraLib
#include "../../../common/Singleton.h"
#include "../Config.h"

// Qt
#include <QWidget>

// QtPropertyBrowser
#include <QtPropertyBrowser/QtCheckBoxFactory>
#include <QtPropertyBrowser/QtDoubleSpinBoxFactory>
#include <QtPropertyBrowser/QtGroupPropertyManager>
#include <QtPropertyBrowser/QtSpinBoxFactory>
#include <QtPropertyBrowser/QtTreePropertyBrowser>
#include <QtPropertyBrowser/QtVariantPropertyManager>

namespace te
{
  namespace qt
  {
    namespace widgets
    {
      /*!
        \class AbstractPropertyManager

        \brief A singleton for holding the property browser 
                items factories.
      */
      class TEQTWIDGETSEXPORT AbstractPropertyManager : public te::common::Singleton<AbstractPropertyManager>, public QWidget
      {
        friend class te::common::Singleton<AbstractPropertyManager>;

      public:

        void setFactories(QtTreePropertyBrowser* pb);

        protected:

          /*! \brief It initializes the singleton. */
          AbstractPropertyManager();

          /*! \brief Destructor. */
          ~AbstractPropertyManager();

        public:
          QtGroupPropertyManager*   m_groupManager;
          QtIntPropertyManager*     m_intManager;
          QtIntPropertyManager*     m_intSliderManager;
          QtDoublePropertyManager*  m_doubleManager;
          QtStringPropertyManager*  m_stringManager;
          QtStringPropertyManager*  m_strDlgManager;
          QtColorPropertyManager*   m_colorManager;
          QtFontPropertyManager*    m_fontManager;
          QtSizePropertyManager*    m_sizeManager;
          QtEnumPropertyManager*    m_enumManager;
          QtPointFPropertyManager*  m_pointFManager;

          QtDoubleSpinBoxFactory*   m_doubleSpinBoxFactory;
          QtCheckBoxFactory*        m_checkBoxFactory;
          QtSpinBoxFactory*         m_spinBoxFactory;
          QtLineEditFactory*        m_lineEditFactory;
          QtEnumEditorFactory*      m_comboBoxFactory;
          QtSliderFactory*          m_sliderFactory;
          QtColorEditorFactory*     m_colorFactory;
          QtFontEditorFactory*      m_fontFactory;
          QtCharEditorFactory*      m_charFactory;
          //QtDlgEditorFactory*       m_dlgFactory;

      };

    }   // end namespace af
  }     // end namespace qt
}       // end namespace te

#endif  // __TERRALIB_QT_WIDGETS_INTERNAL_ABSTRACTPROPERTYMANAGER_H
