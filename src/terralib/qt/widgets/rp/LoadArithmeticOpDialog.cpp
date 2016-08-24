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
  \file terralib/qt/widgets/rp/LoadArithmeticOpDialog.cpp

  \brief This file defines a class for a ArithmeticOp Dialog.
*/

// TerraLib
#include "../../../common/PlatformUtils.h"
#include "../../../common/StringUtils.h"
#include "../../../core/translator/Translator.h"
#include "../../../core/utils/Platform.h"
#include "../../../dataaccess/dataset/DataSet.h"
#include "../../../dataaccess/utils/Utils.h"
#include "LoadArithmeticOpDialog.h"
#include "ui_LoadArithmeticOpDialogForm.h"

//Boost
#include <boost/foreach.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/property_tree/json_parser.hpp>

// Qt
#include <QApplication>
#include <QGridLayout>
#include <QMessageBox>
#include <QComboBox>

// stl
#include <memory>

Q_DECLARE_METATYPE(te::map::AbstractLayerPtr);

te::qt::widgets::LoadArithmeticOpDialog::LoadArithmeticOpDialog(QWidget* parent, Qt::WindowFlags f)
  : QDialog(parent, f),
  m_ui(new Ui::LoadArithmeticOpDialogForm)
{
// setup controls
  m_ui->setupUi(this);

  //connects
  connect(m_ui->m_operationComboBox, SIGNAL(activated(QString)), this, SLOT(onOperationComboBoxActivated(QString)));
  connect(m_ui->m_okPushButton, SIGNAL(clicked()), this, SLOT(onOkPushButtonClicked()));
}

te::qt::widgets::LoadArithmeticOpDialog::~LoadArithmeticOpDialog()
{

}

bool te::qt::widgets::LoadArithmeticOpDialog::loadOperations()
{
  std::string jsonfile = te::core::FindInTerraLibPath("share/terralib/json/arithmeticOperations.json");

  if (jsonfile.empty())
    throw te::common::Exception(TE_TR("Could not find arithmeticOperations.json file!"));

  // Reading JSON
  boost::property_tree::ptree pt;
  boost::property_tree::json_parser::read_json(jsonfile, pt);

  BOOST_FOREACH(boost::property_tree::ptree::value_type &v, pt.get_child("operations"))
  {
    std::string op = v.second.get<std::string>("operation");
    m_ui->m_operationComboBox->addItem(op.c_str());
  }
}

void te::qt::widgets::LoadArithmeticOpDialog::setList(std::list<te::map::AbstractLayerPtr>& layerList)
{
  m_layerList = layerList;
}

std::string te::qt::widgets::LoadArithmeticOpDialog::getExpression()
{
  return m_operationExt;
}

std::map<std::string, te::map::AbstractLayer*> te::qt::widgets::LoadArithmeticOpDialog::getLayers()
{
  return m_mapLayer;
}

std::string te::qt::widgets::LoadArithmeticOpDialog::getOperation()
{
  return m_operation;
}

void te::qt::widgets::LoadArithmeticOpDialog::onOperationComboBoxActivated(QString operation)
{
  int r = 0;

  m_layersComboBox.clear();
  m_bandsComboBox.clear();
  m_ui->m_layersTableWidget->setRowCount(0);

  m_ui->m_gainLineEdit->setEnabled(false);
  m_ui->m_offsetLineEdit->setEnabled(false);
  m_ui->m_gainLineEdit->setText("");
  m_ui->m_offsetLineEdit->setText("");

  std::vector<std::string> arithExpVec;
  boost::split(arithExpVec, operation.toStdString(), boost::is_any_of(" "));

  for (int i = 0; i < arithExpVec.size(); i++)
  {
    if (arithExpVec[i].find(":") != -1)
    {
      std::vector<std::string> rasterVec;
      boost::split(rasterVec, arithExpVec[i], boost::is_any_of(":"));

      bool exist = false;
      for (int j = 0; j < m_ui->m_layersTableWidget->rowCount(); j++)
      {
        std::string header = m_ui->m_layersTableWidget->verticalHeaderItem(j)->text().toStdString();
        if (rasterVec[0] == header)
          exist = true;
      }

      if (!exist)
      {
        m_ui->m_layersTableWidget->setRowCount(r + 1);

        QTableWidgetItem* item = new QTableWidgetItem(rasterVec[0].c_str());
        m_ui->m_layersTableWidget->setVerticalHeaderItem(r, item);

        QComboBox* layerComboBox = new QComboBox(m_ui->m_layersTableWidget);
        m_layersComboBox.push_back(layerComboBox);

        QComboBox* bandComboBox = new QComboBox(m_ui->m_layersTableWidget);
        m_bandsComboBox.push_back(bandComboBox);

        connect(m_layersComboBox[r], SIGNAL(activated(int)), this, SLOT(onLayerComboBoxActivated(int)));

        r++;
      }
    }
    else if (te::common::Convert2LCase(arithExpVec[i]) == "gain")
    {
      m_ui->m_gainLineEdit->setEnabled(true);
      m_ui->m_gainLineEdit->setText("1");
    }
    else if (te::common::Convert2LCase(arithExpVec[i]) == "offset")
    {
      m_ui->m_offsetLineEdit->setEnabled(true);
      m_ui->m_offsetLineEdit->setText("0");
    }
  }

  std::list<te::map::AbstractLayerPtr>::iterator it = m_layerList.begin();
  std::vector<std::size_t> bands;

  m_layers.clear();

  int count = 0;

  while (it != m_layerList.end())
  {
    te::map::AbstractLayerPtr l = *it;

    std::auto_ptr<te::da::DataSet> ds = l->getData();
    std::auto_ptr<te::da::DataSetType> dst = l->getSchema();

    if (dst.get() && dst->hasRaster())
    {
      if (count == 0)
      {
        std::size_t rpos = te::da::GetFirstPropertyPos(ds.get(), te::dt::RASTER_TYPE);
        std::auto_ptr<te::rst::Raster> inputRst = ds->getRaster(rpos);

        int nbands = inputRst->getNumberOfBands();

        for (int i = 0; i < m_layersComboBox.size(); i++)
        {
          for (int b = 0; b < nbands; b++)
            m_bandsComboBox[i]->addItem(te::common::Convert2String(b).c_str());
        }
      }

      m_layers.push_back(l);

      for (int i = 0; i < m_layersComboBox.size(); i++)
      {
        m_layersComboBox[i]->addItem(l->getTitle().c_str(), QVariant::fromValue(l));
      }

      ++count;
    }

    ++it;
  }

  for (int i = 0; i < m_layersComboBox.size(); i++)
  {
    m_ui->m_layersTableWidget->setCellWidget(i, 0, m_layersComboBox[i]);
    m_ui->m_layersTableWidget->setCellWidget(i, 1, m_bandsComboBox[i]);
  }

  m_ui->m_layersTableWidget->resizeColumnsToContents();
}

