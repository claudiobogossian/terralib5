#include "HLDelegateDecorator.h"

//TerraLib include files
#include <terralib/common/Exception.h>

//Qt4 include files
#include <QColor>
#include <QModelIndex>
#include <QPainter>
#include <QStyleOptionViewItem>
#include <QApplication>

namespace te 
{
  namespace qt 
  {
    namespace widgets 
    {
      HLDelegateDecorator::~HLDelegateDecorator()
      {
      }

      HighlightDelegate* HLDelegateDecorator::clone ()
      {
        HLDelegateDecorator* r = 0;

        if(m_decorated != 0)
        {
          r = new HLDelegateDecorator(m_decorated->clone());
          r->m_delDecorated = m_delDecorated;
        }

        r->copyAttributes(this);

        return r;
      }

      void HLDelegateDecorator::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
      {
        if(!index.isValid())
          return;

        if(HighlightDelegate::toHighlight(index))
          HighlightDelegate::paint(painter, option, index);
        else if(m_decorated != 0)
          m_decorated->paint(painter, option, index);
      }

      void HLDelegateDecorator::setPKeysColumns(const std::vector<size_t>& pKeyCols)
      {
        HighlightDelegate::setPKeysColumns(pKeyCols);

        if(m_decorated != 0)
          m_decorated->setPKeysColumns(pKeyCols);
      }

      void HLDelegateDecorator::setClassColor(const size_t& cPosition, const QColor& color)
      {
        HighlightDelegate* del = getDecorated(cPosition);

        if(del == 0)
          throw te::common::Exception(tr("Class position out of boundaries.").toAscii().data());

        del->setHighlightColor(color);
      }

      void HLDelegateDecorator::setClassSelection(const size_t& cPosition, const std::set<std::string>& sel)
      {
        HighlightDelegate* del = getDecorated(cPosition);

        if(del == 0)
          throw te::common::Exception(tr("Class position out of boundaries.").toAscii().data());

        del->setHighlightedPkeys(sel);
      }

      HighlightDelegate* HLDelegateDecorator::getDecorated(const size_t& pos)
      {
        size_t lim = this->getNumberOfClasses()-1;

        if(pos == lim)
          return this;

        if((m_decorated == 0) && (pos > lim))
          throw te::common::Exception(tr("Delegate position out of boundaries.").toAscii().data());

        HLDelegateDecorator* del = dynamic_cast<HLDelegateDecorator*>(m_decorated);

        if(del == 0)
        {
          if(pos > lim)
            throw te::common::Exception(tr("Delegate position out of boundaries.").toAscii().data());

          return m_decorated;
        }

        return del->getDecorated(pos);
      }

      int HLDelegateDecorator::getGroupPosition(const std::string& pKey) const
      {
        std::set<std::string>::iterator it = m_hlOids.find(pKey);

        if(it == m_hlOids.end())
        {
          if(m_decorated == 0)
            return -1;

          HLDelegateDecorator* aux = dynamic_cast<HLDelegateDecorator*>(m_decorated);

          return (aux != 0) ? (aux->getGroupPosition(pKey)) : -1;
        }

        return (int)this->getNumberOfClasses()-1;
      }

      size_t HLDelegateDecorator::getNumberOfClasses() const
      {
        if(m_decorated == 0)
          return 0;

        HLDelegateDecorator* dec = dynamic_cast<HLDelegateDecorator*>(m_decorated);

        if(dec == 0)
          return 1;

        return dec->getNumberOfClasses() + 1;
      }

      void HLDelegateDecorator::clearSelection()
      {
        if(m_decorated != 0)
          m_decorated->clearSelection();

        HighlightDelegate::clearSelection();
      }

      HighlightDelegate* HLDelegateDecorator::getDelegate(const size_t& numClasses)

      {
        HighlightDelegate* dele = new HighlightDelegate;

        for(size_t i = 0; i<numClasses; i++)
          dele = new HLDelegateDecorator(dele);

        return dele;
      }

      HLDelegateDecorator::HLDelegateDecorator(HighlightDelegate* decorated) :
      te::common::Decorator<HighlightDelegate>(decorated)
      {
        if(m_decorated)
        {
          m_decorated->setParent(this);
          m_pkeys = m_decorated->getPKeysColumns();
        }
      }
    } // namespace te::qt::widgets
  } // namespace te::qt
} // namespace te
