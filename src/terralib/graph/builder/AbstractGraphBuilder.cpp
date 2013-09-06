#include "AbstractGraphBuilder.h"

te::graph::AbstractGraphBuilder::AbstractGraphBuilder()
{
  m_graph = 0;
  m_errorMessage = "";
}

te::graph::AbstractGraphBuilder::~AbstractGraphBuilder()
{
}

std::string te::graph::AbstractGraphBuilder::getErrorMessage()
{
  return m_errorMessage;
}
       
te::graph::AbstractGraph* te::graph::AbstractGraphBuilder::getGraph()
{
  return m_graph;
}