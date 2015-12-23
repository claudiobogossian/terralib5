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
#include "../srs/SRSManagerDialog.h"
#include "../utils/ParameterTableWidget.h"
#include "../Utils.h"
#include "RasterInfoWidget.h"
#include "ui_RasterInfoWidgetForm.h"
#include "ui_ParameterTableWidgetForm.h"

// QT
#include <QFileDialog>
#include <QString>

// BOOST Includes
#include <boost/tokenizer.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/filesystem.hpp>


te::qt::widgets::RasterInfoWidget::RasterInfoWidget(
    const bool outputMode,
    QWidget* parent, Qt::WindowFlags f)
  : QWidget(parent, f),
    m_outputMode( outputMode ),
    m_ui(new Ui::RasterInfoWidgetForm)
{
  m_ui->setupUi(this);

  QGridLayout* layout = new QGridLayout(m_ui->m_widget);
  m_table.reset( new te::qt::widgets::ParameterTableWidget(m_ui->m_widget));
  layout->addWidget(m_table.get());
  layout->setContentsMargins(0,0,0,0);

  m_ui->m_openFileDlgToolButton->setIcon(QIcon::fromTheme("file-raster"));
  m_ui->m_openSRIDDlgToolButton->setIcon(QIcon::fromTheme("srs"));

  m_table->getForm()->m_parameterTitle->setText(tr("Extra parameters"));
  
  if( outputMode )
  {
    m_ui->m_forceRAWRastercheckBox->setEnabled( false );    
  }
  else
  {
    m_ui->m_forceRAWRastercheckBox->setEnabled( true );    
  }  

  //connects
  connect(m_ui->m_openFileDlgToolButton, SIGNAL(clicked()), this, SLOT(onOpenFileDlgToolButtonClicked()));
  connect(m_ui->m_openSRIDDlgToolButton, SIGNAL(clicked()), this, SLOT(onOpenSRIDDlgToolButtonClicked()));
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
  return "GDAL";
}

std::map<std::string, std::string> te::qt::widgets::RasterInfoWidget::getInfo() const
{
  std::map<std::string, std::string> rinfo;

  rinfo["URI"] = m_ui->m_fileNameLineEdit->text().toStdString();

  //get extra parameters
  
  std::map<std::string, std::string> extra = m_table->getMap();

  rinfo.insert(extra.begin(), extra.end());

  return rinfo;
}

std::map<std::string, std::string> te::qt::widgets::RasterInfoWidget::getInfo(int count) const
{
  std::map<std::string, std::string> rinfo;

  boost::filesystem::path path( m_ui->m_fileNameLineEdit->text().toStdString() );
  
  //std::string name = getBaseName();

  std::string fileName = 
    path.parent_path().string() 
    + std::string( "/" )
    + path.stem().string()
    + std::string( "_" )
    + boost::lexical_cast< std::string >( count ) 
    + path.extension().string();

  rinfo["URI"] = fileName;

  //get extra parameters
  
  std::map<std::string, std::string> extra = m_table->getMap();

  rinfo.insert(extra.begin(), extra.end());

  return rinfo;
}

std::auto_ptr<te::da::DataSource> te::qt::widgets::RasterInfoWidget::getDataSource() const
{
  boost::filesystem::path path( m_ui->m_fileNameLineEdit->text().toStdString() );
  std::map<std::string, std::string> connInfoRaster;
  connInfoRaster["SOURCE"] = path.parent_path().string();

  std::auto_ptr< te::da::DataSource > dsPtr( te::da::DataSourceFactory::make("GDAL") );
  dsPtr->setConnectionInfo( connInfoRaster );
  dsPtr->open();

  return dsPtr;
}

std::string te::qt::widgets::RasterInfoWidget::getName() const
{
  boost::filesystem::path path( m_ui->m_fileNameLineEdit->text().toStdString() );
  return path.filename().string();
}

std::string te::qt::widgets::RasterInfoWidget::getShortName() const
{
  boost::filesystem::path path( m_ui->m_fileNameLineEdit->text().toStdString() );
  std::string name = path.filename().string();

  std::size_t pos = name.find(".");
  if(pos != std::string::npos)
  {
    name = name.substr(0, pos);
  }

  return name;
}

std::string te::qt::widgets::RasterInfoWidget::getFullName() const
{
  boost::filesystem::path path( m_ui->m_fileNameLineEdit->text().toStdString() );
  return path.string();
}

std::string te::qt::widgets::RasterInfoWidget::getExtension() const
{
  boost::filesystem::path path( m_ui->m_fileNameLineEdit->text().toStdString() );
  return path.extension().string();
}

std::string te::qt::widgets::RasterInfoWidget::getPath() const
{
  boost::filesystem::path path( m_ui->m_fileNameLineEdit->text().toStdString() );
  return path.parent_path().string();
}

bool te::qt::widgets::RasterInfoWidget::fileExists() const
{
  boost::filesystem::path path( m_ui->m_fileNameLineEdit->text().toStdString() );
  return boost::filesystem::exists( path );
}

void te::qt::widgets::RasterInfoWidget::onOpenFileDlgToolButtonClicked()
{
  m_ui->m_nameLineEdit->clear();
  m_ui->m_fileNameLineEdit->clear();
  
  QString fileName;
  
  if( m_outputMode )
  {
    fileName = QFileDialog::getSaveFileName(this, tr("Select the output file name"), 
      te::qt::widgets::GetFilePathFromSettings("rp_raster_info"), 
      te::qt::widgets::GetDiskRasterFileSelFilter(), 0 , 0);    
  }
  else
  {
    fileName = QFileDialog::getOpenFileName(this, tr("Select File"), 
      te::qt::widgets::GetFilePathFromSettings("rp_raster_info"), 
      te::qt::widgets::GetDiskRasterFileSelFilter(), 0 ,QFileDialog::ReadOnly);
  }
  
  if(fileName.isEmpty() == false)
  {
    m_ui->m_fileNameLineEdit->setText(fileName);

    boost::filesystem::path path( m_ui->m_fileNameLineEdit->text().toStdString() );
    
    m_ui->m_nameLineEdit->setText(path.stem().string().c_str());
    
    te::qt::widgets::AddFilePathToSettings(path.parent_path().string().c_str(), 
      "rp_raster_info");
  }  
}

void te::qt::widgets::RasterInfoWidget::onOpenSRIDDlgToolButtonClicked()
{
  m_ui->m_sridLineEdit->clear();
  
  te::qt::widgets::SRSManagerDialog diag( this, 0 );
  diag.exec();
  
  if( 
      ( diag.getSelectedSRS().first != 0 )
      &&
      ( ! diag.getSelectedSRS().second.empty() )
    )
  {
    m_ui->m_sridLineEdit->setText( QString::number( diag.getSelectedSRS().first ) );
  }
}
