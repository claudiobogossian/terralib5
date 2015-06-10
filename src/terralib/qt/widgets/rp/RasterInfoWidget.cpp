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
  \file terralib/qt/widgets/rp/RasterInfoWidget.cpp

  \brief This file has the RasterInfoWidget class.
*/

// TerraLib
#include "../../../dataaccess/datasource/DataSource.h"
#include "../../../dataaccess/datasource/DataSourceCapabilities.h"
#include "../../../dataaccess/datasource/DataSourceFactory.h"
#include "../utils/ParameterTableWidget.h"
#include "../Utils.h"
#include "RasterInfoWidget.h"
#include "ui_RasterInfoWidgetForm.h"
#include "ui_ParameterTableWidgetForm.h"

// QT
#include <QFileDialog>

// BOOST Includes
#include <boost/tokenizer.hpp>
#include <boost/lexical_cast.hpp>


te::qt::widgets::RasterInfoWidget::RasterInfoWidget(QWidget* parent, Qt::WindowFlags f)
  : QWidget(parent, f),
    m_ui(new Ui::RasterInfoWidgetForm),
    m_dir("")
{
  m_ui->setupUi(this);

  QGridLayout* layout = new QGridLayout(m_ui->m_widget);
  m_table.reset( new te::qt::widgets::ParameterTableWidget(m_ui->m_widget));
  layout->addWidget(m_table.get());
  layout->setContentsMargins(0,0,0,0);

  m_ui->m_openFileDlgToolButton->setIcon(QIcon::fromTheme("folder"));
  m_ui->m_openDSDlgToolButton->setIcon(QIcon::fromTheme("datasource"));

  m_table->getForm()->m_parameterTitle->setText(tr("Extra parameters"));

  //connects
  connect(m_ui->m_openFileDlgToolButton, SIGNAL(clicked()), this, SLOT(onOpenFileDlgToolButtonClicked()));

  fillExtensions();
}

te::qt::widgets::RasterInfoWidget::~RasterInfoWidget()
{
}

Ui::RasterInfoWidgetForm* te::qt::widgets::RasterInfoWidget::getForm() const
{
  return m_ui.get();
}

std::string te::qt::widgets::RasterInfoWidget::getType() const
{
  if(m_ui->m_fileRadioButton->isChecked())
  {
    return "GDAL";
  }
  else if(m_ui->m_memRadioButton->isChecked())
  {
    return "MEM";
  }
  else if(m_ui->m_dsRadioButton->isChecked())
  {
    return "";
  }

  return "";
}

std::map<std::string, std::string> te::qt::widgets::RasterInfoWidget::getInfo() const
{
  std::map<std::string, std::string> rinfo;

  std::string name = getBaseName();

  if(m_ui->m_fileRadioButton->isChecked())
  {
    std::string fileName = m_dir + "/" + name + getExtension();

    rinfo["URI"] = fileName;
  }
  else if(m_ui->m_memRadioButton->isChecked())
  {

  }
  else if(m_ui->m_dsRadioButton->isChecked())
  {

  }

  if(m_ui->m_overightRadioButton->isChecked() == false)
  {
    //get extra parameters
    std::map<std::string, std::string> extra = m_table->getMap();

    rinfo.insert(extra.begin(), extra.end());
  }

  return rinfo;
}

std::map<std::string, std::string> te::qt::widgets::RasterInfoWidget::getInfo(int count) const
{
  std::map<std::string, std::string> rinfo;

  std::string name = getBaseName();

  if(m_ui->m_fileRadioButton->isChecked())
  {
    std::string str = boost::lexical_cast< std::string >( count );
    std::string fileName = m_dir + "/" + name + "_" + str + getExtension();

    rinfo["URI"] = fileName;
  }
  else if(m_ui->m_memRadioButton->isChecked())
  {

  }
  else if(m_ui->m_dsRadioButton->isChecked())
  {

  }

  if(m_ui->m_overightRadioButton->isChecked() == false)
  {
    //get extra parameters
    std::map<std::string, std::string> extra = m_table->getMap();

    rinfo.insert(extra.begin(), extra.end());
  }

  return rinfo;
}

