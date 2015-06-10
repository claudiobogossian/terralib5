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
  \file terralib/qt/widgets/rp/ArithmeticOpWizardPage.cpp

  \brief This file defines a class for a ArithmeticOp Wizard page.
*/

// TerraLib
#include "../../../dataaccess/dataset/DataSet.h"
#include "../../../dataaccess/utils/Utils.h"
#include "ArithmeticOpWizard.h"
#include "ArithmeticOpWizardPage.h"
#include "ui_ArithmeticOpWizardPageForm.h"

// Qt
#include <QApplication>
#include <QGridLayout>
#include <QMessageBox>

// stl
#include <memory>

Q_DECLARE_METATYPE(te::map::AbstractLayerPtr);

te::qt::widgets::ArithmeticOpWizardPage::ArithmeticOpWizardPage(QWidget* parent)
  : QWizardPage(parent),
    m_ui(new Ui::ArithmeticOpWizardPageForm)
{
// setup controls
  m_ui->setupUi(this);

//configure page
  this->setTitle(tr("Arithmetic Operations"));
  this->setSubTitle(tr("Select the type of arithmetic operation and set their specific parameters."));

  m_ui->m_valueLineEdit->setValidator(new QDoubleValidator(this));
  m_ui->m_gainLineEdit->setValidator(new QDoubleValidator(this));
  m_ui->m_offsetLineEdit->setValidator(new QDoubleValidator(this));

  m_ui->m_addValueOpToolButton->setIcon(QIcon::fromTheme("list-add"));
  m_ui->m_addRasterOpToolButton->setIcon(QIcon::fromTheme("list-add"));
  m_ui->m_undoToolButton->setIcon(QIcon::fromTheme("edit-undo"));
  m_ui->m_redoToolButton->setIcon(QIcon::fromTheme("edit-redo"));
  m_ui->m_clearToolButton->setIcon(QIcon::fromTheme("edit-clear"));
  m_ui->m_validateToolButton->setIcon(QIcon::fromTheme("check"));

  m_expressionHistoryCounter = -1;

  m_ui->m_undoToolButton->setEnabled(false);
  m_ui->m_redoToolButton->setEnabled(false);

  m_ui->m_tabWidget->removeTab(1);

  //connects
  connect(m_ui->m_layerComboBox, SIGNAL(activated(int)), this, SLOT(layerComboBoxActivated(int)));
  connect(m_ui->m_layerAComboBox, SIGNAL(activated(int)), this, SLOT(layerAComboBoxActivated(int)));
  connect(m_ui->m_layerBComboBox, SIGNAL(activated(int)), this, SLOT(layerBComboBoxActivated(int)));

  connect(m_ui->m_leftBracketToolButton, SIGNAL(clicked()), this, SLOT(leftBracketToolButtonClicked()));
  connect(m_ui->m_rightBracketToolButton, SIGNAL(clicked()), this, SLOT(rightBracketToolButtonClicked()));
  connect(m_ui->m_plusToolButton, SIGNAL(clicked()), this, SLOT(plusToolButtonClicked()));
  connect(m_ui->m_minusToolButton, SIGNAL(clicked()), this, SLOT(minusToolButtonClicked()));
  connect(m_ui->m_multiToolButton, SIGNAL(clicked()), this, SLOT(multiToolButtonClicked()));
  connect(m_ui->m_divToolButton, SIGNAL(clicked()), this, SLOT(divToolButtonClicked()));

  connect(m_ui->m_addValueOpToolButton, SIGNAL(clicked()), this, SLOT(addValueOpToolButtonClicked()));
  connect(m_ui->m_addRasterOpToolButton, SIGNAL(clicked()), this, SLOT(addRasterOpToolButtonClicked()));

  connect(m_ui->m_undoToolButton, SIGNAL(clicked()), this, SLOT(undoToolButtonClicked()));
  connect(m_ui->m_redoToolButton, SIGNAL(clicked()), this, SLOT(redoToolButtonClicked()));
  connect(m_ui->m_clearToolButton, SIGNAL(clicked()), this, SLOT(clearToolButtonClicked()));
  connect(m_ui->m_validateToolButton, SIGNAL(clicked()), this, SLOT(validateToolButtonClicked()));

}

te::qt::widgets::ArithmeticOpWizardPage::~ArithmeticOpWizardPage()
{
  m_expressionHistory.clear();
}

bool te::qt::widgets::ArithmeticOpWizardPage::isComplete() const
{
  return true;
}


