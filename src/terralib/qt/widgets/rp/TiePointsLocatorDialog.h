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
  \file terralib/qt/widgets/rp/TiePointsLocatorDialog.h

  \brief A dialog used to execute image contrast enhencement.
*/

#ifndef __TERRALIB_QT_WIDGETS_RP_INTERNAL_TIEPOINTSLOCATOR_H
#define __TERRALIB_QT_WIDGETS_RP_INTERNAL_TIEPOINTSLOCATOR_H

// TerraLib
#include "../Config.h"
#include "../../../raster/Raster.h"

// Qt
#include <QtGui/QDialog>

// STL

#include <vector>

// Boost
//#include <boost/shared_ptr.hpp>
//#include <terralib/qt/widgets/se/BasicFillDialog.h>

// Forward user interface declaration
namespace Ui
{
  class TiePointsLocatorForm;
}

namespace te
{
  namespace qt
  {
    namespace widgets
    {
      /*!
        \class TiePointsLocatorDialog

        \brief A dialog used to execute tie points location.
      */
      class TEQTWIDGETSEXPORT TiePointsLocatorDialog : public QDialog
      {
        Q_OBJECT
        
        public:

          /*! \brief Constructs a basic dialog which is a child of parent, with widget flags set to f. 
          
              \param inRaster1Ptr Input raster 1 pointer.
              
              \param inRaster1Ptr Input raster 2 pointer.
              
              \param parent Parent widget pointer.
              
              \param f Widget flags.
          */
          TiePointsLocatorDialog(
            te::rst::Raster const* inRaster1Ptr,
            te::rst::Raster const* inRaster2Ptr,
            QWidget* parent = 0, 
            Qt::WindowFlags f = 0);

          /*! \brief Destructor. */
          ~TiePointsLocatorDialog();

        protected slots:

          void on_okPushButton_clicked();

        private:

          Ui::TiePointsLocatorForm* m_uiPtr; //! User interface.
          te::rst::Raster const* m_inRaster1Ptr; //! Input raster 1 pointer.
          te::rst::Raster const* m_inRaster2Ptr; //! Input raster 2 pointer.
      }; 

    }
  }
}

#endif
