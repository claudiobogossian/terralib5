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
  \file terralib/qt/widgets/rp/SegmenterDialog.h

  \brief A dialog used to execute image segmentation.
*/

#ifndef __TERRALIB_QT_WIDGETS_RP_INTERNAL_SEGMENTERDIALOG_H
#define __TERRALIB_QT_WIDGETS_RP_INTERNAL_SEGMENTERDIALOG_H

// TerraLib
#include "../Config.h"
#include "../../../raster/Raster.h"

// Qt
#include <QDialog>

// STL

#include <map>
#include <string>

// Boost
#include <boost/shared_ptr.hpp>
#include <terralib/qt/widgets/se/BasicFillDialog.h>

// Forward user interface declaration
namespace Ui
{
  class SegmenterForm;
}

namespace te
{
  namespace qt
  {
    namespace widgets
    {
      /*!
        \class SegmenterDialog

        \brief A dialog used to execute image segmentation.
      */
      class TEQTWIDGETSEXPORT SegmenterDialog : public QDialog
      {
        Q_OBJECT
        
        public:

          /*! \brief Constructs a basic dialog which is a child of parent, with widget flags set to f. 
          
              \param inputRasterPtr Input raster pointer.
              
              \param outpuRasterDSType Output raster data source type (as described in te::raster::RasterFactory ).
              
              \param outpuRasterInfo The necessary information to create the raster (as described in te::raster::RasterFactory). 
          */
          SegmenterDialog(te::rst::Raster const* inputRasterPtr, 
            const std::string& outpuRasterDSType, 
            const std::map< std::string, std::string >& outpuRasterInfo,
            QWidget* parent = 0, 
            Qt::WindowFlags f = 0);

          /*! \brief Destructor. */
          ~SegmenterDialog();

          /*! \brief Returns the output result raster.
          
              \param outputRasterPtr A pointer to the output result raster
              
              \return true if OK, false on errors.
          */
          bool getOutputRaster( boost::shared_ptr< te::rst::Raster >& outputRasterPtr );
          
        protected slots:

          void on_okPushButton_clicked();

        private:

          Ui::SegmenterForm* m_uiPtr; //! User interface.
          te::rst::Raster const* m_inputRasterPtr; //! Input raster pointer.
          std::string m_outpuRasterDSType; //! Output raster data source type (as described in te::raster::RasterFactory ).
          std::map< std::string, std::string > m_outpuRasterInfo; //! The necessary information to create the raster (as described in te::raster::RasterFactory). 
          boost::shared_ptr< te::rst::Raster > m_outputRasterPtr; //! Output raster pointer.
      }; 

    } // end namespace widgets
  }   // end namespace qt
}     // end namespace te

#endif  // __TERRALIB_QT_WIDGETS_RP_INTERNAL_SEGMENTERDIALOG_H
