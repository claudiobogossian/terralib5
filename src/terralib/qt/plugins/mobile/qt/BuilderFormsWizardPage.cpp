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
\file src/terraMobilePlugin/qt/BuilderFormsWizardPage.cpp

\brief This interface is used to configure the form json file for GeoPackage Builder wizard operation.
*/

// TerraLib
#include "../../../../common/STLUtils.h"
#include "../../../../common/StringUtils.h"
#include "../../../../dataaccess/dataset/PrimaryKey.h"
#include "../core/form/BoolFormItem.h"
#include "../core/form/DateFormItem.h"
#include "../core/form/DoubleFormItem.h"
#include "../core/form/IntFormItem.h"
#include "../core/form/LabelFormItem.h"
#include "../core/form/PictureFormItem.h"
#include "../core/form/Serializer.h"
#include "../core/form/StringComboFormItem.h"
#include "../core/form/StringFormItem.h"
#include "../core/form/TimeFormItem.h"
#include "../core/form/Form.h"
#include "../core/form/Section.h"
#include "BuilderFormsWizardPage.h"
#include "ui_BuilderFormsWizardPageForm.h"

// Qt
#include <QListWidgetItem>
#include <QMessageBox>
#include <QValidator>

#define FORM_TREE_ITEM      0
#define PROPERTY_TREE_ITEM  1

te::qt::plugins::terramobile::BuilderFormsWizardPage::BuilderFormsWizardPage(QWidget* parent)
  : QWizardPage(parent),
  m_ui(new Ui::BuilderFormsWizardPageForm),
  m_curForm(0),
  m_curFormItem(0),
  m_itemTreeType(-1)
{
  // setup controls
  m_ui->setupUi(this);

  //configure page
  this->setTitle(tr("GeoPackage Builder Forms Configurer"));
  this->setSubTitle(tr("Used to configure Forms for each Gathering Layer selected."));

  m_ui->m_saveFormItemToolButton->setIcon(QIcon::fromTheme("check"));

  m_ui->m_doubleLineEdit->setValidator(new QDoubleValidator(this));
  m_ui->m_intLineEdit->setValidator(new QIntValidator(this));

  //empty page
  m_ui->m_stackedWidget->setCurrentIndex(9);

  //connect slots
  connect(m_ui->m_treeWidget, SIGNAL(itemClicked(QTreeWidgetItem*, int)), this, SLOT(onTreeItemClicked(QTreeWidgetItem*, int)));
  connect(m_ui->m_stringComboAddToolButton, SIGNAL(pressed()), this, SLOT(onStringComboAddToolButtonPressed()));
  connect(m_ui->m_saveFormItemToolButton, SIGNAL(pressed()), this, SLOT(onSaveFormItemToolButton()));
}

te::qt::plugins::terramobile::BuilderFormsWizardPage::~BuilderFormsWizardPage()
{
  te::common::FreeContents(m_sectionsMap);
  m_sectionsMap.clear();
}

bool te::qt::plugins::terramobile::BuilderFormsWizardPage::isComplete() const
{
  return true;
}

