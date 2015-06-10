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
  \file terralib/qt/widgets/rp/ArithmeticOpWizard.cpp

  \brief A Qt dialog that allows users to run a arithmeticOp operation defined by RP module.
*/

// TerraLib 
#include "../../../common/progress/ProgressManager.h"
#include "../../../common/STLUtils.h"
#include "../../../dataaccess/dataset/DataSet.h"
#include "../../../dataaccess/utils/Utils.h"
#include "../../../raster/RasterFactory.h"
#include "../../../rp/Functions.h"
#include "../../widgets/help/HelpPushButton.h"
#include "../../widgets/progress/ProgressViewerDialog.h"
#include "../layer/search/LayerSearchWidget.h"
#include "../layer/search/LayerSearchWizardPage.h"
#include "ArithmeticOpWizard.h"
#include "ArithmeticOpWizardPage.h"
#include "RasterInfoWidget.h"
#include "RasterInfoWizardPage.h"
#include "Utils.h"

// STL
#include <cassert>

// Qt
#include <QMessageBox>
#include <QApplication>


te::qt::widgets::ArithmeticOpWizard::ArithmeticOpWizard(QWidget* parent)
  : QWizard(parent)
{
  //configure the wizard
  this->setWizardStyle(QWizard::ModernStyle);
  this->setWindowTitle(tr("Arithmetic Operation"));
  //this->setFixedSize(640, 480);

  this->setOption(QWizard::HaveHelpButton, true);
  this->setOption(QWizard::HelpButtonOnRight, false);

  te::qt::widgets::HelpPushButton* helpButton = new te::qt::widgets::HelpPushButton(this);

  this->setButton(QWizard::HelpButton, helpButton);

  helpButton->setPageReference("plugins/rp/rp_arithOp.html");

  addPages();
}

te::qt::widgets::ArithmeticOpWizard::~ArithmeticOpWizard()
{

}

bool te::qt::widgets::ArithmeticOpWizard::validateCurrentPage()
{
  if(currentPage() ==  m_layerSearchPage.get())
  {
    std::list<te::map::AbstractLayerPtr> list = m_layerSearchPage->getSearchWidget()->getSelecteds();

    m_arithmeticOpPage->setList(list);

    return m_layerSearchPage->isComplete();
  }
  else if(currentPage() ==  m_arithmeticOpPage.get())
  {
    return m_arithmeticOpPage->isComplete();
  }
  else if(currentPage() ==  m_rasterInfoPage.get())
  {
    return execute();
  }

  return true;
}

void te::qt::widgets::ArithmeticOpWizard::setList(std::list<te::map::AbstractLayerPtr>& layerList)
{
  m_layerSearchPage->getSearchWidget()->setList(layerList);
  m_layerSearchPage->getSearchWidget()->filterOnlyByRaster();
  m_layerSearchPage->getSearchWidget()->enableMultiSelection(true);
}

te::map::AbstractLayerPtr te::qt::widgets::ArithmeticOpWizard::getOutputLayer()
{
  return m_outputLayer;
}

void te::qt::widgets::ArithmeticOpWizard::addPages()
{
  m_layerSearchPage.reset(new te::qt::widgets::LayerSearchWizardPage(this));
  m_arithmeticOpPage.reset(new te::qt::widgets::ArithmeticOpWizardPage(this));
  m_rasterInfoPage.reset(new te::qt::widgets::RasterInfoWizardPage(this));

  addPage(m_layerSearchPage.get());
  addPage(m_arithmeticOpPage.get());
  addPage(m_rasterInfoPage.get());
}

