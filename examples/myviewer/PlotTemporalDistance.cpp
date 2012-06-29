#include "PlotTemporalDistance.h"

//TerraLib
#include <terralib/dataaccess.h>

//QT
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QGroupBox>
#include <QAction>
#include <QMessageBox>

PlotTemporalDistance::PlotTemporalDistance(std::vector<te::map::AbstractLayer*>& layers, QWidget* parent, Qt::WindowFlags f) : 
  QDialog(parent, f)
{
  setWindowTitle("Plot Temporal Distance");
  QVBoxLayout* vlayout = new QVBoxLayout(this);
  QLabel* label1 = new QLabel("Moving Object 1:", this);
  m_layer1ComboBox = new QComboBox(this);
  QHBoxLayout* layout1 = new QHBoxLayout(this);
  layout1->addWidget(label1);
  layout1->addWidget(m_layer1ComboBox);
  vlayout->addLayout(layout1);

  QLabel* label2 = new QLabel("Moving Object 2:", this);
  m_layer2ComboBox = new QComboBox(this);
  QHBoxLayout* layout2 = new QHBoxLayout(this);
  layout2->addWidget(label2);
  layout2->addWidget(m_layer2ComboBox);
  vlayout->addLayout(layout2);

  QGroupBox* gb = new QGroupBox(this);
  m_okPushButton = new QPushButton("Ok", gb);
  m_cancelPushButton = new QPushButton("Cancel", gb);
  QHBoxLayout* layout3 = new QHBoxLayout(gb);
  layout3->addStretch();
  layout3->addWidget(m_okPushButton);
  layout3->addStretch();
  layout3->addWidget(m_cancelPushButton);
  layout3->addStretch();
  vlayout->addWidget(gb);

  setLayout(vlayout);

  QObject::connect(m_okPushButton, SIGNAL(clicked()), this, SLOT(okSlot()));
  QObject::connect(m_cancelPushButton, SIGNAL(clicked()), this, SLOT(cancelSlot()));

  std::vector<te::map::AbstractLayer*>::iterator it;
  for(it = layers.begin(); it != layers.end(); ++it)
  {
    m_layer1ComboBox->addItem((*it)->getTitle().c_str());
    m_layer2ComboBox->addItem((*it)->getTitle().c_str());
  }
}

PlotTemporalDistance::~PlotTemporalDistance()
{
  delete m_layer1ComboBox;
  delete m_layer2ComboBox;
  delete m_okPushButton;
  delete m_cancelPushButton;
}

void PlotTemporalDistance::okSlot()
{
  if(m_layer1ComboBox->currentText() == m_layer2ComboBox->currentText())
  {
    QMessageBox::information(this, tr("Select different Moving Objects!"), tr("Choose two different objects"));
    return;
  }

  accept();
  hide();
}

void PlotTemporalDistance::cancelSlot()
{
  reject();
  hide();
}
