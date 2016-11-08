/*  Copyright(C) 2008 National Institute For Space Research(INPE) - Brazil.

This file is part of the TerraLib - a Framework for building GIS enabled applications.

TerraLib is free software : you can redistribute it and / or modify
it under the terms of the GNU Lesser General Public License as published by
the Free Software Foundation, either version 3 of the License,
or(at your option) any later version.

TerraLib is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.See the
GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License
along with TerraLib.See COPYING.If not, write to
TerraLib Team at <terralib - team@terralib.org>.
*/

/*!

\file terralib/mnt/qt/LayerSearchDialog.h

\brief A dialog Layer Search
*/

#ifndef __TERRALIB_MNT_INTERNAL_LAYERSEARCHDIALOG_H
#define __TERRALIB_MNT_INTERNAL_LAYERSEARCHDIALOG_H

//terralib
#include "../../maptools/AbstractLayer.h"
#include "../core/Config.h"
#include "../core/Enums.h"

// STL
#include <list>
#include <map>

// Qt
#include <QDialog>
#include <QList>

namespace Ui { class LayerSearchDialogForm; }

namespace te
{
  namespace mnt
  {
    class TEMNTEXPORT LayerSearchDialog : public QDialog
    {
      Q_OBJECT

    public:
      LayerSearchDialog(QWidget* parent = 0, Qt::WindowFlags f = 0, bool mnt = true);

      ~LayerSearchDialog();

      void setLayers(std::list<te::map::AbstractLayerPtr> layers);
      void setActive(const QList<mntType> &types);

      te::map::AbstractLayerPtr getLayer();
      int getLayerIndex() { return m_selectlayer_index; }

      const QString& GetGeometryName(te::gm::GeomType t);
      void ApplyFilter();

      protected slots:

      void onstartsEnabled(bool);
      void onendsEnabled(bool);
      void oncontainsEnabled(bool);
      void onequalsEnabled(bool);
      void onnameLineEditEditingFinished();

      void onisolinesEnabled(bool);
      void onsamplesEnabled(bool);
      void ongridEnabled(bool);
      void ontinEnabled(bool);
      void onothersEnabled(bool);
      void onallEnabled(bool);

      void onHelpPushButtonClicked();
      void onOkPushButtonClicked();
      void onCancelPushButtonClicked();

    private:

      std::auto_ptr<Ui::LayerSearchDialogForm> m_ui;

      std::list<te::map::AbstractLayerPtr> m_layers;                                    //!< List of layers.
      QStringList m_geom_name;
      QString m_namefilter;
      QString m_geomfilter;
      Qt::MatchFlags m_matchflags;

      te::map::AbstractLayerPtr m_selectLayer;                                          //!< Selected Layer.
      int m_selectlayer_index;

    }; //class LayerSearchDialog
  } //mnt
} //te

#endif