bool te::qt::widgets::ArithmeticOpWizard::execute()
{
  //progress
  te::qt::widgets::ProgressViewerDialog v(this);
  int id = te::common::ProgressManager::getInstance().addViewer(&v);

  QApplication::setOverrideCursor(Qt::WaitCursor);

  int type = m_arithmeticOpPage->getOperationType();
  
  te::rp::ArithmeticOperations::InputParameters algoInputParams;

  if(type == ARITH_OP_TYPE_1)
    algoInputParams = paramsFromOp1();
  else if(type == ARITH_OP_TYPE_2)
    algoInputParams = paramsFromOp2();
  else if(type == ARITH_OP_TYPE_3)
    algoInputParams = paramsFromOp3();
  else if(type == ARITH_OP_TYPE_4)
    algoInputParams = paramsFromOp4();
  else if(type == ARITH_OP_TYPE_5)
    algoInputParams = paramsFromOp5();
  else if(type == ARITH_OP_TYPE_6)
    algoInputParams = paramsFromOp6();
  else if(type == ARITH_OP_TYPE_USER_DEFINED)
    algoInputParams = paramsFromOpUserdDef();

  te::rp::ArithmeticOperations::OutputParameters algoOutputParams;

  algoOutputParams.m_rType = m_rasterInfoPage->getWidget()->getType();
  algoOutputParams.m_rInfo = m_rasterInfoPage->getWidget()->getInfo();

  te::rp::ArithmeticOperations algorithmInstance;

  try
  {
    if(algorithmInstance.initialize(algoInputParams))
    {
      if(algorithmInstance.execute(algoOutputParams))
      {
        algoOutputParams.reset();

        //set output layer
        m_outputLayer = te::qt::widgets::createLayer(m_rasterInfoPage->getWidget()->getType(), 
                                                      m_rasterInfoPage->getWidget()->getInfo());
      
        QMessageBox::information(this, tr("Arithmetic Operation"), tr("Arithmetic Operation ended sucessfully"));
      }
      else
      {
        QMessageBox::critical(this, tr("Arithmetic Operation"), tr("Arithmetic Operation execution error.") +  ( " " + te::rp::Module::getLastLogStr() ).c_str());

        te::common::ProgressManager::getInstance().removeViewer(id);

        QApplication::restoreOverrideCursor();

        return false;
      }
    }
    else
    {
      QMessageBox::critical(this, tr("Arithmetic Operation"), tr("Arithmetic Operation initialization error") +
        ( " " + te::rp::Module::getLastLogStr() ).c_str());

      te::common::ProgressManager::getInstance().removeViewer(id);

      QApplication::restoreOverrideCursor();

      return false;
    }
  }
  catch(const std::exception& e)
  {
    QMessageBox::warning(this, tr("Arithmetic Operation"), e.what());

    te::common::ProgressManager::getInstance().removeViewer(id);

    QApplication::restoreOverrideCursor();

    return false;
  }
  catch(...)
  {
    QMessageBox::warning(this, tr("Arithmetic Operation"), tr("An exception has occurred!"));

    te::common::ProgressManager::getInstance().removeViewer(id);

    QApplication::restoreOverrideCursor();

    return false;
  }

  te::common::ProgressManager::getInstance().removeViewer(id);

  QApplication::restoreOverrideCursor();

  return true;
}

te::rp::ArithmeticOperations::InputParameters te::qt::widgets::ArithmeticOpWizard::paramsFromOp1()
{
  te::rp::ArithmeticOperations::InputParameters params;

  double gain = m_arithmeticOpPage->getGainValue();
  double offSet = m_arithmeticOpPage->getOffSetValue();
  bool normalize = m_arithmeticOpPage->normalize();

  te::map::AbstractLayerPtr lA = m_arithmeticOpPage->getLayerRasterA();
  int bA = m_arithmeticOpPage->getLayerBandA();

  //get raster
  std::auto_ptr<te::da::DataSet> dsA = lA->getData();
  std::size_t rposA = te::da::GetFirstPropertyPos(dsA.get(), te::dt::RASTER_TYPE);
  std::auto_ptr<te::rst::Raster> rstA = dsA->getRaster(rposA);

//Gain * ( A ) + Offset
  std::string arithmeticString  = QString::number(gain).toLatin1().data();
              arithmeticString += " * R0:";
              arithmeticString += QString::number(bA).toLatin1().data();
              arithmeticString += " + ";
              arithmeticString += QString::number(offSet).toLatin1().data();

  params.m_arithmeticString = arithmeticString;
  params.m_normalize = normalize;

  params.m_inputRasters.push_back(rstA.release());

  return params;
}

te::rp::ArithmeticOperations::InputParameters te::qt::widgets::ArithmeticOpWizard::paramsFromOp2()
{
  te::rp::ArithmeticOperations::InputParameters params;

  double gain = m_arithmeticOpPage->getGainValue();
  double offSet = m_arithmeticOpPage->getOffSetValue();
  bool normalize = m_arithmeticOpPage->normalize();

  te::map::AbstractLayerPtr lA = m_arithmeticOpPage->getLayerRasterA();
  int bA = m_arithmeticOpPage->getLayerBandA();

  //get raster
  std::auto_ptr<te::da::DataSet> dsA = lA->getData();
  std::size_t rposA = te::da::GetFirstPropertyPos(dsA.get(), te::dt::RASTER_TYPE);
  std::auto_ptr<te::rst::Raster> rstA = dsA->getRaster(rposA);

  te::map::AbstractLayerPtr lB = m_arithmeticOpPage->getLayerRasterB();
  int bB = m_arithmeticOpPage->getLayerBandB();

  //get raster
  std::auto_ptr<te::da::DataSet> dsB = lB->getData();
  std::size_t rposB = te::da::GetFirstPropertyPos(dsB.get(), te::dt::RASTER_TYPE);
  std::auto_ptr<te::rst::Raster> rstB = dsB->getRaster(rposB);

//Gain * ( A + B ) + Offset
  std::string arithmeticString  = QString::number(gain).toLatin1().data();
              arithmeticString += " * ( R0:";
              arithmeticString += QString::number(bA).toLatin1().data();
              arithmeticString += " + R1:";
              arithmeticString += QString::number(bB).toLatin1().data();
              arithmeticString += " ) + ";
              arithmeticString += QString::number(offSet).toLatin1().data();

  params.m_arithmeticString = arithmeticString;
  params.m_normalize = normalize;

  params.m_inputRasters.push_back(rstA.release());

  params.m_inputRasters.push_back(rstB.release());

  return params;
}

