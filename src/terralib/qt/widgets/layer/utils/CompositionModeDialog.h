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
  \file terralib/qt/widgets/layer/utils/CompositionModeDialog.h

  \brief This file defines a class for a CompositionModeDialog.
*/

#ifndef __TERRALIB_QT_WIDGETS_LAYER_INTERNAL_COMPOSITIONMODEDIALOG_H
#define __TERRALIB_QT_WIDGETS_LAYER_INTERNAL_COMPOSITIONMODEDIALOG_H

// TerraLib
#include "../../../../maptools/AbstractLayer.h"
#include "../../Config.h"

//STL
#include <memory>

// Qt
#include <QDialog>

namespace Ui { class CompositionModeDialogForm; }

namespace te
{
  namespace qt
  {
    namespace widgets
    {
      /*!
        \class CompositionModeDialog

        \brief This class is a dialog used to select the composition mode for a layer.
      */
      class TEQTWIDGETSEXPORT CompositionModeDialog : public QDialog
      {
        Q_OBJECT

        public:

          /*! Default Constructor */
          CompositionModeDialog(QWidget* parent = 0, Qt::WindowFlags f = 0);

          /*! Default Destructor */
          ~CompositionModeDialog();

        public:

          /*!
            \brief Set the layer that can be used

            \param layer  Pointer to the selected layer.
          */
          void setLayer(te::map::AbstractLayerPtr layer);

          QWidget* getApplyButton();

        public slots:

          void onClearRadioButtonClicked(bool flag);

          void onSourceRadioButtonClicked(bool flag);

          void onDestinationRadioButtonClicked(bool flag);

          void onSourceOverRadioButtonClicked(bool flag);

          void onDestinationOverRadioButtonClicked(bool flag);

          void onSourceInRadioButtonClicked(bool flag);

          void onDestinationInRadioButtonClicked(bool flag);

          void onSourceOutRadioButtonClicked(bool flag);

          void onDestinationOutRadioButtonClicked(bool flag);

          void onSourceAtTopRadioButtonClicked(bool flag);

          void onDestinationAtTopRadioButtonClicked(bool flag);

          void onXorRadioButtonClicked(bool flag);

          void onPlusRadioButtonClicked(bool flag);

          void onMultiplyRadioButtonClicked(bool flag);

          void onScreenRadioButtonClicked(bool flag);

          void onOverlayRadioButtonClicked(bool flag);

          void onDarkenRadioButtonClicked(bool flag);

          void onLightenRadioButtonClicked(bool flag);

          void onColorDodgeRadioButtonClicked(bool flag);

          void onColorBurnRadioButtonClicked(bool flag);

          void onHardLightRadioButtonClicked(bool flag);

          void onSoftLightRadioButtonClicked(bool flag);

          void onDifferenceRadioButtonClicked(bool flag);

          void onExclusionRadioButtonClicked(bool flag);

        private:
          std::auto_ptr<Ui::CompositionModeDialogForm> m_ui;                          //!< Qt component

          te::map::AbstractLayerPtr m_layer;                                          //!< Selected Layer.
      };

    } // end namespace widgets
  }   // end namespace qt
}     // end namespace te

#endif  // __TERRALIB_QT_WIDGETS_LAYER_INTERNAL_COMPOSITIONMODEDIALOG_H
