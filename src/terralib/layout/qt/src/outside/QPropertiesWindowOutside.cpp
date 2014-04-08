/*  Copyright (C) 2001-2014 National Institute For Space Research (INPE) - Brazil.

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
  \file QPropertiesWindowOutside.cpp
   
  \brief 

  \ingroup layout
*/

// TerraLib
#include "QPropertiesWindowOutside.h"
#include "LayoutContext.h"
#include "LayoutScene.h"
#include "QLayoutScene.h"
#include "LayoutOutsideModelObservable.h"
#include "LayoutItemObserver.h"
#include "LayoutOutsideObserver.h"
#include "LayoutOutsideController.h"
#include "../../../../geometry/Envelope.h"
#include "QLayoutPropertyBrowser.h"

// Qt
#include <QGraphicsWidget>
#include <QGroupBox>
#include <QRegExpValidator>
#include <QRegExp>
#include "../../../../../../third-party/qt/propertybrowser/qtvariantproperty.h"
#include "../../../../../../third-party/qt/propertybrowser/qttreepropertybrowser.h"

// Boost
#include <boost/foreach.hpp>
#include <boost/property_tree/ptree.hpp>

te::layout::QPropertiesWindowOutside::QPropertiesWindowOutside( LayoutOutsideController* controller, LayoutObservable* o ) :
	QDockWidget("", 0, 0),
	LayoutOutsideObserver(controller, o),
  m_updatingValues(false)
{
	te::gm::Envelope box = m_model->getBox();	
	setBaseSize(box.getWidth(), box.getHeight());
	setVisible(false);
	setWindowTitle("Layout - Propriedades");
  setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

  setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);

  m_layoutPropertyBrowser = new QLayoutPropertyBrowser;

  createLayout();
}

te::layout::QPropertiesWindowOutside::~QPropertiesWindowOutside()
{

}

void te::layout::QPropertiesWindowOutside::createLayout()
{
  //Layout

  QVBoxLayout* layout = new QVBoxLayout(this);
  layout->setMargin(0);

  QHBoxLayout* filterLayout = new QHBoxLayout;

  m_configurePropertyEditor = new QToolButton(this);
  m_configurePropertyEditor->setText(tr("Config"));
  m_configurePropertyEditor->setBaseSize(QSize(16,16));

  m_propertyFilterEdit = new QLineEdit(this);
  m_propertyFilterEdit->setToolTip(tr("String or regular expression to filter property list with"));

  connect(m_propertyFilterEdit,SIGNAL(textChanged(QString)),
    m_layoutPropertyBrowser,SLOT(onChangeFilter(QString)));
  filterLayout->addWidget(m_propertyFilterEdit);
  filterLayout->addWidget(m_configurePropertyEditor);

  layout->addLayout(filterLayout);
  m_nameLabel = new QLabel(tr("QObject::unknown"), this);
  layout->addWidget(m_nameLabel);
  layout->addWidget(m_layoutPropertyBrowser->getPropertyEditor());

  QGroupBox* groupBox = new QGroupBox;
  groupBox->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
  groupBox->setLayout(layout);

  setWidget(groupBox);
}

void te::layout::QPropertiesWindowOutside::updateObserver( ContextLayoutItem context )
{
	setVisible(context.getShow());
	if(context.getShow() == true)
		show();
	else
		hide();
}

void te::layout::QPropertiesWindowOutside::setPosition( const double& x, const double& y )
{
	move(x,y);
	refresh();
}

te::gm::Coord2D te::layout::QPropertiesWindowOutside::getPosition()
{
  QPointF posF = pos();
  qreal valuex = posF.x();
  qreal valuey = posF.y();

  te::gm::Coord2D coordinate;
  coordinate.x = valuex;
  coordinate.y = valuey;

  return coordinate;
}

void te::layout::QPropertiesWindowOutside::itemsSelected(QList<QGraphicsItem*> graphicsItems)
{
  m_updatingValues = false;

  m_layoutPropertyBrowser->clearAll();

  m_graphicsItems = graphicsItems;
    
  foreach( QGraphicsItem *item, graphicsItems) 
  {
    if (item)
    {			
      QGraphicsWidget* outside = dynamic_cast<QGraphicsWidget*>(item);

      if(outside)
        continue;

      LayoutItemObserver* lItem = dynamic_cast<LayoutItemObserver*>(item);
      if(lItem)
      {
        //boost::property_tree::ptree pjson = lItem->getJSONProperties();
        LayoutProperties* props = const_cast<LayoutProperties*>(lItem->getProperties());

        foreach( LayoutProperty prop, props->getProperties()) 
        {
          m_layoutPropertyBrowser->addProperty(prop);
        }
        
        //if(!pjson.empty())
        //{
        //  BOOST_FOREACH(boost::property_tree::ptree::value_type &v, pjson.get_child("array"))
        //  {           
        //    /*property = m_variantPropertyEditorManager->addProperty(QVariant::String, tr(v.first.data()));
        //    property->setValue(QVariant(v.second.get<bool>("flag1")));
        //    addProperty(property, QLatin1String(v.first.data()));*/
        //  }
        //}
      }
    }
  }

  update();
}