te::rp::ArithmeticOperations::InputParameters te::qt::widgets::ArithmeticOpWizard::paramsFromOp3()
{
  te::rp::ArithmeticOperations::InputParameters params;

  double gain = m_arithmeticOpPage->getGainValue();
  double offSet = m_arithmeticOpPage->getOffSetValue();
  bool normalize = m_arithmeticOpPage->normalize();

  te::map::AbstractLayerPtr lA = m_arithmeticOpPage->getLayerRasterA();
  int bA = m_arithmeticOpPage->getLayerBandA();

  //get raster
  std::auto_ptr<te::da::DataSet> dsA = lA->getData();
  std::size_t rposA = te::da::GetFirstPropertyPos(dsA.get(), te::dt::RASTER_TYPE);
  std::auto_ptr<te::rst::Raster> rstA = dsA->getRaster(rposA);

  te::map::AbstractLayerPtr lB = m_arithmeticOpPage->getLayerRasterB();
  int bB = m_arithmeticOpPage->getLayerBandB();

  //get raster
  std::auto_ptr<te::da::DataSet> dsB = lB->getData();
  std::size_t rposB = te::da::GetFirstPropertyPos(dsB.get(), te::dt::RASTER_TYPE);
  std::auto_ptr<te::rst::Raster> rstB = dsB->getRaster(rposB);

//Gain * ( A - B ) + Offset
  std::string arithmeticString  = QString::number(gain).toLatin1().data();
              arithmeticString += " * ( R0:";
              arithmeticString += QString::number(bA).toLatin1().data();
              arithmeticString += " - R1:";
              arithmeticString += QString::number(bB).toLatin1().data();
              arithmeticString += " ) + ";
              arithmeticString += QString::number(offSet).toLatin1().data();

  params.m_arithmeticString = arithmeticString;
  params.m_normalize = normalize;

  params.m_inputRasters.push_back(rstA.release());

  params.m_inputRasters.push_back(rstB.release());

  return params;
}

te::rp::ArithmeticOperations::InputParameters te::qt::widgets::ArithmeticOpWizard::paramsFromOp4()
{
  te::rp::ArithmeticOperations::InputParameters params;

  double gain = m_arithmeticOpPage->getGainValue();
  double offSet = m_arithmeticOpPage->getOffSetValue();
  bool normalize = m_arithmeticOpPage->normalize();

  te::map::AbstractLayerPtr lA = m_arithmeticOpPage->getLayerRasterA();
  int bA = m_arithmeticOpPage->getLayerBandA();

  //get raster
  std::auto_ptr<te::da::DataSet> dsA = lA->getData();
  std::size_t rposA = te::da::GetFirstPropertyPos(dsA.get(), te::dt::RASTER_TYPE);
  std::auto_ptr<te::rst::Raster> rstA = dsA->getRaster(rposA);

  te::map::AbstractLayerPtr lB = m_arithmeticOpPage->getLayerRasterB();
  int bB = m_arithmeticOpPage->getLayerBandB();

  //get raster
  std::auto_ptr<te::da::DataSet> dsB = lB->getData();
  std::size_t rposB = te::da::GetFirstPropertyPos(dsB.get(), te::dt::RASTER_TYPE);
  std::auto_ptr<te::rst::Raster> rstB = dsB->getRaster(rposB);

//Gain * ( A * B ) + Offset
  std::string arithmeticString  = QString::number(gain).toLatin1().data();
              arithmeticString += " * ( R0:";
              arithmeticString += QString::number(bA).toLatin1().data();
              arithmeticString += " * R1:";
              arithmeticString += QString::number(bB).toLatin1().data();
              arithmeticString += " ) + ";
              arithmeticString += QString::number(offSet).toLatin1().data();

  params.m_arithmeticString = arithmeticString;
  params.m_normalize = normalize;

  params.m_inputRasters.push_back(rstA.release());

  params.m_inputRasters.push_back(rstB.release());

  return params;
}