std::auto_ptr<te::da::DataSource> te::qt::widgets::RasterInfoWidget::getDataSource() const
{
  std::map<std::string, std::string> connInfoRaster;
  connInfoRaster["SOURCE"] = m_dir + "/";

  std::auto_ptr< te::da::DataSource > dsPtr( te::da::DataSourceFactory::make("GDAL") );
  dsPtr->setConnectionInfo( connInfoRaster );
  dsPtr->open();

  return dsPtr;
}

std::string te::qt::widgets::RasterInfoWidget::getName() const
{
  std::string name = getBaseName();

  if(m_ui->m_fileRadioButton->isChecked())
  {
     name += getExtension();
  }

  return name;
}

std::string te::qt::widgets::RasterInfoWidget::getShortName() const
{
  std::string name = "";

  if(m_ui->m_nameLineEdit->text().isEmpty() == false)
    name = m_ui->m_nameLineEdit->text().toStdString();

  std::size_t pos = name.find(".");
  if(pos != std::string::npos)
  {
    name = name.substr(0, pos);
  }

  return name;
}

std::string te::qt::widgets::RasterInfoWidget::getExtension() const
{
  std::string ext = "." + m_ui->m_extComboBox->currentText().toStdString();

  return ext;
}

std::string te::qt::widgets::RasterInfoWidget::getPath() const
{
  return m_dir + "/";
}

bool te::qt::widgets::RasterInfoWidget::overight() const
{
  return m_ui->m_overightRadioButton->isChecked();
}

bool te::qt::widgets::RasterInfoWidget::fileExists() const
{
  if(m_ui->m_fileRadioButton->isChecked())
  {
    std::string name = getBaseName();

    std::string fileName = m_dir + "/" + name + getExtension();

    QFile file(fileName.c_str());

    return file.exists();
  }

  return false;
}

std::string te::qt::widgets::RasterInfoWidget::getBaseName() const
{
  std::string name = "";

  if(m_ui->m_nameLineEdit->text().isEmpty() == false)
  {
    name = m_ui->m_nameLineEdit->text().toStdString();

    QFileInfo file(name.c_str());

    name = file.baseName().toStdString();
  }

  return name;
}

void te::qt::widgets::RasterInfoWidget::fillExtensions()
{
  //fill extensions
  m_ui->m_extComboBox->clear();

  std::auto_ptr<te::da::DataSource> ds = te::da::DataSourceFactory::make("GDAL");

  if(!ds.get())
    return;

  te::da::DataSourceCapabilities dsCap = ds->getCapabilities();

  std::map<std::string, std::string> mapSpecCap = dsCap.getSpecificCapabilities();

  std::string exts = mapSpecCap["SUPPORTED_EXTENSIONS"];

//create boost tokenizer
  typedef boost::tokenizer< boost::escaped_list_separator<char> > Tokenizer;
  boost::escaped_list_separator<char> sep('\\', ';', '\"');
  
  std::vector<std::string> extVec;

  Tokenizer tok(exts, sep);
  extVec.assign(tok.begin(), tok.end());

  int curIdx = 0;

  for(std::size_t t = 0; t < extVec.size(); ++t)
  {
    m_ui->m_extComboBox->addItem(extVec[t].c_str());

    if(extVec[t] == "tif")
      curIdx = (int)t;
  }

  m_ui->m_extComboBox->setCurrentIndex(curIdx);
}

void te::qt::widgets::RasterInfoWidget::onOpenFileDlgToolButtonClicked()
{
  QString dir = QFileDialog::getExistingDirectory(this, tr("Open Directory"), te::qt::widgets::GetFilePathFromSettings("rp_raster_info"), QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);

  if(dir.isEmpty() == false)
  {
    m_dir = dir.replace(QRegExp("\\\\"), "/").toStdString();

    m_ui->m_fileNameLineEdit->setText(m_dir.c_str());

    te::qt::widgets::AddFilePathToSettings(m_dir.c_str(), "rp_raster_info");
  }
}