void te::qt::widgets::ArithmeticOpWizardPage::setList(std::list<te::map::AbstractLayerPtr>& layerList)
{
  m_layerList = layerList;

  //fill layer combo
  m_ui->m_layerComboBox->clear();
  m_ui->m_layerAComboBox->clear();
  m_ui->m_layerBComboBox->clear();
  m_ui->m_rasterIdComboBox->clear();

  std::list<te::map::AbstractLayerPtr>::iterator it = m_layerList.begin();
  std::vector<std::size_t> bands;

  int count = 0;

  while(it != m_layerList.end())
  {
    te::map::AbstractLayerPtr l = *it;

    std::auto_ptr<te::da::DataSetType> dst = l->getSchema();

    if(dst.get() && dst->hasRaster())
    {
      m_ui->m_layerComboBox->addItem(l->getTitle().c_str(), QVariant::fromValue(l));
      m_ui->m_layerAComboBox->addItem(l->getTitle().c_str(), QVariant::fromValue(l));
      m_ui->m_layerBComboBox->addItem(l->getTitle().c_str(), QVariant::fromValue(l));

      m_ui->m_rasterIdComboBox->addItem("R" + QString::number(count));

      ++count;
    }

    ++it;
  }

  layerComboBoxActivated(0);
  layerAComboBoxActivated(0);
  layerBComboBoxActivated(0);
}

int te::qt::widgets::ArithmeticOpWizardPage::getOperationType()
{
  int type = -1;

  if(m_ui->m_tabWidget->currentIndex() == 1)
  {
    type = te::qt::widgets::ArithmeticOpWizard::ARITH_OP_TYPE_USER_DEFINED;
  }
  else if(m_ui->m_op1RadioButton->isChecked())
    type = te::qt::widgets::ArithmeticOpWizard::ARITH_OP_TYPE_1;
  else if(m_ui->m_op2RadioButton->isChecked())
    type = te::qt::widgets::ArithmeticOpWizard::ARITH_OP_TYPE_2;
  else if(m_ui->m_op3RadioButton->isChecked())
    type = te::qt::widgets::ArithmeticOpWizard::ARITH_OP_TYPE_3;
  else if(m_ui->m_op4RadioButton->isChecked())
    type = te::qt::widgets::ArithmeticOpWizard::ARITH_OP_TYPE_4;
  else if(m_ui->m_op5RadioButton->isChecked())
    type = te::qt::widgets::ArithmeticOpWizard::ARITH_OP_TYPE_5;
  else if(m_ui->m_op6RadioButton->isChecked())
    type = te::qt::widgets::ArithmeticOpWizard::ARITH_OP_TYPE_6;


  return type;
}

te::map::AbstractLayerPtr te::qt::widgets::ArithmeticOpWizardPage::getLayerRasterA()
{
  int curIdx = m_ui->m_layerAComboBox->currentIndex();
  QVariant varLayer = m_ui->m_layerAComboBox->itemData(curIdx, Qt::UserRole);
  te::map::AbstractLayerPtr l = varLayer.value<te::map::AbstractLayerPtr>();

  return l;
}

int te::qt::widgets::ArithmeticOpWizardPage::getLayerBandA()
{
  return m_ui->m_bandAComboBox->currentText().toInt();
}

te::map::AbstractLayerPtr te::qt::widgets::ArithmeticOpWizardPage::getLayerRasterB()
{
  int curIdx = m_ui->m_layerBComboBox->currentIndex();
  QVariant varLayer = m_ui->m_layerBComboBox->itemData(curIdx, Qt::UserRole);
  te::map::AbstractLayerPtr l = varLayer.value<te::map::AbstractLayerPtr>();

  return l;
}

int te::qt::widgets::ArithmeticOpWizardPage::getLayerBandB()
{
  return m_ui->m_bandBComboBox->currentText().toInt();
}

double te::qt::widgets::ArithmeticOpWizardPage::getGainValue()
{
  double val = 1.;

  if(!m_ui->m_gainLineEdit->text().isEmpty())
    val = m_ui->m_gainLineEdit->text().toDouble();

  return val;
}

double te::qt::widgets::ArithmeticOpWizardPage::getOffSetValue()
{
  double val = 0.;

  if(!m_ui->m_offsetLineEdit->text().isEmpty())
    val = m_ui->m_offsetLineEdit->text().toDouble();

  return val;
}

bool te::qt::widgets::ArithmeticOpWizardPage::normalize()
{
  return m_ui->m_normalizeCheckBox->isChecked();
}

std::string te::qt::widgets::ArithmeticOpWizardPage::getUserDefinedExpression()
{
  return m_ui->m_expressionLineEdit->text().toLatin1().data();
}

void te::qt::widgets::ArithmeticOpWizardPage::layerComboBoxActivated(int index)
{
  getRasterBands(m_ui->m_layerComboBox, index, m_ui->m_bandComboBox);

  m_ui->m_rasterIdComboBox->setCurrentIndex(index);
}

void te::qt::widgets::ArithmeticOpWizardPage::layerAComboBoxActivated(int index)
{
  getRasterBands(m_ui->m_layerAComboBox, index, m_ui->m_bandAComboBox);
}

void te::qt::widgets::ArithmeticOpWizardPage::layerBComboBoxActivated(int index)
{
  getRasterBands(m_ui->m_layerBComboBox, index, m_ui->m_bandBComboBox);
}

