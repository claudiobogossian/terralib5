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
#include "../../../srs/SpatialReferenceSystemManager.h"
#include "../../../common/StringUtils.h"
#include "../srs/SRSManagerDialog.h"
#include "../utils/ParameterTableWidget.h"
#include "../Utils.h"
#include "../Exception.h"
#include "RasterInfoWidget.h"
#include "ui_RasterInfoWidgetForm.h"
#include "ui_ParameterTableWidgetForm.h"

// QT
#include <QFileDialog>
#include <QString>
#include <QMessageBox>

// BOOST Includes
#include <boost/tokenizer.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/filesystem.hpp>

// STL
#include <fstream>


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
  
  connect(m_ui->m_forceRAWRastercheckBox, SIGNAL(stateChanged(int)), this, SLOT(rawRasterCheckBoxStateChanged(int)));
  connect(m_ui->m_rowsLineEdit, SIGNAL(textChanged(const QString &)), this, SLOT(rawRasterInfoChanged(const QString &)));
  connect(m_ui->m_columnsLineEdit, SIGNAL(textChanged(const QString &)), this, SLOT(rawRasterInfoChanged(const QString &)));
  connect(m_ui->m_bandsLineEdit, SIGNAL(textChanged(const QString &)), this, SLOT(rawRasterInfoChanged(const QString &)));
  connect(m_ui->m_dataTypeComboBox, SIGNAL(currentTextChanged(const QString &)), this, SLOT(rawRasterInfoChanged(const QString &)));
  
  connect(m_ui->m_resolutionXLineEdit, SIGNAL(textChanged(const QString &)), this, SLOT(rawRasterInfoChanged(const QString &)));
  connect(m_ui->m_resolutionYLineEdit, SIGNAL(textChanged(const QString &)), this, SLOT(rawRasterInfoChanged(const QString &)));
  connect(m_ui->m_sridLineEdit, SIGNAL(textChanged(const QString &)), this, SLOT(rawRasterInfoChanged(const QString &)));
  
  connect(m_ui->m_upperLeftXLineEdit, SIGNAL(textChanged(const QString &)), this, SLOT(rawRasterInfoChanged(const QString &)));
  connect(m_ui->m_upperLeftYLineEdit, SIGNAL(textChanged(const QString &)), this, SLOT(rawRasterInfoChanged(const QString &)));
  connect(m_ui->m_byteOrderComboBox, SIGNAL(currentTextChanged(const QString &)), this, SLOT(rawRasterInfoChanged(const QString &)));
  
  connect(m_ui->m_interleaveComboBox, SIGNAL(currentTextChanged(const QString &)), this, SLOT(rawRasterInfoChanged(const QString &)));
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

  std::size_t pos = name.find_last_of(".");
  if(pos != std::string::npos)
  {
    name = name.substr(0, pos);
  }

  return name;
}