void te::qt::plugins::terramobile::BuilderFormsWizardPage::setLayerList(std::list<te::map::AbstractLayerPtr> list)
{
  m_list = list;

  //clear current sections
  te::common::FreeContents(m_sectionsMap);
  m_sectionsMap.clear();

  //get informations from input layers
  std::list<te::map::AbstractLayerPtr>::iterator it = m_list.begin();

  while (it != m_list.end())
  {
    te::map::AbstractLayerPtr l = *it;

    std::string layerName = l->getTitle();

    //get dataset type
    std::auto_ptr<te::da::DataSetType> dsType = l->getSchema();

    std::string pkAttrName = "";

    te::da::PrimaryKey* pk = dsType->getPrimaryKey();

    if (pk && !pk->getProperties().empty())
    {
      pkAttrName = pk->getProperties()[0]->getName();
    }

    std::vector<te::dt::Property*> props = dsType->getProperties();

    //create section
    te::qt::plugins::terramobile::Section* section = new te::qt::plugins::terramobile::Section();
    section->setName("terramobile");
    section->setDescription("Data collector for field work");

    //create form
    te::qt::plugins::terramobile::Form* form = new te::qt::plugins::terramobile::Form();
    form->setName(layerName);

    //create items
    for (std::size_t t = 0; t < props.size(); ++t)
    {
      if (props[t]->getName() != pkAttrName && props[t]->getType() != te::dt::GEOMETRY_TYPE)
      {
        te::qt::plugins::terramobile::AbstractFormItem* item;

        if (props[t]->getType() == te::dt::INT16_TYPE || props[t]->getType() == te::dt::INT32_TYPE || props[t]->getType() == te::dt::INT64_TYPE)
        {
          item = new te::qt::plugins::terramobile::IntFormItem();
        }
        else if (props[t]->getType() == te::dt::DOUBLE_TYPE || props[t]->getType() == te::dt::FLOAT_TYPE)
        {
          item = new te::qt::plugins::terramobile::DoubleFormItem();
        }
        else if (props[t]->getType() == te::dt::BOOLEAN_TYPE)
        {
          item = new te::qt::plugins::terramobile::BoolFormItem();
        }
        else if (props[t]->getType() == te::dt::STRING_TYPE)
        {
          item = new te::qt::plugins::terramobile::StringFormItem();
        }

        item->setKey(props[t]->getName());
        item->setLabel(props[t]->getName());

        form->getItems().push_back(item);
      }
    }

    //add picture property
    te::qt::plugins::terramobile::PictureFormItem* pic = new te::qt::plugins::terramobile::PictureFormItem();

    pic->setKey("picture");
    pic->setLabel("picture");

    form->getItems().push_back(pic);

    //add form to section
    section->getForms().push_back(form);

    //add section to section map
    m_sectionsMap.insert(std::map<std::string, Section*>::value_type(layerName, section));

    ++it;
  }

  listSectionMap();
}

std::map<std::string, te::qt::plugins::terramobile::Section*>& te::qt::plugins::terramobile::BuilderFormsWizardPage::getSections()
{
  return m_sectionsMap;
}

