#include "LayerPropertiesInfoWidget.h"
#include "LayerPropertiesInfo.h"

// Qt
#include <QtGui/QGridLayout>

te::qt::widgets::LayerPropertiesInfoWidget::LayerPropertiesInfoWidget(te::map::AbstractLayer* layer, QWidget* parent) :
  QWidget(parent)
{
  QWidget::resize(100, 100);
  QWidget::setMinimumSize(QSize(100, 100));
  QWidget::setWindowTitle(tr("Layer properties"));

  QtTreePropertyBrowser* pb = new QtTreePropertyBrowser(this);
  te::qt::widgets::LayerPropertiesInfo* info = new te::qt::widgets::LayerPropertiesInfo(pb, layer);

  pb->setPropertiesWithoutValueMarked(true);
  pb->setResizeMode(QtTreePropertyBrowser::ResizeToContents);

  QGridLayout* lay = new QGridLayout(this);
  lay->addWidget(pb);
}

te::qt::widgets::LayerPropertiesInfoWidget::~LayerPropertiesInfoWidget()
{
}