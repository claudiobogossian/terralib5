#ifndef __TERRALIB_EXAMPLE_STARDELEGATE_H
#define __TERRALIB_EXAMPLE_STARDELEGATE_H

//! TerraLib include files
#include <terralib/common/Decorator.h>
#include <terralib/qt/widgets/dataview/HLDelegateDecorator.h>

//! Qt include files 
#include <QImage>

class StarDelegate : public te::qt::widgets::HLDelegateDecorator
{

public:
  StarDelegate(const size_t nClass = 0);

  StarDelegate(te::qt::widgets::HighlightDelegate* decorated);

  void paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const;

  void setHighlightColor(const QColor & value);

protected:

  void init();

  mutable QImage m_img;
};

#endif //__TERRALIB_EXAMPLE_STARDELEGATE_H
