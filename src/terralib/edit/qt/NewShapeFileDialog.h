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
  \file terralib/edit/qt/NewShapeFileDialog.h

  \brief A dialog used to create a shapefile layer.
*/

#ifndef __TERRALIB_EDIT_QT_INTERNAL_NEWSHAPEFILEDIALOG_H
#define __TERRALIB_EDIT_QT_INTERNAL_NEWSHAPEFILEDIALOG_H

// TerraLib
#ifndef Q_MOC_RUN
#include "../../maptools/AbstractLayer.h"
//#include "../../qt/widgets/canvas/MapDisplay.h"
#include "../../qt/widgets/layer/explorer/LayerItemView.h"
#endif
#include "Config.h"

// Qt
#include <QDialog>

// STL
#include <memory>

// Forward declaraion
namespace Ui { class NewShapeFileDialogForm; }

namespace te
{
  namespace edit
  {
    /*!
      \class SnapOptionsDialog

      \brief A dialog used to configure geometry snap options.
    */
    class TEEDITQTEXPORT NewShapeFileDialog : public QDialog
    {
      Q_OBJECT

      public:

        /** @name Initializer Methods
          *  Methods related to instantiation and destruction.
          */
        //@{

        /*! \brief Constructs the snap options dialog which is a child of parent, with widget flags set to f. */
        NewShapeFileDialog(QWidget* parent = 0, Qt::WindowFlags f = 0);

        /*! \brief Destructor. */
        ~NewShapeFileDialog();

        //@}

        void setLayerExplorer(te::qt::widgets::LayerItemView* m_layerExplorer);

        void setSRID(const int& srid);

      private slots:

        void onOkPushButtonPressed();

        void onMapSRIDTriggered();

      private:

        std::auto_ptr<Ui::NewShapeFileDialogForm> m_ui;   //!< Dialog form.
        //te::qt::widgets::MapDisplay* m_display;          //!< The map display associated with the tool.//getlayerexplorer..
        te::qt::widgets::LayerItemView* m_layerExplorer;
        int m_srid;
    };

  }   // end namespace edit
}     // end namespace te

#endif  // __TERRALIB_EDIT_QT_INTERNAL_NEWSHAPEFILEDIALOG_H
