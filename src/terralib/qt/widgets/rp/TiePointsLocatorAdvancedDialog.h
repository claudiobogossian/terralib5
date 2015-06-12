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
  \file terralib/qt/widgets/rp/TiePointsLocatorAdvancedDialog.h

  \brief A dialog used to execute tie points location advanced options.
*/

#ifndef __TERRALIB_QT_WIDGETS_RP_INTERNAL_TIEPOINTSLOCATORADVANCEDDIALOG_H
#define __TERRALIB_QT_WIDGETS_RP_INTERNAL_TIEPOINTSLOCATORADVANCEDDIALOG_H

// TerraLib
#include "../Config.h"
#include "../../../rp/TiePointsLocator.h"

// Qt
#include <QDialog>


// Forward user interface declaration
namespace Ui
{
  class TiePointsLocatorAdvancedForm;
}

namespace te
{
  namespace qt
  {
    namespace widgets
    {
      /*!
        \class TiePointsLocatorAdvancedDialog

        \brief A dialog used to execute tie points location advanced options.
      */
      class TEQTWIDGETSEXPORT TiePointsLocatorAdvancedDialog : public QDialog
      {
        Q_OBJECT
        
        public:
          
          /*! \brief The current parameters.
          */          
          te::rp::TiePointsLocator::InputParameters m_inputParameters;

          /*! \brief Constructs a basic dialog which is a child of parent, with widget flags set to f. 
              
              \param parent Parent widget pointer.
              
              \param f Widget flags.
          */
          TiePointsLocatorAdvancedDialog( QWidget* parent = 0,
            Qt::WindowFlags f = 0);

          /*! \brief Destructor. */
          ~TiePointsLocatorAdvancedDialog();

        protected slots:

          void on_okPushButton_clicked();
          
          // overloaded
          void showEvent( QShowEvent* );

        private:

          Ui::TiePointsLocatorAdvancedForm* m_uiPtr; //!< User interface.
          
      }; 
    }
  }
}



#endif
