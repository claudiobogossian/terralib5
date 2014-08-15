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


#include "DataSetTableHorizontalHeader.h"
#include "../../../dataaccess/dataset/DataSet.h"
#include "../../../dataaccess/datasource/DataSourceManager.h"
#include "../../../dataaccess/datasource/DataSourceTransactor.h"
#include "../../../dataaccess/query/DataSetName.h"
#include "../../../dataaccess/query/Field.h"
#include "../../../dataaccess/query/From.h"
#include "../../../dataaccess/query/OrderBy.h"
#include "../../../dataaccess/query/OrderByItem.h"
#include "../../../dataaccess/query/Select.h"
#include "../../../dataaccess/utils/Utils.h"
#include "../../../maptools/DataSetLayer.h"
#include "../../../maptools/DataSetAdapterLayer.h"
#include "../../../maptools/QueryLayer.h"

#include "DataSetTableView.h"

// Qt
#include <QtGui/QMouseEvent>
#include <QMimeData>
#include <QDrag>
#include <QLabel>
#include <QPainter>
#include <QMessageBox>

te::qt::widgets::DataSetTableHorizontalHeader::DataSetTableHorizontalHeader(DataSetTableView *view) :
QHeaderView(Qt::Horizontal, view),
  m_view(view),
  m_doDragDrop(false),
  m_layer(0),
  m_dset(0)
{
  if(m_doDragDrop)
  {
    setMouseTracking(true);
    setAcceptDrops(true);
    setDragDropMode(QAbstractItemView::DragDrop);
    setDropIndicatorShown(true);
  }
}

void te::qt::widgets::DataSetTableHorizontalHeader::setDragDrop(bool b)
{
  m_doDragDrop = b;
  if(m_doDragDrop)
  {
    setMouseTracking(true);
    setAcceptDrops(true);
    setDragDropMode(QAbstractItemView::DragDrop);
    setDropIndicatorShown(true);
  }
  else
  {
    setMouseTracking(false);
    setAcceptDrops(false);
    setDragDropMode(QAbstractItemView::NoDragDrop);
    setDropIndicatorShown(false);
  }
}

void te::qt::widgets::DataSetTableHorizontalHeader::setLayer(const te::map::AbstractLayer* layer)
{
  m_layer = layer;
}

void te::qt::widgets::DataSetTableHorizontalHeader::setDataSet(te::da::DataSet* dset)
{
  m_dset = dset;
}

void te::qt::widgets::DataSetTableHorizontalHeader::mousePressEvent(QMouseEvent *e)
{
  if(e->button() == Qt::LeftButton)
  {
    if(m_doDragDrop == false)
    {
      int c = m_view->columnAt(e->pos().x());
      QItemSelectionModel* sel = m_view->selectionModel();

      if(c > 0)
      {
        QItemSelectionModel::SelectionFlag selF = (sel->isColumnSelected(c, QModelIndex())) ? QItemSelectionModel::Deselect : QItemSelectionModel::Select;

        if(m_view->selectionMode() == QAbstractItemView::SingleSelection)
          sel->clear();


        QModelIndex tl = m_view->model()->index(0, c);
        QModelIndex br = m_view->model()->index(model()->rowCount()-1, c);

        QItemSelection newSel(tl, br);

        sel->select(newSel, selF);
      }
      else
        sel->clear();
    }
    else
    {
      QPixmap p(200, 50);
      QPainter painter(&p);
      QRect r = painter.boundingRect(p.rect(), Qt::AlignCenter, "Link Table");
      r = QRect(0, 0, r.width(), r.height());
      QPoint hotSpot(r.center());
      QPixmap pixmap(r.size());
      painter.end();
      painter.begin(&pixmap);
      painter.setPen(QPen(Qt::blue));
      painter.drawText(r, "Link Table");
      painter.end();

      QMimeData *mimeData = new QMimeData;
      mimeData->setText("TerraView: Link Table");

      if(m_layer)
      {
        std::string sid(GetDataSource(m_layer)->getId());
        mimeData->setData("DataSourceId", sid.c_str());
        std::string lname(m_layer->getSchema()->getName());
        mimeData->setData("LayerName", lname.c_str());
        std::string ltytle(m_layer->getTitle());
        mimeData->setData("LayerTitle", ltytle.c_str());
        std::string lid(m_layer->getId());
        mimeData->setData("LayerId", lid.c_str());
        std::string ltype(m_layer->getType());
        mimeData->setData("LayerType", ltype.c_str());
      }

      size_t col = logicalIndexAt(e->pos());
      QString s;
      s.setNum(col);
      mimeData->setData("FieldNumber", s.toStdString().c_str());

      std::string colName = m_dset->getPropertyName(col);
      mimeData->setData("FieldName", colName.c_str());

      int propType = m_dset->getPropertyDataType(col);
      s.setNum(propType);
      mimeData->setData("FieldType", s.toStdString().c_str());

      QDrag *drag = new QDrag(this);
      drag->setMimeData(mimeData);
      drag->setPixmap(pixmap);
      drag->setHotSpot(hotSpot);

      Qt::DropAction dropAction = drag->exec(Qt::LinkAction);    
    }
  }

  QHeaderView::mousePressEvent(e);
}

