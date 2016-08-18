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
  \file terralib/qt/widgets/rp/ArithmeticOpDialogForm.cpp

  \brief This file defines a class for a ArithmeticOp Dialog.
*/

// TerraLib
#include "../../../common/progress/ProgressManager.h"
#include "../../../common/StringUtils.h"
#include "../../../core/translator/Translator.h"
#include "../../../core/utils/Platform.h"
#include "../../../dataaccess/dataset/DataSet.h"
#include "../../../dataaccess/datasource/DataSourceInfo.h"
#include "../../../dataaccess/utils/Utils.h"
#include "../../../raster/Raster.h"
#include "../../../raster/RasterProperty.h"
#include "../../../rp/Module.h"
#include "../../widgets/progress/ProgressViewerDialog.h"
#include "../datasource/selector/DataSourceSelectorDialog.h"
#include "../layer/search/LayerSearchDialog.h"
#include "../layer/search/LayerSearchWidget.h"
#include "LoadArithmeticOpDialog.h"
#include "OutputRasterWidget.h"
#include "ArithmeticOpDialogForm.h"
#include "ui_ArithmeticOpDialogForm.h"
#include "Utils.h"

//Boost
#include <boost/filesystem.hpp>
#include <boost/foreach.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/property_tree/json_parser.hpp>

// Qt
#include <QApplication>
#include <QGridLayout>
#include <QMessageBox>
#include <QFileDialog>
#include <QLineEdit>
#include <QTextEdit>

// stl
#include <memory>

Q_DECLARE_METATYPE(te::map::AbstractLayerPtr);

