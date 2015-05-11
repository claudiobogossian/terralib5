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
  m_acceptDrop(false),
  m_layer(0),
  m_dset(0)
{
}

void te::qt::widgets::DataSetTableHorizontalHeader::setDragDrop(bool b)
{
  m_doDragDrop = b;
}

bool te::qt::widgets::DataSetTableHorizontalHeader::getDragDrop()
{
  return m_doDragDrop;
}

void te::qt::widgets::DataSetTableHorizontalHeader::setAcceptDrop(bool b)
{
  m_acceptDrop = b;
  if(m_acceptDrop)
  {
    setAcceptDrops(true);
    setDropIndicatorShown(true);
  }
  else
  {
    setAcceptDrops(false);
    setDropIndicatorShown(false);
  }
}

bool te::qt::widgets::DataSetTableHorizontalHeader::getAcceptDrop()
{
  return m_acceptDrop;
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
        te::da::DataSourcePtr ds = GetDataSource(m_layer);
        std::string sid(ds->getId());
        mimeData->setData("DataSourceId", sid.c_str());
        std::string stype(ds->getType());
        mimeData->setData("DataSourceType", stype.c_str());
        m_connInfo = ds->getConnectionInfo();
        QString connInfo;
        connInfo.setNum((unsigned long long)&m_connInfo);
        mimeData->setData("DataSourceConnInfo", connInfo.toStdString().c_str());
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

      QDrag drag(this);
      drag.setMimeData(mimeData);
      drag.setPixmap(pixmap);
      drag.setHotSpot(hotSpot);

      Qt::DropAction dropAction = drag.exec(Qt::LinkAction);    
    }
  }
  //else if(e->button() == Qt::MiddleButton) // code for drag and drop test
  //{
  //  m_doDragDrop = !m_doDragDrop;
  //  m_acceptDrop = m_doDragDrop;
  //  setDragDrop(m_doDragDrop);
  //  setAcceptDrop(m_acceptDrop);
  //  if(m_doDragDrop)
  //    QMessageBox::information(this, "", "DRAG DROP ENABLED");
  //  else
  //    QMessageBox::information(this, "", "DRAG DROP DISABLED");
  //}

  QHeaderView::mousePressEvent(e);
}

void te::qt::widgets::DataSetTableHorizontalHeader::dragEnterEvent(QDragEnterEvent *e)
{
  if(e->source() == this || m_acceptDrop == false)
    return;

  const QMimeData* mdata = e->mimeData();
  if(mdata->text() == "TerraView: Link Table")
    e->setAccepted(true);
  else
    e->setAccepted(false);
}

void te::qt::widgets::DataSetTableHorizontalHeader::dragMoveEvent(QDragMoveEvent *e)
{
  if(e->source() == this || m_acceptDrop == false)
    return;

  const QMimeData* mdata = e->mimeData();
  if(mdata->text() == "TerraView: Link Table")
    e->setAccepted(true);
  else
    e->setAccepted(false);
}

void te::qt::widgets::DataSetTableHorizontalHeader::dropEvent(QDropEvent *e)
{
  if(e->source() == this || m_acceptDrop == false)
    return;

  const QMimeData* mdata = e->mimeData();
  std::string text = mdata->text().toStdString();
  if(text == "TerraView: Link Table")
  {
    e->setDropAction(Qt::LinkAction);

    if(m_layer)
    {
      te::da::DataSourcePtr sds = GetDataSource(m_layer);
      m_secondLinkInfo.m_dataSourceId = sds->getId();
      m_secondLinkInfo.m_dataSourceType = sds->getType();
      m_secondLinkInfo.m_connInfo = sds->getConnectionInfo();
      m_secondLinkInfo.m_layerId = m_layer->getId();
      m_secondLinkInfo.m_layerName = m_layer->getSchema()->getName();
      m_secondLinkInfo.m_layerTitle = m_layer->getTitle();
      m_secondLinkInfo.m_layerType = m_layer->getType();

      if(mdata->data("DataSourceId").isNull() == false)
      {
        m_firstLinkInfo.m_dataSourceId = mdata->data("DataSourceId").data();
        m_firstLinkInfo.m_dataSourceType = mdata->data("DataSourceType").data();
        QString connInfo(mdata->data("DataSourceConnInfo").data());
        m_firstLinkInfo.m_connInfo = *(std::map<std::string, std::string>*)connInfo.toULongLong();
        m_firstLinkInfo.m_layerName = mdata->data("LayerName").data();
        m_firstLinkInfo.m_layerTitle = mdata->data("LayerTitle").data();
        m_firstLinkInfo.m_layerId = mdata->data("LayerId").data();
        m_firstLinkInfo.m_layerType = mdata->data("LayerType").data();

        if(m_firstLinkInfo.m_dataSourceId != m_secondLinkInfo.m_dataSourceId)
        {
          QMessageBox::warning(this, "Link Error", "The data are from different sources");
          return;
        }

        if(m_firstLinkInfo.m_layerId == m_secondLinkInfo.m_layerId)
        {
          QMessageBox::warning(this, "Link Error", "The data are from the same layer");
          return;
        }
      }
    }

    QString fromFieldNumber(mdata->data("FieldNumber").data());
    m_firstLinkInfo.m_fieldNumber = fromFieldNumber.toUInt();
    m_firstLinkInfo.m_fieldName = mdata->data("FieldName").data();
    QString fromFieldType(mdata->data("FieldType").data());
    m_firstLinkInfo.m_fieldType = fromFieldType.toUInt();

    m_secondLinkInfo.m_fieldNumber = logicalIndexAt(e->pos());
    m_secondLinkInfo.m_fieldType = m_dset->getPropertyDataType(m_secondLinkInfo.m_fieldNumber);
    if(m_secondLinkInfo.m_fieldType != m_firstLinkInfo.m_fieldType)
    {
      QMessageBox::warning(this, "Link Error", "Field types do not match");
      return;
    }
    m_secondLinkInfo.m_fieldName = m_dset->getPropertyName(m_secondLinkInfo.m_fieldNumber);

    if(mdata->data("DataSourceId").isNull() == false && m_layer)
      emit linkTable(m_firstLinkInfo, m_secondLinkInfo);
    else
      emit linkTable(m_firstLinkInfo.m_fieldName, m_secondLinkInfo.m_fieldName);
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