te::rp::ArithmeticOperations::InputParameters te::qt::widgets::ArithmeticOpWizard::paramsFromOp5()
{
  te::rp::ArithmeticOperations::InputParameters params;

  double gain = m_arithmeticOpPage->getGainValue();
  double offSet = m_arithmeticOpPage->getOffSetValue();
  bool normalize = m_arithmeticOpPage->normalize();

  te::map::AbstractLayerPtr lA = m_arithmeticOpPage->getLayerRasterA();
  int bA = m_arithmeticOpPage->getLayerBandA();

  //get raster
  std::auto_ptr<te::da::DataSet> dsA = lA->getData();
  std::size_t rposA = te::da::GetFirstPropertyPos(dsA.get(), te::dt::RASTER_TYPE);
  std::auto_ptr<te::rst::Raster> rstA = dsA->getRaster(rposA);

  te::map::AbstractLayerPtr lB = m_arithmeticOpPage->getLayerRasterB();
  int bB = m_arithmeticOpPage->getLayerBandB();

  //get raster
  std::auto_ptr<te::da::DataSet> dsB = lB->getData();
  std::size_t rposB = te::da::GetFirstPropertyPos(dsB.get(), te::dt::RASTER_TYPE);
  std::auto_ptr<te::rst::Raster> rstB = dsB->getRaster(rposB);

//Gain * ( A / B ) + Offset
  std::string arithmeticString  = QString::number(gain).toLatin1().data();
              arithmeticString += " * ( R0:";
              arithmeticString += QString::number(bA).toLatin1().data();
              arithmeticString += " / R1:";
              arithmeticString += QString::number(bB).toLatin1().data();
              arithmeticString += " ) + ";
              arithmeticString += QString::number(offSet).toLatin1().data();
  
  params.m_arithmeticString = arithmeticString;
  params.m_normalize = normalize;

  params.m_inputRasters.push_back(rstA.release());

  params.m_inputRasters.push_back(rstB.release());

  return params;
}

te::rp::ArithmeticOperations::InputParameters te::qt::widgets::ArithmeticOpWizard::paramsFromOp6()
{
  te::rp::ArithmeticOperations::InputParameters params;

  double gain = m_arithmeticOpPage->getGainValue();
  double offSet = m_arithmeticOpPage->getOffSetValue();
  bool normalize = m_arithmeticOpPage->normalize();

  te::map::AbstractLayerPtr lA = m_arithmeticOpPage->getLayerRasterA();
  int bA = m_arithmeticOpPage->getLayerBandA();

  //get raster
  std::auto_ptr<te::da::DataSet> dsA = lA->getData();
  std::size_t rposA = te::da::GetFirstPropertyPos(dsA.get(), te::dt::RASTER_TYPE);
  std::auto_ptr<te::rst::Raster> rstA = dsA->getRaster(rposA);

  te::map::AbstractLayerPtr lB = m_arithmeticOpPage->getLayerRasterB();
  int bB = m_arithmeticOpPage->getLayerBandB();

  //get raster
  std::auto_ptr<te::da::DataSet> dsB = lB->getData();
  std::size_t rposB = te::da::GetFirstPropertyPos(dsB.get(), te::dt::RASTER_TYPE);
  std::auto_ptr<te::rst::Raster> rstB = dsB->getRaster(rposB);

//( Gain * ( A - B ) / ( A + B ) ) + Offset
  std::string arithmeticString  = "( ";
              arithmeticString += QString::number(gain).toLatin1().data();
              arithmeticString += " * ( R0:";
              arithmeticString += QString::number(bA).toLatin1().data();
              arithmeticString += " - R1:";
              arithmeticString += QString::number(bB).toLatin1().data();
              arithmeticString += " ) / ";
              arithmeticString += "( R0:";
              arithmeticString += QString::number(bA).toLatin1().data();
              arithmeticString += " + R1:";
              arithmeticString += QString::number(bB).toLatin1().data();
              arithmeticString += " ) ) + ";
              arithmeticString += QString::number(offSet).toLatin1().data();

  params.m_arithmeticString = arithmeticString;
  params.m_normalize = normalize;

  params.m_inputRasters.push_back(rstA.release());

  params.m_inputRasters.push_back(rstB.release());

  return params;
}

te::rp::ArithmeticOperations::InputParameters te::qt::widgets::ArithmeticOpWizard::paramsFromOpUserdDef()
{
  te::rp::ArithmeticOperations::InputParameters params;

  bool normalize = m_arithmeticOpPage->normalize();
  std::string arithExpStr = m_arithmeticOpPage->getUserDefinedExpression();

  params.m_arithmeticString = arithExpStr;
  params.m_normalize = normalize;

  //params.m_rasterVec.push_back();

  return params;
}