te::qt::widgets::ArithmeticOpDialogForm::ArithmeticOpDialogForm(QWidget* parent)
  : QDialog(parent),
  m_ui(new Ui::ArithmeticOpDialogForm)
{
// setup controls
  m_ui->setupUi(this);

  //build output parameters
  QGridLayout* outputLayout = new QGridLayout(m_ui->m_outputWidget);
  outputLayout->setContentsMargins(0, 0, 0, 0);
  m_outputWidget = new te::qt::widgets::OutputRasterWidget(m_ui->m_outputWidget);
  outputLayout->addWidget(m_outputWidget);

  //set icons
  m_ui->m_addLayerPushButton->setIcon(QIcon::fromTheme("list-add"));
  m_ui->m_loadPushButton->setIcon(QIcon::fromTheme("document-open"));
  m_ui->m_savePushButton->setIcon(QIcon::fromTheme("document-save"));

  m_expressionHistoryCounter = -1;
  m_nLayer = 0;
  m_operation = "";
  
  connect(m_ui->m_searchLayerToolButton, SIGNAL(pressed()), this, SLOT(onSearchLayerToolButtonPressed()));
  connect(m_ui->m_addLayerPushButton, SIGNAL(clicked()), this, SLOT(onAddLayerPushButtonClicked()));

  connect(m_ui->m_sqrtPushButton, SIGNAL(clicked()), this, SLOT(onSqrtPushButtonClicked()));
  connect(m_ui->m_parRightPushButton, SIGNAL(clicked()), this, SLOT(onParRightPushButtonClicked()));
  connect(m_ui->m_parLeftPushButton, SIGNAL(clicked()), this, SLOT(onParLeftPushButtonClicked()));
  connect(m_ui->m_sinPushButton, SIGNAL(clicked()), this, SLOT(onSinPushButtonClicked()));
  connect(m_ui->m_asinPushButton, SIGNAL(clicked()), this, SLOT(onASinPushButtonClicked()));
  connect(m_ui->m_expPushButton, SIGNAL(clicked()), this, SLOT(onExpPushButtonClicked()));
  connect(m_ui->m_cosPushButton, SIGNAL(clicked()), this, SLOT(onCosPushButtonClicked()));
  connect(m_ui->m_acosPushButton, SIGNAL(clicked()), this, SLOT(onACosPushButtonClicked()));
  connect(m_ui->m_logPushButton, SIGNAL(clicked()), this, SLOT(onLogPushButtonClicked()));
  connect(m_ui->m_tanPushButton, SIGNAL(clicked()), this, SLOT(onTanPushButtonClicked()));
  connect(m_ui->m_atanPushButton, SIGNAL(clicked()), this, SLOT(onATanPushButtonClicked()));
  connect(m_ui->m_lnPushButton, SIGNAL(clicked()), this, SLOT(onLnPushButtonClicked()));
  connect(m_ui->m_n0PushButton, SIGNAL(clicked()), this, SLOT(onN0PushButtonClicked()));
  connect(m_ui->m_n1PushButton, SIGNAL(clicked()), this, SLOT(onN1PushButtonClicked()));
  connect(m_ui->m_n2PushButton, SIGNAL(clicked()), this, SLOT(onN2PushButtonClicked()));
  connect(m_ui->m_n3PushButton, SIGNAL(clicked()), this, SLOT(onN3PushButtonClicked()));
  connect(m_ui->m_n4PushButton, SIGNAL(clicked()), this, SLOT(onN4PushButtonClicked()));
  connect(m_ui->m_n5PushButton, SIGNAL(clicked()), this, SLOT(onN5PushButtonClicked()));
  connect(m_ui->m_n6PushButton, SIGNAL(clicked()), this, SLOT(onN6PushButtonClicked()));
  connect(m_ui->m_n7PushButton, SIGNAL(clicked()), this, SLOT(onN7PushButtonClicked()));
  connect(m_ui->m_n8PushButton, SIGNAL(clicked()), this, SLOT(onN8PushButtonClicked()));
  connect(m_ui->m_n9PushButton, SIGNAL(clicked()), this, SLOT(onN9PushButtonClicked()));
  connect(m_ui->m_dotPushButton, SIGNAL(clicked()), this, SLOT(onDotPushButtonClicked()));
  connect(m_ui->m_signalPushButton, SIGNAL(clicked()), this, SLOT(onSignalPushButtonClicked()));
  connect(m_ui->m_divPushButton, SIGNAL(clicked()), this, SLOT(onDivPushButtonClicked()));
  connect(m_ui->m_multPushButton, SIGNAL(clicked()), this, SLOT(onMultPushButtonClicked()));
  connect(m_ui->m_subPushButton, SIGNAL(clicked()), this, SLOT(onSubPushButtonClicked()));
  connect(m_ui->m_addPushButton, SIGNAL(clicked()), this, SLOT(onAddPushButtonClicked()));
  connect(m_ui->m_cePushButton, SIGNAL(clicked()), this, SLOT(onCePushButtonClicked()));
  connect(m_ui->m_cPushButton, SIGNAL(clicked()), this, SLOT(onCPushButtonClicked()));
  connect(m_ui->m_loadPushButton, SIGNAL(clicked()), this, SLOT(onLoadPushButtonClicked()));
  connect(m_ui->m_savePushButton, SIGNAL(clicked()), this, SLOT(onSavePushButtonClicked()));

  connect(m_ui->m_okPushButton, SIGNAL(clicked()), this, SLOT(onOkPushButtonClicked()));
}

te::qt::widgets::ArithmeticOpDialogForm::~ArithmeticOpDialogForm()
{
  m_expressionHistory.clear();
}

bool te::qt::widgets::ArithmeticOpDialogForm::isComplete() const
{
  return true;
}

void te::qt::widgets::ArithmeticOpDialogForm::setList(std::list<te::map::AbstractLayerPtr>& layerList)
{
  m_layerList = layerList;
}

std::string te::qt::widgets::ArithmeticOpDialogForm::getUserDefinedExpression()
{
  return m_ui->m_operationTextEdit->toPlainText().toLatin1();
}

std::string te::qt::widgets::ArithmeticOpDialogForm::getExpression()
{
  return m_operation;
}

te::map::AbstractLayerPtr te::qt::widgets::ArithmeticOpDialogForm::getOutputLayer()
{
  return m_outputLayer;
}

std::map<std::string, te::map::AbstractLayer*> te::qt::widgets::ArithmeticOpDialogForm::getMapLayer()
{
  return m_mapLayer;
}

std::vector<std::string> te::qt::widgets::ArithmeticOpDialogForm::getBandsVec()
{
  return m_bandsVec;
}