void te::qt::widgets::DataSetTableHorizontalHeader::dragEnterEvent(QDragEnterEvent *e)
{
  if(e->source() == this)
    return;

  const QMimeData* mdata = e->mimeData();
  if(mdata->text() == "TerraView: Link Table")
    e->setAccepted(true);
  else
    e->setAccepted(false);
}

void te::qt::widgets::DataSetTableHorizontalHeader::dragMoveEvent(QDragMoveEvent *e)
{
  if(e->source() == this)
    return;

  const QMimeData* mdata = e->mimeData();
  if(mdata->text() == "TerraView: Link Table")
    e->setAccepted(true);
  else
    e->setAccepted(false);
}

void te::qt::widgets::DataSetTableHorizontalHeader::dropEvent(QDropEvent *e)
{
  if(e->source() == this)
    return;

  const QMimeData* mdata = e->mimeData();
  std::string text = mdata->text().toStdString();
  if(text == "TerraView: Link Table")
  {
    e->setDropAction(Qt::LinkAction);

    QString fromFieldNumber(mdata->data("FieldNumber").data());
    m_linkFromFieldNumber = fromFieldNumber.toUInt();
    m_linkFromFieldName = mdata->data("FieldName").data();
    QString fromFieldType(mdata->data("FieldType").data());
    m_linkFromFieldType = fromFieldType.toUInt();

    m_linkToFieldNumber = logicalIndexAt(e->pos());
    m_linkToFieldType = m_dset->getPropertyDataType(m_linkToFieldNumber);
    if(m_linkToFieldType != m_linkFromFieldType)
    {
      QMessageBox::warning(this, "Link Error", "Field types do not match");
      return;
    }
    m_linkToFieldName = m_dset->getPropertyName(m_linkToFieldNumber);

    m_linkToDataSourceId = GetDataSource(m_layer)->getId();
    m_linkToLayerId = m_layer->getId();
    m_linkToLayerName = m_layer->getSchema()->getName();
    m_linkToLayerTitle = m_layer->getTitle();
    m_linkToLayerType = m_layer->getType();

    if(mdata->data("DataSourceId").isNull() == false)
    {
      m_linkFromDataSourceId = mdata->data("DataSourceId").data();
      m_linkFromLayerName = mdata->data("LayerName").data();
      std::string fromLayerTitle = mdata->data("LayerTitle").data();
      m_linkFromLayerId = mdata->data("LayerId").data();
      m_linkFromLayerType = mdata->data("LayerType").data();

      if(m_linkFromDataSourceId != m_linkToDataSourceId)
      {
        QMessageBox::warning(this, "Link Error", "The data are from different sources");
        return;
      }

      if(m_linkFromLayerId == m_linkToLayerId)
      {
        QMessageBox::warning(this, "Link Error", "The data are from the same layer");
        return;
      }
    }
    emit linkTable(m_linkFromFieldName, m_linkToFieldName);
  }
}

te::da::DataSourcePtr te::qt::widgets::DataSetTableHorizontalHeader::GetDataSource(const te::map::AbstractLayer* layer)
{
  // Getting data source, if it is available
  te::da::DataSourcePtr ds;

  if(layer->getType() == "DATASETLAYER")
  {
    const te::map::DataSetLayer* l = dynamic_cast<const te::map::DataSetLayer*>(layer);

    if(l != 0)
      ds = te::da::DataSourceManager::getInstance().find(l->getDataSourceId());
  }
  else if(layer->getType() == "DATASETADAPTERLAYER")
  {
    const te::map::DataSetAdapterLayer* l = dynamic_cast<const te::map::DataSetAdapterLayer*>(layer);

    if(l != 0)
      ds = te::da::DataSourceManager::getInstance().find(l->getDataSourceId());
  }
  else if(layer->getType() == "QUERYLAYER")
  {
    const te::map::QueryLayer* l = dynamic_cast<const te::map::QueryLayer*>(layer);

    if(l != 0)
      ds = te::da::DataSourceManager::getInstance().find(l->getDataSourceId());
  }

  return ds;
}