std::string te::qt::widgets::RasterInfoWidget::getFullName() const
{
  return m_ui->m_fileNameLineEdit->text().toStdString();
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
  m_originalFullFileName.clear();
  
  // Acquiring the initial full file name
  
  if( m_outputMode )
  {
    m_originalFullFileName = QFileDialog::getSaveFileName(this, tr("Select the output file name"), 
      te::qt::widgets::GetFilePathFromSettings("rp_raster_info"), 
      te::qt::widgets::GetDiskRasterFileSelFilter(), 0 , 0).toStdString();    
  }
  else
  {
    m_originalFullFileName = QFileDialog::getOpenFileName(this, tr("Select File"), 
      te::qt::widgets::GetFilePathFromSettings("rp_raster_info"), 
      te::qt::widgets::GetDiskRasterFileSelFilter(), 0 ,QFileDialog::ReadOnly).toStdString();
  }
  
  // Configure the interface
  
  if( ! m_originalFullFileName.empty() )
  {    
    boost::filesystem::path fullFilePath( m_originalFullFileName );
    
    m_ui->m_fileNameLineEdit->setText(m_originalFullFileName.c_str());
    m_ui->m_nameLineEdit->setText(fullFilePath.stem().string().c_str());

    te::qt::widgets::AddFilePathToSettings(fullFilePath.parent_path().string().c_str(), 
      "rp_raster_info");    
  }  
  
  updateRawRasterFileName();
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

void te::qt::widgets::RasterInfoWidget::rawRasterInfoChanged(const QString & )
{
  updateRawRasterFileName();
}

void te::qt::widgets::RasterInfoWidget::rawRasterCheckBoxStateChanged( int state )
{
  if( state == Qt::Unchecked )
  {
    m_ui->m_fileNameLineEdit->setText( m_originalFullFileName.c_str() );
  }
  else
  {
    updateRawRasterFileName();
  }
}

void te::qt::widgets::RasterInfoWidget::updateRawRasterFileName()
{
  // Is this a RAW image file ?
  
  if( 
      ( ! m_originalFullFileName.empty() )
      &&
      m_ui->m_forceRAWRastercheckBox->isChecked()
    )
  {
    boost::filesystem::path fullFilePath( m_originalFullFileName );
    
    std::string upCaseExtension = te::common::Convert2UCase( fullFilePath.extension().string() );
    
    if( upCaseExtension != ".VRT" )
    {
      // Create a new vrt file and point the file name to it.
      
      std::string vrtFullFileName = fullFilePath.string();
      std::size_t pos = vrtFullFileName.find_last_of(".");
      if(pos != std::string::npos)
      {
        vrtFullFileName = vrtFullFileName.substr(0, pos);
      }
      vrtFullFileName = vrtFullFileName + ".vrt";
            
      boost::filesystem::path fullVrtPath( vrtFullFileName );
      
      boost::filesystem::remove( vrtFullFileName );
      
      if( !boost::filesystem::is_regular_file( fullVrtPath ) )
      {
        const unsigned int bandsNumber = m_ui->m_bandsLineEdit->text().toUInt();
        const unsigned int colsNumber = m_ui->m_columnsLineEdit->text().toUInt();
        const unsigned int rowsNumber = m_ui->m_rowsLineEdit->text().toUInt();
        const double resX = m_ui->m_resolutionXLineEdit->text().toDouble();
        const double resY = m_ui->m_resolutionYLineEdit->text().toDouble();
        const double upperLeftX = m_ui->m_upperLeftXLineEdit->text().toDouble();
        const double upperLeftY = m_ui->m_upperLeftYLineEdit->text().toDouble();
        const int SRID = m_ui->m_sridLineEdit->text().toInt();
        const std::string byteOrder = m_ui->m_byteOrderComboBox->currentText().toStdString();
        const std::string dataTypeString = m_ui->m_dataTypeComboBox->currentText().toStdString();;
        const std::string interleaveString = m_ui->m_interleaveComboBox->currentText().toStdString();;
        
        unsigned int dataTypeSizeBytes = 0;
        if( dataTypeString == "Byte" )
        {
          dataTypeSizeBytes = 1;
        }
        else if( dataTypeString == "UInt16" )
        {
          dataTypeSizeBytes = 2;
        }   
        else if( dataTypeString == "Int16" )
        {
          dataTypeSizeBytes = 2;
        } 
        else if( dataTypeString == "UInt32" )
        {
          dataTypeSizeBytes = 4;
        } 
        else if( dataTypeString == "Int32" )
        {
          dataTypeSizeBytes = 4;
        } 
        else if( dataTypeString == "Float32" )
        {
          dataTypeSizeBytes = 4; 
        } 
        else if( dataTypeString == "Float64" )
        {
          dataTypeSizeBytes = 8;
        } 
        else if( dataTypeString == "CInt16" )
        {
          dataTypeSizeBytes = 4;
        } 
        else if( dataTypeString == "CInt32" )
        {
          dataTypeSizeBytes = 8;
        } 
        else if( dataTypeString == "CFloat32" )
        {
          dataTypeSizeBytes = 8;
        }                                                                      
        else if( dataTypeString == "CFloat64" )
        {
          dataTypeSizeBytes = 16;
        }                                           
        else
        {
          throw te::qt::widgets::Exception( "Invalid data type" );
        }

        std::ofstream vrtfile;
        vrtfile.open ( vrtFullFileName.c_str() );
        
        if( !vrtfile.is_open() )
        {
          QMessageBox::critical( this, "Error", "Unable to create a VRT file" );
          return;
        }
        else
        {
          vrtfile.precision( 20 );
          
          vrtfile 
            << "<VRTDataset rasterXSize=\"" << colsNumber << "\""
            << " rasterYSize=\"" << rowsNumber << "\""
            << ">";
            
          if( SRID > 0 )
          {
            const std::string wktStr = te::srs::SpatialReferenceSystemManager::getInstance().getWkt( SRID );
            
            if( ! wktStr.empty() )
            {
              vrtfile << std::endl << "<SRS>" << wktStr << "</SRS>";
            }
          }
          
          if( ( resX != 0.0 ) && ( resY != 0.0 ) )
          {
            vrtfile << std::endl << "<GeoTransform>" << upperLeftX << "," 
              << resX << ",0," << upperLeftY << ",0," << (-1.0 * resY) 
              << "</GeoTransform>";
          }
            
          unsigned int pixelOffset = 0;
          unsigned int lineOffset = 0;
          unsigned int imageOffset = 0;
            
          for( unsigned int bandIdx = 0 ; bandIdx < bandsNumber ; ++bandIdx )
          {
            
            if( interleaveString == "BIP" ) // Band interleaved by pixel
            {
              pixelOffset = dataTypeSizeBytes * bandsNumber;
              lineOffset = pixelOffset * colsNumber;
              imageOffset = bandIdx * dataTypeSizeBytes;
            }
            else
            {
              // BSQ (Band sequential) interleave
              
              pixelOffset = dataTypeSizeBytes;
              lineOffset = pixelOffset * colsNumber;
              imageOffset = bandIdx * lineOffset * rowsNumber;
            }            
            
            vrtfile
              << std::endl
              << "<VRTRasterBand dataType=\"" << dataTypeString << "\" band=\"" << ( bandIdx + 1 )<< "\" subClass=\"VRTRawRasterBand\">" << std::endl
                << "<SourceFilename relativetoVRT=\"1\">" << fullFilePath.filename().string() << "</SourceFilename>" << std::endl
                << "<ImageOffset>" << imageOffset << "</ImageOffset>" << std::endl
                << "<PixelOffset>" << pixelOffset << "</PixelOffset>" << std::endl
                << "<LineOffset>" << lineOffset << "</LineOffset>";
                
            if( !byteOrder.empty() ) vrtfile << std::endl << "<ByteOrder>" << byteOrder << "</ByteOrder>" << std::endl;
              
            vrtfile << std::endl << "</VRTRasterBand>";
          }
          
          vrtfile << std::endl << "</VRTDataset>" << std::endl;
          
          m_ui->m_fileNameLineEdit->setText( vrtFullFileName.c_str() );
        }
      }
      
      
    }
  }
}