void te::qt::widgets::ArithmeticOpDialogForm::onAddLayerPushButtonClicked()
{
  if (m_ui->m_inputLayerLineEdit->text().isEmpty())
    return;

  QString operation = m_ui->m_operationTextEdit->toPlainText() + m_ui->m_inputLayerLineEdit->text() + ":" + m_ui->m_bandComboBox->currentText();

  m_ui->m_operationTextEdit->setText(operation);

  std::string nLayer = "R" + te::common::Convert2String(m_nLayer);
  m_mapLayer.insert(std::map<std::string, te::map::AbstractLayer*>::value_type(nLayer, m_layer));
  std::string band = m_ui->m_bandComboBox->currentText().toLatin1();
  m_bandsVec.push_back(band);

  m_nLayer++;
}

void te::qt::widgets::ArithmeticOpDialogForm::onSearchLayerToolButtonPressed()
{
  te::qt::widgets::LayerSearchDialog* dlg = new te::qt::widgets::LayerSearchDialog(this);
  dlg->getWidget()->filterOnlyByRaster();
  dlg->getWidget()->setList(m_layerList);
  if (dlg->exec())
  {
    m_ui->m_bandComboBox->clear();

    std::list<te::map::AbstractLayerPtr> layerList = dlg->getWidget()->getSelecteds();

    std::list<te::map::AbstractLayerPtr>::iterator it = layerList.begin();

    while (it != layerList.end())
    {
      m_layer = it->get();

      std::string layerTitle = it->get()->getTitle();
      m_ui->m_inputLayerLineEdit->setText(layerTitle.c_str());

      std::auto_ptr<te::da::DataSetType> dsType = it->get()->getSchema();
      te::rst::RasterProperty* rasterProp = te::da::GetFirstRasterProperty(dsType.get());
      std::auto_ptr<te::da::DataSet> dsRaster = it->get()->getData();
      std::auto_ptr<te::rst::Raster> raster = dsRaster->getRaster(rasterProp->getName());

      for (int b = 0; b < raster->getNumberOfBands(); b++)
      {
        m_ui->m_bandComboBox->addItem(te::common::Convert2String(b).c_str());
      }

      it++;
    }
  }
}

void te::qt::widgets::ArithmeticOpDialogForm::onSqrtPushButtonClicked()
{
  QString txt = m_ui->m_operationTextEdit->toPlainText();

  if (!txt.isEmpty() && txt.at(txt.size() - 1) == ' ')
    txt.replace(txt.size() - 1, txt.size(), "");

  m_ui->m_operationTextEdit->setText(txt + " sqrt( ");
}

void te::qt::widgets::ArithmeticOpDialogForm::onParRightPushButtonClicked()
{
  QString txt = m_ui->m_operationTextEdit->toPlainText();

  if (!txt.isEmpty() && txt.at(txt.size() - 1) == ' ')
    txt.replace(txt.size() - 1, txt.size(), "");

  m_ui->m_operationTextEdit->setText(txt + " ( ");
}

void te::qt::widgets::ArithmeticOpDialogForm::onParLeftPushButtonClicked()
{
  QString txt = m_ui->m_operationTextEdit->toPlainText();

  if (!txt.isEmpty() && txt.at(txt.size() - 1) == ' ')
    txt.replace(txt.size() - 1, txt.size(), "");

  m_ui->m_operationTextEdit->setText(txt + " ) ");
}

void te::qt::widgets::ArithmeticOpDialogForm::onSinPushButtonClicked()
{
  QString txt = m_ui->m_operationTextEdit->toPlainText();

  if (!txt.isEmpty() && txt.at(txt.size() - 1) == ' ')
    txt.replace(txt.size() - 1, txt.size(), "");

  m_ui->m_operationTextEdit->setText(txt + " sin( ");
}

void te::qt::widgets::ArithmeticOpDialogForm::onASinPushButtonClicked()
{
  QString txt = m_ui->m_operationTextEdit->toPlainText();

  if (!txt.isEmpty() && txt.at(txt.size() - 1) == ' ')
    txt.replace(txt.size() - 1, txt.size(), "");

  m_ui->m_operationTextEdit->setText(txt + " asin( ");
}