void te::qt::plugins::terramobile::BuilderFormsWizardPage::onTreeItemClicked(QTreeWidgetItem* item, int column)
{
  if (!item)
    return;

  if (item->type() == PROPERTY_TREE_ITEM)
  {
    std::string propertyName = item->text(0).toStdString();

    QTreeWidgetItem* parent = item->parent();

    std::string formName = parent->text(0).toStdString();

    //get form
    te::qt::plugins::terramobile::Form* form = getForm(formName);

    if (!form)
      return;

    //get property
    te::qt::plugins::terramobile::AbstractFormItem* formItem = form->getItem(propertyName);

    if (!formItem)
      return;

    //set the right stacked widget
    std::string propType = formItem->getType();

    if (propType == BOOLFORMITEMTYPE)
    {
      te::qt::plugins::terramobile::BoolFormItem* fi = dynamic_cast<te::qt::plugins::terramobile::BoolFormItem*>(formItem);

      m_ui->m_defaultValueBoolCheckBox->setChecked(fi->hasDefaultValue());

      if (fi->hasDefaultValue())
      {
        m_ui->m_boolComboBox->setCurrentText(te::common::Convert2UCase(fi->getStrDefaultValue()).c_str());
      }

      m_ui->m_stackedWidget->setCurrentIndex(0);
    }
    else if (propType == DATEFORMITEMTYPE)
    {
      te::qt::plugins::terramobile::DateFormItem* fi = dynamic_cast<te::qt::plugins::terramobile::DateFormItem*>(formItem);

      m_ui->m_stackedWidget->setCurrentIndex(1);
    }
    else if (propType == DOUBLEFORMITEMTYPE)
    {
      te::qt::plugins::terramobile::DoubleFormItem* fi = dynamic_cast<te::qt::plugins::terramobile::DoubleFormItem*>(formItem);

      m_ui->m_defaultValueDoubleCheckBox->setChecked(fi->hasDefaultValue());

      if (fi->hasDefaultValue())
      {
        m_ui->m_doubleLineEdit->setText(fi->getStrDefaultValue().c_str());
      }
      else
      {
        m_ui->m_doubleLineEdit->clear();
      }

      m_ui->m_stackedWidget->setCurrentIndex(2);
    }
    else if (propType == INTFORMITEMTYPE)
    {
      te::qt::plugins::terramobile::IntFormItem* fi = dynamic_cast<te::qt::plugins::terramobile::IntFormItem*>(formItem);

      m_ui->m_defaultValueIntCheckBox->setChecked(fi->hasDefaultValue());

      if (fi->hasDefaultValue())
      {
        m_ui->m_intLineEdit->setText(fi->getStrDefaultValue().c_str());
      }
      else
      {
        m_ui->m_intLineEdit->clear();
      }

      m_ui->m_stackedWidget->setCurrentIndex(3);
    }
    else if (propType == LABELFORMITEMTYPE)
    {
      te::qt::plugins::terramobile::LabelFormItem* fi = dynamic_cast<te::qt::plugins::terramobile::LabelFormItem*>(formItem);

      m_ui->m_defaultValueLabelCheckBox->setChecked(fi->hasDefaultValue());

      if (fi->hasDefaultValue())
      {
        m_ui->m_labelLineEdit->setText(fi->getStrDefaultValue().c_str());
      }
      else
      {
        m_ui->m_labelLineEdit->clear();
      }

      m_ui->m_stackedWidget->setCurrentIndex(4);
    }
    else if (propType == PICTUREFORMITEMTYPE)
    {
      te::qt::plugins::terramobile::PictureFormItem* fi = dynamic_cast<te::qt::plugins::terramobile::PictureFormItem*>(formItem);

      m_ui->m_stackedWidget->setCurrentIndex(9);
    }
    else if (propType == STRINGCOMBOFORMITEMTYPE)
    {
      te::qt::plugins::terramobile::StringComboFormItem* fi = dynamic_cast<te::qt::plugins::terramobile::StringComboFormItem*>(formItem);

      m_ui->m_defaultValueStringComboCheckBox->setChecked(fi->hasDefaultValue());

      if (fi->hasDefaultValue())
      {
        m_ui->m_stringComboLineEdit->setText(fi->getStrDefaultValue().c_str());
      }
      else
      {
        m_ui->m_stringComboLineEdit->clear();
      }

      m_ui->m_listWidget->clear();

      for (std::size_t t = 0; t < fi->getValues().size(); ++t)
      {
        m_ui->m_listWidget->addItem(fi->getValues()[t].c_str());
      }

      m_ui->m_stackedWidget->setCurrentIndex(5);
    }
    else if (propType == STRINGFORMITEMTYPE)
    {
      te::qt::plugins::terramobile::StringFormItem* fi = dynamic_cast<te::qt::plugins::terramobile::StringFormItem*>(formItem);

      m_ui->m_defaultValueStringCheckBox->setChecked(fi->hasDefaultValue());

      if (fi->hasDefaultValue())
      {
        m_ui->m_stringLineEdit->setText(fi->getStrDefaultValue().c_str());
      }
      else
      {
        m_ui->m_stringLineEdit->clear();
      }

      m_ui->m_stackedWidget->setCurrentIndex(6);
    }
    else if (propType == TIMEFORMITEMTYPE)
    {
      te::qt::plugins::terramobile::TimeFormItem* fi = dynamic_cast<te::qt::plugins::terramobile::TimeFormItem*>(formItem);

      m_ui->m_stackedWidget->setCurrentIndex(7);
    }

    m_ui->m_formItemLineEdit->setText(propertyName.c_str());
      
    m_ui->m_descLineEdit->setText(formItem->getLabel().c_str());

    m_ui->m_descLineEdit->setEnabled(true);

    m_curFormItem = formItem;
  }
  else if (item->type() == FORM_TREE_ITEM)
  {
    m_ui->m_stackedWidget->setCurrentIndex(8);

    std::string formName = item->text(0).toStdString();

    //get form
    te::qt::plugins::terramobile::Form* form = getForm(formName);

    //set properties
    m_ui->m_formItemsListWidget->clear();

    for (std::size_t t = 0; t < form->getItems().size(); ++t)
    {
      m_ui->m_formItemsListWidget->addItem(form->getItems()[t]->getKey().c_str());
    }

    m_ui->m_formItemLineEdit->setText(formName.c_str());

    m_ui->m_descLineEdit->clear();

    m_ui->m_descLineEdit->setEnabled(false);

    m_curForm = form;
  }

  m_itemTreeType = item->type();
}