void te::qt::widgets::ArithmeticOpWizardPage::leftBracketToolButtonClicked()
{
  m_ui->m_expressionLineEdit->insert("( ");

  updateExpressionHistory();
}

void te::qt::widgets::ArithmeticOpWizardPage::rightBracketToolButtonClicked()
{
  m_ui->m_expressionLineEdit->insert(") ");

  updateExpressionHistory();
}

void te::qt::widgets::ArithmeticOpWizardPage::plusToolButtonClicked()
{
  m_ui->m_expressionLineEdit->insert("+ ");

  updateExpressionHistory();
}

void te::qt::widgets::ArithmeticOpWizardPage::minusToolButtonClicked()
{
  m_ui->m_expressionLineEdit->insert("- ");

  updateExpressionHistory();
}

void te::qt::widgets::ArithmeticOpWizardPage::multiToolButtonClicked()
{
  m_ui->m_expressionLineEdit->insert("* ");

  updateExpressionHistory();
}

void te::qt::widgets::ArithmeticOpWizardPage::divToolButtonClicked()
{
  m_ui->m_expressionLineEdit->insert("/ ");

  updateExpressionHistory();
}

void te::qt::widgets::ArithmeticOpWizardPage::addValueOpToolButtonClicked()
{
  m_ui->m_expressionLineEdit->insert(m_ui->m_valueLineEdit->text() + " ");

  updateExpressionHistory();
}

void te::qt::widgets::ArithmeticOpWizardPage::addRasterOpToolButtonClicked()
{
  m_ui->m_expressionLineEdit->insert(m_ui->m_rasterIdComboBox->currentText() + ":" + m_ui->m_bandComboBox->currentText() + " ");

  updateExpressionHistory();
}

void te::qt::widgets::ArithmeticOpWizardPage::undoToolButtonClicked()
{
  --m_expressionHistoryCounter;

  if(m_expressionHistoryCounter >= (int)m_expressionHistory.size() || m_expressionHistoryCounter < 0)
  {
    m_expressionHistoryCounter = 0;

    m_expressionHistory.clear();
    
    m_ui->m_expressionLineEdit->clear();

    m_ui->m_undoToolButton->setEnabled(false);
    m_ui->m_redoToolButton->setEnabled(false);

    return;
  }

  std::string expression = m_expressionHistory[m_expressionHistoryCounter];

  m_ui->m_expressionLineEdit->setText(expression.c_str());

  if(m_expressionHistoryCounter < 0)
    m_ui->m_undoToolButton->setEnabled(false);

  m_ui->m_redoToolButton->setEnabled(true);
}

void te::qt::widgets::ArithmeticOpWizardPage::redoToolButtonClicked()
{
  ++m_expressionHistoryCounter;

  if(m_expressionHistoryCounter >= (int)m_expressionHistory.size() || m_expressionHistoryCounter < 0)
  {
    m_expressionHistoryCounter = (int)m_expressionHistory.size() - 1;

    m_ui->m_redoToolButton->setEnabled(false);

    return;
  }

  std::string expression = m_expressionHistory[m_expressionHistoryCounter];

  m_ui->m_expressionLineEdit->setText(expression.c_str());

  if(m_expressionHistoryCounter >=  (int)m_expressionHistory.size() - 1)
    m_ui->m_redoToolButton->setEnabled(false);

  m_ui->m_undoToolButton->setEnabled(true);
}

void te::qt::widgets::ArithmeticOpWizardPage::clearToolButtonClicked()
{
  m_expressionHistoryCounter = 0;

  m_expressionHistory.clear();
    
  m_ui->m_expressionLineEdit->clear();

  m_ui->m_undoToolButton->setEnabled(false);
  m_ui->m_redoToolButton->setEnabled(false);
}

void te::qt::widgets::ArithmeticOpWizardPage::validateToolButtonClicked()
{
}

void te::qt::widgets::ArithmeticOpWizardPage::getRasterBands(QComboBox* layer, int index, QComboBox* band)
{
  //get layer
  QVariant varLayer = layer->itemData(index, Qt::UserRole);
  te::map::AbstractLayerPtr l = varLayer.value<te::map::AbstractLayerPtr>();

  //get raster
  std::auto_ptr<te::da::DataSet> ds = l->getData();
  std::size_t rpos = te::da::GetFirstPropertyPos(ds.get(), te::dt::RASTER_TYPE);
  std::auto_ptr<te::rst::Raster> rst = ds->getRaster(rpos);

  //fill band info
  if(rst.get())
  {
    band->clear();

    for(std::size_t t = 0; t < rst->getNumberOfBands(); ++t)
      band->addItem(QString::number(t));
  }
}

void te::qt::widgets::ArithmeticOpWizardPage::updateExpressionHistory()
{
  m_expressionHistory.push_back(m_ui->m_expressionLineEdit->text().toLatin1().data());

  m_ui->m_undoToolButton->setEnabled(true);
  m_ui->m_redoToolButton->setEnabled(false);

  m_expressionHistoryCounter = (int)m_expressionHistory.size() - 1;
}