void te::qt::widgets::ArithmeticOpDialogForm::onExpPushButtonClicked()
{
  QString txt = m_ui->m_operationTextEdit->toPlainText();

  if (txt.isEmpty())
    return;

  if (txt.at(txt.size() - 1) == ' ')
    txt.replace(txt.size() - 1, txt.size(), "");

  m_ui->m_operationTextEdit->setText(txt + " ^ ");
}

void te::qt::widgets::ArithmeticOpDialogForm::onCosPushButtonClicked()
{
  QString txt = m_ui->m_operationTextEdit->toPlainText();

  if (!txt.isEmpty() && txt.at(txt.size() - 1) == ' ')
    txt.replace(txt.size() - 1, txt.size(), "");

  m_ui->m_operationTextEdit->setText(txt + " cos( ");
}

void te::qt::widgets::ArithmeticOpDialogForm::onACosPushButtonClicked()
{
  QString txt = m_ui->m_operationTextEdit->toPlainText();

  if (!txt.isEmpty() && txt.at(txt.size() - 1) == ' ')
    txt.replace(txt.size() - 1, txt.size(), "");

  m_ui->m_operationTextEdit->setText(txt + " acos( ");
}

void te::qt::widgets::ArithmeticOpDialogForm::onLogPushButtonClicked()
{
  QString txt = m_ui->m_operationTextEdit->toPlainText();

  if (!txt.isEmpty() && txt.at(txt.size() - 1) == ' ')
    txt.replace(txt.size() - 1, txt.size(), "");

  m_ui->m_operationTextEdit->setText(txt + " log( ");
}

void te::qt::widgets::ArithmeticOpDialogForm::onTanPushButtonClicked()
{
  QString txt = m_ui->m_operationTextEdit->toPlainText();

  if (!txt.isEmpty() && txt.at(txt.size() - 1) == ' ')
    txt.replace(txt.size() - 1, txt.size(), "");

  m_ui->m_operationTextEdit->setText(txt + " tan( ");
}

void te::qt::widgets::ArithmeticOpDialogForm::onATanPushButtonClicked()
{
  QString txt = m_ui->m_operationTextEdit->toPlainText();

  if (!txt.isEmpty() && txt.at(txt.size() - 1) == ' ')
    txt.replace(txt.size() - 1, txt.size(), "");

  m_ui->m_operationTextEdit->setText(txt + " atan( ");
}

void te::qt::widgets::ArithmeticOpDialogForm::onLnPushButtonClicked()
{
  QString txt = m_ui->m_operationTextEdit->toPlainText();

  if (!txt.isEmpty() && txt.at(txt.size() - 1) == ' ')
    txt.replace(txt.size() - 1, txt.size(), "");

  m_ui->m_operationTextEdit->setText(txt + " ln( ");
}

void te::qt::widgets::ArithmeticOpDialogForm::onN0PushButtonClicked()
{
  QString txt = m_ui->m_operationTextEdit->toPlainText();
  m_ui->m_operationTextEdit->setText(txt + "0");
}

void te::qt::widgets::ArithmeticOpDialogForm::onN1PushButtonClicked()
{
  QString txt = m_ui->m_operationTextEdit->toPlainText();
  m_ui->m_operationTextEdit->setText(txt + "1");
}

void te::qt::widgets::ArithmeticOpDialogForm::onN2PushButtonClicked()
{
  QString txt = m_ui->m_operationTextEdit->toPlainText();
  m_ui->m_operationTextEdit->setText(txt + "2");
}

void te::qt::widgets::ArithmeticOpDialogForm::onN3PushButtonClicked()
{
  QString txt = m_ui->m_operationTextEdit->toPlainText();
  m_ui->m_operationTextEdit->setText(txt + "3");
}

void te::qt::widgets::ArithmeticOpDialogForm::onN4PushButtonClicked()
{
  QString txt = m_ui->m_operationTextEdit->toPlainText();
  m_ui->m_operationTextEdit->setText(txt + "4");
}

void te::qt::widgets::ArithmeticOpDialogForm::onN5PushButtonClicked()
{
  QString txt = m_ui->m_operationTextEdit->toPlainText();
  m_ui->m_operationTextEdit->setText(txt + "5");
}