void te::qt::plugins::terramobile::BuilderFormsWizardPage::onStringComboAddToolButtonPressed()
{
  if (m_ui->m_stringComboValueLineEdit->text().isEmpty())
  {
    QMessageBox::warning(this, tr("Warning"), tr("Value not defined."));
    return;
  }

  m_ui->m_listWidget->addItem(m_ui->m_stringComboValueLineEdit->text());
}

void te::qt::plugins::terramobile::BuilderFormsWizardPage::onSaveFormItemToolButton()
{
  if (m_itemTreeType == PROPERTY_TREE_ITEM)
  {
    std::string propType = m_curFormItem->getType();

    if (propType == BOOLFORMITEMTYPE)
    {
      te::qt::plugins::terramobile::BoolFormItem* fi = dynamic_cast<te::qt::plugins::terramobile::BoolFormItem*>(m_curFormItem);

      if (m_ui->m_defaultValueBoolCheckBox->isChecked())
      {
        std::string value = m_ui->m_boolComboBox->currentText().toStdString();

        if (value == "TRUE")
          fi->setValue(true);
        else
          fi->setValue(false);
      }
    }
    else if (propType == DATEFORMITEMTYPE)
    {
      te::qt::plugins::terramobile::DateFormItem* fi = dynamic_cast<te::qt::plugins::terramobile::DateFormItem*>(m_curFormItem);
    }
    else if (propType == DOUBLEFORMITEMTYPE)
    {
      te::qt::plugins::terramobile::DoubleFormItem* fi = dynamic_cast<te::qt::plugins::terramobile::DoubleFormItem*>(m_curFormItem);

      if (m_ui->m_defaultValueDoubleCheckBox->isChecked())
      {
        fi->setValue(m_ui->m_doubleLineEdit->text().toDouble());
      }
    }
    else if (propType == INTFORMITEMTYPE)
    {
      te::qt::plugins::terramobile::IntFormItem* fi = dynamic_cast<te::qt::plugins::terramobile::IntFormItem*>(m_curFormItem);

      if (m_ui->m_defaultValueIntCheckBox->isChecked())
      {
        fi->setValue(m_ui->m_intLineEdit->text().toInt());
      }
    }
    else if (propType == LABELFORMITEMTYPE)
    {
      te::qt::plugins::terramobile::LabelFormItem* fi = dynamic_cast<te::qt::plugins::terramobile::LabelFormItem*>(m_curFormItem);

      if (m_ui->m_defaultValueLabelCheckBox->isChecked())
      {
        fi->setValue(m_ui->m_labelLineEdit->text().toStdString());
      }
    }
    else if (propType == PICTUREFORMITEMTYPE)
    {
      te::qt::plugins::terramobile::PictureFormItem* fi = dynamic_cast<te::qt::plugins::terramobile::PictureFormItem*>(m_curFormItem);
    }
    else if (propType == STRINGCOMBOFORMITEMTYPE)
    {
      te::qt::plugins::terramobile::StringComboFormItem* fi = dynamic_cast<te::qt::plugins::terramobile::StringComboFormItem*>(m_curFormItem);

      if (m_ui->m_defaultValueStringComboCheckBox->isChecked())
      {
        fi->setValue(m_ui->m_stringComboLineEdit->text().toStdString());
      }

      std::vector<std::string> values;

      for (int i = 0; i < m_ui->m_listWidget->count(); ++i)
      {
        values.push_back(m_ui->m_listWidget->item(i)->text().toStdString());
      }

      fi->setValues(values);
    }
    else if (propType == STRINGFORMITEMTYPE)
    {
      te::qt::plugins::terramobile::StringFormItem* fi = dynamic_cast<te::qt::plugins::terramobile::StringFormItem*>(m_curFormItem);

      if (m_ui->m_defaultValueStringCheckBox->isChecked())
      {
        fi->setValue(m_ui->m_stringLineEdit->text().toStdString());
      }
    }
    else if (propType == TIMEFORMITEMTYPE)
    {
      te::qt::plugins::terramobile::TimeFormItem* fi = dynamic_cast<te::qt::plugins::terramobile::TimeFormItem*>(m_curFormItem);
    }

    std::string desc = "";

    if (!m_ui->m_descLineEdit->text().isEmpty())
    {
      desc = m_ui->m_descLineEdit->text().toStdString();
    }

    m_curFormItem->setLabel(desc);

    QMessageBox::information(this, tr("Information"), tr("Form Item updated."));
  }
  else if (m_itemTreeType == FORM_TREE_ITEM)
  {
    std::vector<std::string> values;

    for (int i = 0; i < m_ui->m_formItemsListWidget->count(); ++i)
    {
      values.push_back(m_ui->m_formItemsListWidget->item(i)->text().toStdString());
    }

    std::vector<AbstractFormItem*> backupItems;

    for (std::size_t t = 0; t < m_curForm->getItems().size(); ++t)
    {
      backupItems.push_back(m_curForm->getItems()[t]);
    }

    m_curForm->getItems().clear();

    for (std::size_t t = 0; t < values.size(); ++t)
    {
      for (std::size_t p = 0; p < backupItems.size(); ++p)
      {
        if (values[t] == backupItems[p]->getKey())
        {
          m_curForm->getItems().push_back(backupItems[p]);
        }
      }
    }

    backupItems.clear();

    QMessageBox::information(this, tr("Information"), tr("Form updated."));
  }

  listSectionMap();
}

