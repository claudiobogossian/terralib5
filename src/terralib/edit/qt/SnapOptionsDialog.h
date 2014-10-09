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
  \file terralib/edit/qt/SnapOptionsWidget.h

  \brief A widget used to configure geometry snap options.
*/

#ifndef __TERRALIB_EDIT_QT_INTERNAL_SNAPOPTIONSWIDGET_H
#define __TERRALIB_EDIT_QT_INTERNAL_SNAPOPTIONSWIDGET_H

// TerraLib
#include "Config.h"

// Qt
#include <QDialog>

// STL
#include <memory>

// Forward declaraion
namespace Ui { class SnapOptionsDialogForm; }

namespace te
{
  namespace edit
  {
    /*!
      \class SnapOptionsWidget

      \brief A widget used to configure geometry snap options.
    */
    class TEEDITQTEXPORT SnapOptionsWidget : public QDialog
    {
      Q_OBJECT

      public:

        /** @name Initializer Methods
          *  Methods related to instantiation and destruction.
          */
        //@{

        /*! \brief Constructs the snap options widget which is a child of parent, with widget flags set to f. */
        SnapOptionsWidget(QWidget* parent = 0, Qt::WindowFlags f = 0);

        /*! \brief Destructor. */
        ~SnapOptionsWidget();

        //@}

      private:

        std::auto_ptr<Ui::SnapOptionsDialogForm> m_ui; //!< Widget form.
    };

  }   // end namespace edit
}     // end namespace te

#endif  // __TERRALIB_EDIT_QT_INTERNAL_SNAPOPTIONSWIDGET_H
