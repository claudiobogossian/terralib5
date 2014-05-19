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
  \file terralib/qt/widgets/colorbar/ColorCatalogWidget.cpp

  \brief ...
*/

// TerraLib
#include "../../../color/ColorBar.h"
#include "../../../color/ColorScheme.h"
#include "../../../color/ColorSchemeCatalog.h"
#include "../../../color/ColorSchemeCatalogManager.h"
#include "../../../color/ColorSchemeGroup.h"
#include "../../../color/RGBAColor.h"
#include "ColorBar.h"
#include "ColorCatalogWidget.h"
#include "ui_ColorCatalogWidgetForm.h"

// QT
#include <QtGui/QGridLayout>


te::qt::widgets::ColorCatalogWidget::ColorCatalogWidget(QWidget* parent, Qt::WindowFlags f)
  : QWidget(parent, f),
    m_ui(new Ui::ColorCatalogWidgetForm)
{
  m_ui->setupUi(this);

  //build form
  QGridLayout* l = new QGridLayout(m_ui->m_widget);
  l->setContentsMargins(0,0,0,0);

  m_colorBar = new  te::qt::widgets::colorbar::ColorBar(m_ui->m_widget);
  l->addWidget(m_colorBar);

  //connects
  connect(m_ui->m_catalogComboBox, SIGNAL(activated(int)), this, SLOT(onCatalogComboBoxActivated(int)));
  connect(m_ui->m_groupComboBox, SIGNAL(activated(int)), this, SLOT(onGroupComboBoxActivated(int)));
  connect(m_ui->m_schemaComboBox, SIGNAL(activated(int)), this, SLOT(onSchemaComboBoxActivated(int)));
  connect(m_ui->m_catalogGroupBox, SIGNAL(clicked(bool)), this, SLOT(onUseCatalogGroupBoxClicked(bool)));
  
  connect(m_colorBar, SIGNAL(colorBarChanged()), this, SIGNAL(colorBarChanged()));

  onUseCatalogGroupBoxClicked(false);
}

te::qt::widgets::ColorCatalogWidget::~ColorCatalogWidget()
{
}

te::qt::widgets::colorbar::ColorBar* te::qt::widgets::ColorCatalogWidget::getColorBar()
{
  return m_colorBar;
}

void te::qt::widgets::ColorCatalogWidget::loadDefaultColorCatalog()
{
  m_ui->m_catalogComboBox->clear();

  std::pair<std::vector<te::color::ColorSchemeCatalog*>::const_iterator, std::vector<te::color::ColorSchemeCatalog*>::const_iterator> cscPairIt = te::color::ColorSchemeCatalogManager::getInstance().getIterator();

  while(cscPairIt.first != cscPairIt.second)
  {
    te::color::ColorSchemeCatalog* csc = *cscPairIt.first;

    m_ui->m_catalogComboBox->addItem(csc->getName().c_str());

    ++cscPairIt.first;
  }

  if(m_ui->m_catalogComboBox->count() != 0)
    onCatalogComboBoxActivated(0);
}

void te::qt::widgets::ColorCatalogWidget::onCatalogComboBoxActivated(int index)
{
  std::string name = m_ui->m_catalogComboBox->itemText(index).toStdString();

  te::color::ColorSchemeCatalog* csc = te::color::ColorSchemeCatalogManager::getInstance().findByName(name);
  
  if(!csc)
    return;

  m_ui->m_groupComboBox->clear();

  std::vector<te::color::ColorSchemeGroup*> csgVec = csc->getColorSchemeGroups();

  for(std::size_t t = 0; t < csgVec.size(); ++t)
  {
    te::color::ColorSchemeGroup* csg = csgVec[t];

    m_ui->m_groupComboBox->addItem(csg->getName().c_str());
  }

  if(m_ui->m_groupComboBox->count() != 0)
    onGroupComboBoxActivated(0);
}

void te::qt::widgets::ColorCatalogWidget::onGroupComboBoxActivated(int index)
{
  std::string catalog = m_ui->m_catalogComboBox->currentText().toStdString();

  std::string group = m_ui->m_groupComboBox->itemText(index).toStdString();

  te::color::ColorSchemeCatalog* csc = te::color::ColorSchemeCatalogManager::getInstance().findByName(catalog);

  m_ui->m_schemaComboBox->clear();

  if(csc)
  {
    std::vector<te::color::ColorSchemeGroup*> csgVec = csc->getColorSchemeGroups();

    for(std::size_t t = 0; t < csgVec.size(); ++t)
    {
      te::color::ColorSchemeGroup* csg = csgVec[t];

      if(csg->getName() == group)
      {
        std::vector<te::color::ColorScheme*> csVec = csg->getColorSchemes();

        for(std::size_t p = 0; p < csVec.size(); ++p)
        {
          te::color::ColorScheme* cs = csVec[p];

          m_ui->m_schemaComboBox->addItem(cs->getName().c_str());
        }
      }
    }
  }

  if(m_ui->m_schemaComboBox->count() != 0)
    onSchemaComboBoxActivated(0);
}

void te::qt::widgets::ColorCatalogWidget::onSchemaComboBoxActivated(int index)
{
  std::string catalog = m_ui->m_catalogComboBox->currentText().toStdString();

  std::string group = m_ui->m_groupComboBox->currentText().toStdString();

  std::string schema = m_ui->m_schemaComboBox->itemText(index).toStdString();

  te::color::ColorSchemeCatalog* csc = te::color::ColorSchemeCatalogManager::getInstance().findByName(catalog);

  if(csc)
  {
    std::vector<te::color::ColorSchemeGroup*> csgVec = csc->getColorSchemeGroups();

    for(std::size_t t = 0; t < csgVec.size(); ++t)
    {
      te::color::ColorSchemeGroup* csg = csgVec[t];

      if(csg->getName() == group)
      {
        std::vector<te::color::ColorScheme*> csVec = csg->getColorSchemes();

        for(std::size_t p = 0; p < csVec.size(); ++p)
        {
          te::color::ColorScheme* cs = csVec[p];

          if(cs->getName() == schema)
          {
            std::vector<te::color::RGBAColor>* colors = cs->getColors()[0];     //WARNING... Always getting the first position

            std::vector<te::color::RGBAColor>::iterator it = colors->begin();

            // create color bar
            te::color::ColorBar* cb = new te::color::ColorBar(*(colors->begin()), *(colors->end() - 1), 256);

            int count = 0;

            //fill color bar
            while(it != colors->end())
            {
              if(count != 0 && count != colors->size() - 1)
              {
                double pos = (1. / (colors->size() - 1)) * count;

                cb->addColor(*it, pos);
              }
              
              ++count;
              ++it;
            }

            //set color bar
            m_colorBar->setColorBar(cb);
          }
        }
      }
    }
  }
}

void te::qt::widgets::ColorCatalogWidget::onUseCatalogGroupBoxClicked(bool flag)
{
  if(flag) //start catalog
    loadDefaultColorCatalog();
  else //create default color bar
  {
    te::color::ColorBar* cb = new te::color::ColorBar(te::color::RGBAColor(0, 0, 0, 255), te::color::RGBAColor(255, 0, 0, 255), 256);
    m_colorBar->setColorBar(cb);
  }
}