void te::qt::plugins::terramobile::BuilderFormsWizardPage::onItemTypeChanged(int index)
{
  QComboBox* cmbBox = dynamic_cast<QComboBox*>(sender());

  if (!cmbBox)
    return;

  //find form and item
  te::qt::plugins::terramobile::Form* form = 0;

  te::qt::plugins::terramobile::AbstractFormItem* formItem = 0;

  QTreeWidgetItemIterator it(m_ui->m_treeWidget);

  while(*it) 
  {
    QTreeWidgetItem* item = *it;

    QWidget* widget = m_ui->m_treeWidget->itemWidget(item, 1);

    if (widget && widget == cmbBox)
    {
      std::string propertyName = item->text(0).toStdString();

      QTreeWidgetItem* parent = item->parent();

      std::string formName = parent->text(0).toStdString();

      //get form
      form = getForm(formName);

      //get property
      if (form)
        formItem = form->getItem(propertyName);

      break;
    }
    ++it;
  }

  if (!form || !formItem)
  {
    return;
  }

  //create new item
  std::string propType = cmbBox->itemText(index).toStdString();

  te::qt::plugins::terramobile::AbstractFormItem* item;

  if (propType == BOOLFORMITEMTYPE)
  {
    item = new te::qt::plugins::terramobile::BoolFormItem();
  }
  else if (propType == DATEFORMITEMTYPE)
  {
    item = new te::qt::plugins::terramobile::DateFormItem();
  }
  else if (propType == DOUBLEFORMITEMTYPE)
  {
    item = new te::qt::plugins::terramobile::DoubleFormItem();
  }
  else if (propType == INTFORMITEMTYPE)
  {
    item = new te::qt::plugins::terramobile::IntFormItem();
  }
  else if (propType == LABELFORMITEMTYPE)
  {
    item = new te::qt::plugins::terramobile::LabelFormItem();
  }
  else if (propType == PICTUREFORMITEMTYPE)
  {
    item = new te::qt::plugins::terramobile::PictureFormItem();
  }
  else if (propType == STRINGCOMBOFORMITEMTYPE)
  {
    item = new te::qt::plugins::terramobile::StringComboFormItem();
  }
  else if (propType == STRINGFORMITEMTYPE)
  {
    item = new te::qt::plugins::terramobile::StringFormItem();
  }
  else if (propType == TIMEFORMITEMTYPE)
  {
    item = new te::qt::plugins::terramobile::TimeFormItem();
  }

  item->setKey(formItem->getKey());
  item->setLabel(formItem->getLabel());

  //replace item
  for (std::size_t t = 0; t < form->getItems().size(); ++t)
  {
    if (form->getItems()[t]->getKey() == formItem->getKey())
    {
      delete form->getItems()[t];

      form->getItems()[t] = item;

      break;
    }
  }

  listSectionMap();
}