void te::qt::widgets::LoadArithmeticOpDialog::onLayerComboBoxActivated(int index)
{
  int row = -1;

  int nrows = m_ui->m_layersTableWidget->rowCount();
  for (int i = 0; i < nrows; i++)
  {
    if (QObject::sender() == m_layersComboBox[i])
    {
      row = i;
      break;
    }
  }

  te::map::AbstractLayerPtr layer = m_layers[index];

  std::auto_ptr<te::da::DataSet> ds = layer->getData();
  std::auto_ptr<te::da::DataSetType> dst = layer->getSchema();

  if (dst.get() && dst->hasRaster())
  {
    std::size_t rpos = te::da::GetFirstPropertyPos(ds.get(), te::dt::RASTER_TYPE);
    std::auto_ptr<te::rst::Raster> inputRst = ds->getRaster(rpos);

    m_bandsComboBox[row]->clear();

    int nbands = inputRst->getNumberOfBands();

    for (int i = 0; i < nbands; i++)
    {
      m_bandsComboBox[row]->addItem(te::common::Convert2String(i).c_str());
    }
  }
}

void te::qt::widgets::LoadArithmeticOpDialog::onOkPushButtonClicked()
{
  if (m_ui->m_gainLineEdit->isEnabled() && m_ui->m_gainLineEdit->text().isEmpty())
  {
    QMessageBox::warning(this, tr("Warning"), tr("Gain value is not defined."));
    return;
  }

  if (m_ui->m_offsetLineEdit->isEnabled() && m_ui->m_offsetLineEdit->text().isEmpty())
  {
    QMessageBox::warning(this, tr("Warning"), tr("Offset value is not defined."));
    return;
  }

  m_operationExt = "";
  m_operation = "";

  std::string operation = m_ui->m_operationComboBox->currentText().toStdString();

  std::vector<std::string> arithExpVec;
  boost::split(arithExpVec, operation, boost::is_any_of(" "));

  for (int i = 0; i < arithExpVec.size(); i++)
  {
    if (te::common::Convert2LCase(arithExpVec[i]) == "gain")
    {
      m_operationExt += " " + m_ui->m_gainLineEdit->text().toStdString();
      m_operation += " " + m_ui->m_gainLineEdit->text().toStdString();
    }
    else if (te::common::Convert2LCase(arithExpVec[i]) == "offset")
    {
      m_operationExt += " " + m_ui->m_offsetLineEdit->text().toStdString();
      m_operation += " " + m_ui->m_offsetLineEdit->text().toStdString();
    }
    else
    {
      std::vector<std::string> layerVec;
      boost::split(layerVec, arithExpVec[i], boost::is_any_of(":"));

      if (layerVec.size() == 1)
      {
        m_operationExt += " " + arithExpVec[i];
        m_operation += " " + arithExpVec[i];
      }
      else
      {
        int nrows = m_ui->m_layersTableWidget->rowCount();

        for (int j = 0; j < nrows; j++)
        {
          std::string header = m_ui->m_layersTableWidget->verticalHeaderItem(j)->text().toStdString();

          if (header == layerVec[0])
          {
            int index = m_layersComboBox[j]->currentIndex();
            m_mapLayer.insert(std::map<std::string, te::map::AbstractLayer*>::value_type(header, m_layers[index].get()));

            m_operationExt += " " + m_layersComboBox[j]->currentText().toStdString() + ":B" + m_bandsComboBox[j]->currentText().toStdString();
            m_operation += " " + m_ui->m_layersTableWidget->verticalHeaderItem(j)->text().toStdString() + ":" + m_bandsComboBox[j]->currentText().toStdString();
            break;
          }
        }
      }
    }
  }

  accept();
}