void te::qt::widgets::ArithmeticOpDialogForm::onN6PushButtonClicked()
{
  QString txt = m_ui->m_operationTextEdit->toPlainText();
  m_ui->m_operationTextEdit->setText(txt + "6");
}

void te::qt::widgets::ArithmeticOpDialogForm::onN7PushButtonClicked()
{
  QString txt = m_ui->m_operationTextEdit->toPlainText();
  m_ui->m_operationTextEdit->setText(txt + "7");
}

void te::qt::widgets::ArithmeticOpDialogForm::onN8PushButtonClicked()
{
  QString txt = m_ui->m_operationTextEdit->toPlainText();
  m_ui->m_operationTextEdit->setText(txt + "8");
}

void te::qt::widgets::ArithmeticOpDialogForm::onN9PushButtonClicked()
{
  QString txt = m_ui->m_operationTextEdit->toPlainText();
  m_ui->m_operationTextEdit->setText(txt + "9");
}

void te::qt::widgets::ArithmeticOpDialogForm::onDotPushButtonClicked()
{
  QString txt = m_ui->m_operationTextEdit->toPlainText();
  m_ui->m_operationTextEdit->setText(txt + ".");
}

void te::qt::widgets::ArithmeticOpDialogForm::onSignalPushButtonClicked()
{
  if (m_ui->m_operationTextEdit->toPlainText().isEmpty())
    return;

  std::string operation = m_ui->m_operationTextEdit->toPlainText().toLatin1();

  std::string txt = "";
  int i;
  int decimal = 0;
  for (i = operation.size()-1; i >= 0; i--)
  {
    if (operation.at(i) == ' ' && i != operation.size() - 1)
    {
      break;
    }

    if (operation.at(i) == '.')
      decimal = operation.size() - 1 - i;

    txt = operation.at(i) + txt;
  }

  QString txt1 = txt.c_str();
  bool isOk;
  double val = txt1.toDouble(&isOk);

  if (!isOk)
    return;

  val *= -1;

  if (i == -1)
  {
    operation.clear();
    operation = te::common::Convert2String(val, decimal);
  }
  else
    operation.replace(i+1, operation.size() - 1, te::common::Convert2String(val, decimal));

  m_ui->m_operationTextEdit->clear();
  m_ui->m_operationTextEdit->setText(operation.c_str());
}

void te::qt::widgets::ArithmeticOpDialogForm::onDivPushButtonClicked()
{
  QString txt = m_ui->m_operationTextEdit->toPlainText();

  if (txt.isEmpty())
    return;

  if (txt.at(txt.size() - 1) == ' ')
    txt.replace(txt.size() - 1, txt.size(), "");

  m_ui->m_operationTextEdit->setText(txt + " / ");
}

void te::qt::widgets::ArithmeticOpDialogForm::onMultPushButtonClicked()
{
  QString txt = m_ui->m_operationTextEdit->toPlainText();

  if (txt.isEmpty())
    return;

  if (txt.at(txt.size() - 1) == ' ')
    txt.replace(txt.size() - 1, txt.size(), "");

  m_ui->m_operationTextEdit->setText(txt + " * ");
}

void te::qt::widgets::ArithmeticOpDialogForm::onSubPushButtonClicked()
{
  QString txt = m_ui->m_operationTextEdit->toPlainText();

  if (txt.isEmpty())
    return;

  if (txt.at(txt.size() - 1) == ' ')
    txt.replace(txt.size() - 1, txt.size(), "");

  m_ui->m_operationTextEdit->setText(txt + " - ");
}

void te::qt::widgets::ArithmeticOpDialogForm::onAddPushButtonClicked()
{
  QString txt = m_ui->m_operationTextEdit->toPlainText();

  if (txt.isEmpty())
    return;

  if (txt.at(txt.size() - 1) == ' ')
    txt.replace(txt.size() - 1, txt.size(), "");

  m_ui->m_operationTextEdit->setText(txt + " + ");
}

