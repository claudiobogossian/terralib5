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


#include "VectorizationWizardPage.h"

#include <ui_VectorizationWizardPageForm.h>

namespace te
{
  namespace qt
  {
    namespace widgets
    {
      
      VectorizationWizardPage::VectorizationWizardPage(QWidget *parent) :
        QWizardPage(parent)
      {
        m_ui.reset(new Ui::VectorizationWizardPageForm);

        m_ui->setupUi(this);
      }
      
    } // namespace widgets
  } // namespace qt
} // namespace te
