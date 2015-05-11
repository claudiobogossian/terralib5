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
  \file terralib/qt/widgets/AbstractPropertyManager.cpp

  \brief A singleton for holding the property browser 
          items factories.
*/

// TerraLib
#include "AbstractPropertyManager.h"

void te::qt::widgets::AbstractPropertyManager::setFactories(QtTreePropertyBrowser* pb)
{
  pb->setFactoryForManager(m_intSliderManager, m_sliderFactory);
  pb->setFactoryForManager(m_intManager, m_spinBoxFactory);
  pb->setFactoryForManager(m_doubleManager, m_doubleSpinBoxFactory);
  pb->setFactoryForManager(m_stringManager, m_lineEditFactory);
  pb->setFactoryForManager(m_strDlgManager, m_dlgFactory);
  pb->setFactoryForManager(m_colorManager, m_colorFactory);
  pb->setFactoryForManager(m_colorManager->subIntPropertyManager(), m_spinBoxFactory);
  pb->setFactoryForManager(m_fontManager, m_fontFactory);
  pb->setFactoryForManager(m_fontManager->subIntPropertyManager(), m_spinBoxFactory);
  pb->setFactoryForManager(m_fontManager->subBoolPropertyManager(), m_checkBoxFactory);
  pb->setFactoryForManager(m_fontManager->subEnumPropertyManager(), m_comboBoxFactory);
  pb->setFactoryForManager(m_sizeManager->subIntPropertyManager(), m_spinBoxFactory);
  pb->setFactoryForManager(m_enumManager, m_comboBoxFactory);
  pb->setFactoryForManager(m_pointFManager->subDoublePropertyManager(), m_doubleSpinBoxFactory);
}

te::qt::widgets::AbstractPropertyManager::AbstractPropertyManager()
{
  m_groupManager = new QtGroupPropertyManager(this);
  m_intManager = new QtIntPropertyManager(this);
  m_intSliderManager = new QtIntPropertyManager(this);
  m_doubleManager = new QtDoublePropertyManager(this);
  m_stringManager = new QtStringPropertyManager(this);
  m_strDlgManager = new QtStringPropertyManager(this);
  m_colorManager = new QtColorPropertyManager(this);
  m_fontManager = new QtFontPropertyManager(this);
  m_sizeManager = new QtSizePropertyManager(this);
  m_enumManager = new QtEnumPropertyManager(this);
  m_pointFManager = new QtPointFPropertyManager(this);

  m_doubleSpinBoxFactory = new QtDoubleSpinBoxFactory(this);
  m_checkBoxFactory = new QtCheckBoxFactory(this);
  m_spinBoxFactory = new QtSpinBoxFactory(this);
  m_lineEditFactory = new QtLineEditFactory(this);
  m_comboBoxFactory = new QtEnumEditorFactory(this);
  m_sliderFactory = new QtSliderFactory(this);
  m_colorFactory = new QtColorEditorFactory(this);
  m_fontFactory = new QtFontEditorFactory(this);
  m_charFactory = new QtCharEditorFactory(this);
  m_dlgFactory = new QtDlgEditorFactory(this);
}

te::qt::widgets::AbstractPropertyManager::~AbstractPropertyManager()
{
}