void te::qt::widgets::ArithmeticOpDialogForm::onCePushButtonClicked()
{
  if (m_ui->m_operationTextEdit->toPlainText().isEmpty())
    return;

  std::string operation = m_ui->m_operationTextEdit->toPlainText().toLatin1();

  std::string txt = "";
  for (int i = operation.size() - 1; i >= 0; i--)
  {
    if (operation.at(i) == ' ' && i != operation.size() - 1)
    {
      operation.replace(i + 1, operation.size() - 1, "");
      break;
    }
    if (i == 0)
      operation.replace(i, operation.size(), "");
  }

  m_ui->m_operationTextEdit->clear();
  m_ui->m_operationTextEdit->setText(operation.c_str());
}

void te::qt::widgets::ArithmeticOpDialogForm::onCPushButtonClicked()
{
  m_ui->m_operationTextEdit->setText("");
}

void te::qt::widgets::ArithmeticOpDialogForm::onLoadPushButtonClicked()
{
  te::qt::widgets::LoadArithmeticOpDialog dlg(this);
  dlg.loadOperations();
  dlg.setList(m_layerList);
  if (dlg.exec())
  {
    m_ui->m_operationTextEdit->setText(dlg.getExpression().c_str());
    m_mapLayer = dlg.getLayers();
    m_operation = dlg.getOperation();
  }
}

void te::qt::widgets::ArithmeticOpDialogForm::onSavePushButtonClicked()
{
  if (m_ui->m_operationTextEdit->toPlainText().isEmpty())
    return;

  std::string arithOp = m_ui->m_operationTextEdit->toPlainText().toLatin1();
  std::string arithExpStr = "";

  int nLayer = 0;
  std::vector<std::string> arithExpVec;
  boost::split(arithExpVec, arithOp, boost::is_any_of(" "));

  for (int i = 0; i < arithExpVec.size(); i++)
  {
    int pos = arithExpVec[i].find(":");
    if (pos != -1)
    {
      std::vector<std::string> strVec;
      boost::split(strVec, arithExpVec[i], boost::is_any_of(":"));
      std::string band = strVec[1];
      if (strVec[1].at(0) == 'B')
        band = strVec[1].erase(0, 1);

      arithExpStr += "R" + te::common::Convert2String(nLayer) + ":B ";
      nLayer++;
    }
    else
      arithExpStr += arithExpVec[i] + " ";
  }

  std::string jsonfile = te::core::FindInTerraLibPath("share/terralib/json/arithmeticOperations.json");

  if (jsonfile.empty())
    throw te::common::Exception(TE_TR("Could not find arithmeticOperations.json file!"));

  boost::property_tree::ptree pt;
  boost::property_tree::ptree children;
  boost::property_tree::ptree operations;

  boost::property_tree::json_parser::read_json(jsonfile, pt);

  BOOST_FOREACH(boost::property_tree::ptree::value_type &v, pt.get_child("operations"))
  {
    std::string name = v.second.get<std::string>("name");
    std::string op = v.second.get<std::string>("operation");

    boost::property_tree::ptree child;
    child.put("name", name);
    child.put("operation", op);
    children.push_back(std::make_pair("", child));
  }

  boost::property_tree::ptree child;
  child.put("name", "");
  child.put("operation", arithExpStr);
  children.push_back(std::make_pair("", child));

  operations.add_child("operations", children);

  boost::property_tree::write_json(jsonfile, operations);

  QMessageBox::information(this, tr("Arithmetic Operations"), tr("The operation was saved."));
}

void te::qt::widgets::ArithmeticOpDialogForm::onOkPushButtonClicked()
{
  //check output parameters
  if (m_outputWidget->getOutputRaster().empty())
  {
    QMessageBox::information(this, "Arithmetic Operations", "Output image is not defined.");
    return;
  }

  if (!execute())
  {
    QMessageBox::information(this, "Arithmetic Operations", "Error to execute the operation.");
    return;
  }
}

void te::qt::widgets::ArithmeticOpDialogForm::getRasterBands(QComboBox* layer, int index, QComboBox* band)
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

void te::qt::widgets::ArithmeticOpDialogForm::updateExpressionHistory()
{
  m_expressionHistoryCounter = (int)m_expressionHistory.size() - 1;
}

