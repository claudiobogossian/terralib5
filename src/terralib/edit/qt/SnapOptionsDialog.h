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
  \file terralib/edit/qt/SnapOptionsDialog.h

  \brief A dialog used to configure geometry snap options.
*/

#ifndef __TERRALIB_EDIT_QT_INTERNAL_SNAPOPTIONSDIALOG_H
#define __TERRALIB_EDIT_QT_INTERNAL_SNAPOPTIONSDIALOG_H

// TerraLib
#include "../../maptools/AbstractLayer.h"
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
      \class SnapOptionsDialog

      \brief A dialog used to configure geometry snap options.
    */
    class TEEDITQTEXPORT SnapOptionsDialog : public QDialog
    {
      Q_OBJECT

      public:

        /** @name Initializer Methods
          *  Methods related to instantiation and destruction.
          */
        //@{

        /*! \brief Constructs the snap options dialog which is a child of parent, with widget flags set to f. */
        SnapOptionsDialog(QWidget* parent = 0, Qt::WindowFlags f = 0);

        /*! \brief Destructor. */
        ~SnapOptionsDialog();

        //@}

        void setLayers(const std::list<te::map::AbstractLayerPtr>& layers);

      private:

        void buildOptions();

        void buildOptions(const te::map::AbstractLayerPtr& layer);

      private slots:

        void onOkPushButtonPressed();

      private:

        std::auto_ptr<Ui::SnapOptionsDialogForm> m_ui;   //!< Dialog form.
        std::list<te::map::AbstractLayerPtr> m_layers;   //!< The layer list.
    };

  }   // end namespace edit
}     // end namespace te

#endif  // __TERRALIB_EDIT_QT_INTERNAL_SNAPOPTIONSWIDGET_H
