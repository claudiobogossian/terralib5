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
  \file terralib/edit/qt/tools/EditInfoTool.h

  \brief This class implements a concrete tool for edit attributes of geometry.
*/

#ifndef __TERRALIB_EDIT_QT_INTERNAL_EDITINFOTOOL_H
#define __TERRALIB_EDIT_QT_INTERNAL_EDITINFOTOOL_H

// TerraLib
#include "../../../geometry/Envelope.h"
#ifndef Q_MOC_RUN
#include "../../../maptools/AbstractLayer.h"
#endif
#include "../Config.h"
#include "GeometriesUpdateTool.h"

// Qt
#include <QPointF>
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QDialog>

namespace te
{
  namespace qt
  {
    namespace widgets
    {
      class Canvas;
      class MapDisplay;
    }
  }

  namespace edit
  {

    class TEEDITQTEXPORT EditInfoTool : public GeometriesUpdateTool
    {
      Q_OBJECT

    public:
      EditInfoTool(te::qt::widgets::MapDisplay* display, const te::map::AbstractLayerPtr& layer, QObject* parent = 0);

      ~EditInfoTool();

      //@}

      /** @name AbstractTool Methods
      *  Methods related with tool behavior.
      */
      //@{

      //bool mouseReleaseEvent(QMouseEvent* e);
      bool mousePressEvent(QMouseEvent* e);

      //@}

    private:

      void draw();

      void updateCursor();

      void pickFeature(const te::map::AbstractLayerPtr& layer, const QPointF& pos);

      te::gm::Envelope buildEnvelope(const QPointF& pos);

      void getInfo(const te::gm::Envelope& e);

      std::auto_ptr<te::dt::AbstractData> getValue(int type, QString value) const;

      void storeFeature();

      te::da::DataSet* m_dataset;                           //!< The attributes dataset type.
      std::map<std::size_t, te::dt::AbstractData*> m_data;
      std::vector<std::size_t> m_restrictivePropertyPos;
      QDialog* m_dialog;
      QTreeWidget* m_infoWidget;                            //!< Widget used to show the informations.

    private slots:

      void onCancelPushButtonPressed();

      void onOkPushButtonPressed();

      void onAttributesTreeWidgetItemDoubleClicked(QTreeWidgetItem* item, int column);

    };

  }   // end namespace edit
}     // end namespace te

#endif  // __TERRALIB_EDIT_QT_INTERNAL_EDITINFOTOOL_H
