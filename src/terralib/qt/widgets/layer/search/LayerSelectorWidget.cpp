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
  \file terralib/qt/widgets/layer/search/LayerSelectorWidget.cpp

  \brief This file has the LayerSelectorWidget class.
*/

// TerraLib
#include "LayerSearchDialog.h"
#include "LayerSearchWidget.h"
#include "LayerSelectorWidget.h"
#include "ui_LayerSelectorWidgetForm.h"

Q_DECLARE_METATYPE(te::map::AbstractLayerPtr);

te::qt::widgets::LayerSelectorWidget::LayerSelectorWidget(QWidget* parent, Qt::WindowFlags f)
  : QWidget(parent, f),
    m_ui(new Ui::LayerSelectorWidgetForm)
{
//build form
  m_ui->setupUi(this);

  m_ui->m_layerSearchToolButton->setIcon(QIcon::fromTheme("zoom"));

  m_layerSearchDlg.reset( new te::qt::widgets::LayerSearchDialog(this));

  m_layerSearchDlg->getWidget()->enableMultiSelection(false);

//connects
  connect(m_ui->m_layerSearchToolButton, SIGNAL(clicked()), this, SLOT(onLayerSearchToolButtonClicked()));
  connect(m_ui->m_layerComboBox, SIGNAL(activated(int)), this, SLOT(onLayerComboBoxActivated(int)));
}

te::qt::widgets::LayerSelectorWidget::~LayerSelectorWidget()
{
}

Ui::LayerSelectorWidgetForm* te::qt::widgets::LayerSelectorWidget::getForm() const
{
  return m_ui.get();
}

void te::qt::widgets::LayerSelectorWidget::setList(std::list<te::map::AbstractLayerPtr>& layerList, FilterOptions fo)
{
  m_layerSearchDlg->getWidget()->setList(layerList);

  m_layerList = layerList;

  //fill combo
  m_ui->m_layerComboBox->clear();

  if(fo == NO_FILTER)
    filterAll();
  else if(fo == FILTER_BY_GEOM)
    filterOnlyByGeom();
  else if(fo == FILTER_BY_RASTER)
    filterOnlyByRaster();

  if(m_ui->m_layerComboBox->count() != 0)
    onLayerComboBoxActivated(0);
}

void te::qt::widgets::LayerSelectorWidget::filterOnlyByRaster()
{
  m_layerSearchDlg->getWidget()->filterOnlyByRaster();

  std::list<te::map::AbstractLayerPtr>::iterator it = m_layerList.begin();

  while(it != m_layerList.end())
  {
    te::map::AbstractLayerPtr l = *it;

    std::auto_ptr<te::da::DataSetType> dsType = l->getSchema();

    if(dsType.get() && dsType->hasRaster())
      m_ui->m_layerComboBox->addItem(it->get()->getTitle().c_str(), QVariant::fromValue(l));

    ++it;
  }
}

void te::qt::widgets::LayerSelectorWidget::filterOnlyByGeom()
{
  m_layerSearchDlg->getWidget()->filterOnlyByGeom();

  std::list<te::map::AbstractLayerPtr>::iterator it = m_layerList.begin();

  while(it != m_layerList.end())
  {
    te::map::AbstractLayerPtr l = *it;

    std::auto_ptr<te::da::DataSetType> dsType = l->getSchema();

    if(dsType.get() && dsType->hasGeom())
      m_ui->m_layerComboBox->addItem(it->get()->getTitle().c_str(), QVariant::fromValue(l));

    ++it;
  }
}

void te::qt::widgets::LayerSelectorWidget::filterAll()
{
  m_layerSearchDlg->getWidget()->filterAll();

  std::list<te::map::AbstractLayerPtr>::iterator it = m_layerList.begin();

  while(it != m_layerList.end())
  {
    te::map::AbstractLayerPtr l = *it;

    m_ui->m_layerComboBox->addItem(it->get()->getTitle().c_str(), QVariant::fromValue(l));

    ++it;
  }
}

void te::qt::widgets::LayerSelectorWidget::onLayerSearchToolButtonClicked()
{
  if(m_layerSearchDlg->exec() == QDialog::Accepted)
  {
    std::list<te::map::AbstractLayerPtr> list = m_layerSearchDlg->getWidget()->getSelecteds();

    if(!list.empty())
    {
      te::map::AbstractLayerPtr layer = *list.begin();

      //set layer in combo
      int size = m_ui->m_layerComboBox->count();

      for(int i = 0; i < size; ++i)
      {
        QVariant varLayer = m_ui->m_layerComboBox->itemData(i, Qt::UserRole);
        te::map::AbstractLayerPtr l = varLayer.value<te::map::AbstractLayerPtr>();

        if(layer == l)
        {
          m_ui->m_layerComboBox->setCurrentIndex(i);

          onLayerComboBoxActivated(i);

          break;
        }
      }
    }
  }
}

void te::qt::widgets::LayerSelectorWidget::onLayerComboBoxActivated(int index)
{
  QVariant varLayer = m_ui->m_layerComboBox->itemData(index, Qt::UserRole);
  te::map::AbstractLayerPtr l = varLayer.value<te::map::AbstractLayerPtr>();

  emit layerSelected(l);
}
