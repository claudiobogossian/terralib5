#include "MyLayer.h"

MyLayer::MyLayer(const std::string& id, const std::string& title, AbstractLayer* parent) : 
  te::map::Layer(id, title, parent),
  m_op(0),
  m_temporal(false)
{
}

MyLayer::~MyLayer()
{
}

void MyLayer::setDataGridOperation(te::map::DataGridOperation* op)
{
  m_op = op;
}

te::map::DataGridOperation* MyLayer::getDataGridOperation()
{
  return m_op;
}

void MyLayer::setTemporal(bool b)
{
  m_temporal = b;
}

bool MyLayer::isTemporal()
{
  return m_temporal;
}