te::qt::plugins::terramobile::Form* te::qt::plugins::terramobile::BuilderFormsWizardPage::getForm(const std::string& formName)
{
  std::map<std::string, Section*>::iterator it = m_sectionsMap.begin();

  while (it != m_sectionsMap.end())
  {
    te::qt::plugins::terramobile::Form* f = it->second->getForm(formName);

    if (f)
    {
      return f;
    }

    ++it;
  }

  return 0;
}

void te::qt::plugins::terramobile::BuilderFormsWizardPage::listSectionMap()
{
  m_ui->m_treeWidget->clear();

  std::map<std::string, Section*>::iterator it = m_sectionsMap.begin();

  while (it != m_sectionsMap.end())
  {
    te::qt::plugins::terramobile::Section* section = it->second;

    for (std::size_t t = 0; t < section->getForms().size(); ++t)
    {
      te::qt::plugins::terramobile::Form* form = section->getForms()[t];

      QTreeWidgetItem* item = new QTreeWidgetItem(m_ui->m_treeWidget, FORM_TREE_ITEM);
      item->setText(0, form->getName().c_str());
      item->setIcon(0, QIcon::fromTheme("document-info"));

      m_ui->m_treeWidget->addTopLevelItem(item);

      for (std::size_t p = 0; p < form->getItems().size(); ++p)
      {
        te::qt::plugins::terramobile::AbstractFormItem* formItem = form->getItems()[p];

        QTreeWidgetItem* subItem = new QTreeWidgetItem(item, PROPERTY_TREE_ITEM);
        subItem->setText(0, formItem->getKey().c_str());
        subItem->setIcon(0, QIcon::fromTheme("layer-info"));
        item->addChild(subItem);
        item->setExpanded(true);

        std::string curType = formItem->getType().c_str();

        QComboBox* cmbProperty = new QComboBox(m_ui->m_treeWidget);

        cmbProperty->addItem(BOOLFORMITEMTYPE);
        cmbProperty->addItem(DATEFORMITEMTYPE);
        cmbProperty->addItem(DOUBLEFORMITEMTYPE);
        cmbProperty->addItem(INTFORMITEMTYPE);
        cmbProperty->addItem(LABELFORMITEMTYPE);
        cmbProperty->addItem(PICTUREFORMITEMTYPE);
        cmbProperty->addItem(STRINGCOMBOFORMITEMTYPE);
        cmbProperty->addItem(STRINGFORMITEMTYPE);
        cmbProperty->addItem(TIMEFORMITEMTYPE);

        cmbProperty->setCurrentText(curType.c_str());

        connect(cmbProperty, SIGNAL(activated(int)), this, SLOT(onItemTypeChanged(int)));

        m_ui->m_treeWidget->setItemWidget(subItem, 1, cmbProperty);
      }
    }

    ++it;
  }

  m_ui->m_treeWidget->resizeColumnToContents(0);
  m_ui->m_treeWidget->resizeColumnToContents(1);

  //empty page
  m_ui->m_stackedWidget->setCurrentIndex(9);
  m_ui->m_formItemLineEdit->clear();
  m_ui->m_descLineEdit->clear();

  m_curForm = 0;
  m_curFormItem = 0;

  //build sections descriptions
  m_ui->m_plainTextEdit->clear();

  it = m_sectionsMap.begin();

  while (it != m_sectionsMap.end())
  {
    te::qt::plugins::terramobile::Section* s = it->second;

    std::string jsonStr = te::qt::plugins::terramobile::Write(s);

    m_ui->m_plainTextEdit->appendPlainText(jsonStr.c_str());

    ++it;
  }
}
