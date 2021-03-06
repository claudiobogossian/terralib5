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
\file terralib/mnt/qt/VolumeDialog.h

\brief A dialog Calculate Volume
*/

#ifndef __TERRALIB_MNT_INTERNAL_VOLUMEDIALOG_H
#define __TERRALIB_MNT_INTERNAL_VOLUMEDIALOG_H

// Terralib
#ifndef Q_MOC_RUN
#include "../../dataaccess/datasource/DataSourceInfo.h"
#include "../../maptools/AbstractLayer.h"
#endif
#include "../core/Config.h"
#include "../core/Enums.h"

// STL
#include <list>
#include <map>

// Qt
#include <QDialog>

namespace Ui { class VolumeDialogForm; }

namespace te
{
  namespace mnt
  {
    class TEMNTEXPORT VolumeDialog : public QDialog
    {
      Q_OBJECT

    public:
      VolumeDialog(QWidget* parent = 0, Qt::WindowFlags f = 0);

      ~VolumeDialog();

      /*!
      \brief Set the layer that can be used
      \param layers   List of AbstractLayerPtr
      */
      void setLayers(std::list<te::map::AbstractLayerPtr> layers);

      te::map::AbstractLayerPtr getLayer();

      protected slots:

      void onInputRasterToolButtonClicked();
      void onRasterInputComboBoxChanged(int index);
      void onDummyLineEditEditingFinished();
      void onInputVectorToolButtonClicked();
      void onVectorInputComboBoxChanged(int index);
      void onHelpPushButtonClicked();
      void onOkPushButtonClicked();
      void onCancelPushButtonClicked();

    private:

      std::auto_ptr<Ui::VolumeDialogForm> m_ui;

      std::list<te::map::AbstractLayerPtr> m_layers;                                    //!< List of layers.
      te::map::AbstractLayerPtr m_rasterinputLayer;                                        //!< Input layer
      te::map::AbstractLayerPtr m_vectorinputLayer;                                        //!< Input layer

    }; //class VolumeDialog
  } //mnt
} //te



#endif //__TERRALIB_MNT_INTERNAL_VOLUMEDIALOG_H
