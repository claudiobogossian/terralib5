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
 \file terralib/srs/qt/SRSDialog.h
 
 \brief A dialog used to manage Spatial Reference Systems for coordinates.
 */

#ifndef __TERRALIB_SRS_INTERNAL_QT_SRSDIALOG_H
#define __TERRALIB_SRS_INTERNAL_QT_SRSDIALOG_H

// TerraLib
#include "../Config.h"

// Qt
#include <QDialog>

//STL
#include <string>

// Forward declaraion
namespace Ui { class SRSDialogForm; }

namespace te
{
  namespace qt
  {
    namespace widgets
    {
      class SRSDialog;
      
      /*!
       \class SRSDialog
       
       \brief A dialog used to build a SRSDialog element.

       \ingroup widgets
      */
      class TEQTWIDGETSEXPORT SRSDialog : public QDialog
      {
        Q_OBJECT
        
      public:

        SRSDialog( QWidget* parent = 0, Qt::WindowFlags f = 0);
        
        SRSDialog( QWidget* parent, Qt::WindowFlags f,
                  unsigned int srid, const std::string& name, const std::string& p4desc);
        
        ~SRSDialog();
        
        unsigned int getSRID() const;
        
        std::string getName() const;
        
        std::string getP4Desc() const;
        
        protected slots:

        void onOkPushButtonClicked();
        void onCancelPushButtonClicked();
        
      private:

        Ui::SRSDialogForm* m_ui;         //!< Dialog form.
        bool m_isUpdate;
      }; 
    } // end namespace widgets
  }   // end namespace qt
}     // end namespace te

#endif  // __TERRALIB_SRS_INTERNAL_QT_SRSDIALOG_H