bool te::qt::widgets::ArithmeticOpDialogForm::execute()
{
  //progress
  te::qt::widgets::ProgressViewerDialog v(this);
  int id = te::common::ProgressManager::getInstance().addViewer(&v);

  QApplication::setOverrideCursor(Qt::WaitCursor);

  te::rp::ArithmeticOperations::InputParameters algoInputParams;

  algoInputParams = paramsFromOpUserdDef();

  te::rp::ArithmeticOperations::OutputParameters algoOutputParams;

  algoOutputParams.m_rType = m_outputWidget->getType();

  std::map<std::string, std::string> rinfo;

  rinfo["URI"] = m_outputWidget->getOutputRaster();

  algoOutputParams.m_rInfo = rinfo;

  te::rp::ArithmeticOperations algorithmInstance;

  try
  {
    if (algorithmInstance.initialize(algoInputParams))
    {
      if (algorithmInstance.execute(algoOutputParams))
      {
        algoOutputParams.reset();

        //set output layer
        m_outputLayer = te::qt::widgets::createLayer("GDAL", rinfo);

        QMessageBox::information(this, tr("Arithmetic Operation"), tr("Arithmetic Operation ended successfully."));
      }
      else
      {
        QMessageBox::critical(this, tr("Arithmetic Operation"), tr("Arithmetic Operation execution error.") + (" " + te::rp::Module::getLastLogStr()).c_str());

        te::common::ProgressManager::getInstance().removeViewer(id);

        QApplication::restoreOverrideCursor();

        return false;
      }
    }
    else
    {
      QMessageBox::critical(this, tr("Arithmetic Operation"), tr("Arithmetic Operation initialization error") +
        (" " + te::rp::Module::getLastLogStr()).c_str());

      te::common::ProgressManager::getInstance().removeViewer(id);

      QApplication::restoreOverrideCursor();

      return false;
    }
  }
  catch (const std::exception& e)
  {
    QMessageBox::warning(this, tr("Arithmetic Operation"), e.what());

    te::common::ProgressManager::getInstance().removeViewer(id);

    QApplication::restoreOverrideCursor();

    return false;
  }
  catch (...)
  {
    QMessageBox::warning(this, tr("Arithmetic Operation"), tr("An exception has occurred!"));

    te::common::ProgressManager::getInstance().removeViewer(id);

    QApplication::restoreOverrideCursor();

    return false;
  }

  te::common::ProgressManager::getInstance().removeViewer(id);

  QApplication::restoreOverrideCursor();

  emit addLayer(m_outputLayer);

  return true;
}

te::rp::ArithmeticOperations::InputParameters te::qt::widgets::ArithmeticOpDialogForm::paramsFromOpUserdDef()
{
  std::string arithExpStr = "";
  te::rp::ArithmeticOperations::InputParameters params;

  std::map<std::string, te::map::AbstractLayer*> mapLayer = getMapLayer();

  std::map<std::string, te::map::AbstractLayer*>::iterator it = mapLayer.begin();
  while (it != mapLayer.end())
  {
    //get raster
    std::auto_ptr<te::da::DataSet> dsA = it->second->getData();
    std::size_t rposA = te::da::GetFirstPropertyPos(dsA.get(), te::dt::RASTER_TYPE);
    std::auto_ptr<te::rst::Raster> rstA = dsA->getRaster(rposA);

    params.m_inputRasters.push_back(rstA.release());

    it++;
  }

  arithExpStr = getExpression();

  if (arithExpStr.empty())
  {
    int nLayer = 0;
    std::vector<std::string> arithExpVec;
    boost::split(arithExpVec, getUserDefinedExpression(), boost::is_any_of(" "));

    for (int i = 0; i < arithExpVec.size(); i++)
    {
      int pos = arithExpVec[i].find(":");
      if (pos != -1)
      {
        std::vector<std::string> strVec;
        boost::split(strVec, arithExpVec[i], boost::is_any_of(":"));
        std::string band = strVec[1];
        if (strVec[1].at(0) == 'B')
          band = strVec[1].erase(0, 1);

        arithExpStr += "R" + te::common::Convert2String(nLayer) + ":" + band + " ";
        nLayer++;
      }
      else
        arithExpStr += arithExpVec[i] + " ";
    }
  }

  bool normalize = m_ui->m_normalizeCheckBox->isChecked();

  params.m_arithmeticString = arithExpStr;
  params.m_normalize = normalize;

  return params;
}
