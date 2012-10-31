#include "HighlightDelegate.h"

#include "DataSetModel.h"

namespace te 
{
  namespace qt 
  {
    namespace widgets 
    {
      HighlightDelegate::HighlightDelegate(QObject * parent) :
      QItemDelegate(parent),
      m_enabled(true)
      {
      }

      HighlightDelegate::~HighlightDelegate()
      {
      }

      HighlightDelegate* HighlightDelegate::clone ()
      {
        HighlightDelegate* cl = new HighlightDelegate(parent());
        cl->copyAttributes(this);

        return cl;
      }

      void HighlightDelegate::paint(QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index) const
      {
        if(!index.isValid())
          return;

        QStyleOptionViewItem opt = option;

        if(toHighlight(index))
        {
          opt.showDecorationSelected = true;
          opt.state |= QStyle::State_Selected;
          opt.palette.setColor(QPalette::Highlight, m_color);
          opt.palette.setColor(QPalette::HighlightedText, Qt::black);
        }

        QItemDelegate::paint(painter, opt, index);
      }

      void HighlightDelegate::setEnabled(const bool& enable)
      {
        m_enabled = enable;
      }

      void HighlightDelegate::setHighlightedPkeys(const std::set<std::string> & value)
      {
        m_hlOids = value;
      }

      void HighlightDelegate::setHighlightColor(const QColor & value)
      {
        m_color = value;
      }

      QColor HighlightDelegate::getHighlightColor() const
      {
        return m_color;
      }

      void HighlightDelegate::setPKeysColumns(const std::vector<size_t> & pKeyCols)
      {
        m_pkeys = pKeyCols;
      }

      std::vector<size_t> HighlightDelegate::getPKeysColumns()
      {
        return m_pkeys;
      }

      std::string HighlightDelegate::getPKey(const QModelIndex & index, char separator) const
      {
        return index.data(DataSetModel::PKEY).toString().toStdString();
      }

      std::set<std::string> HighlightDelegate::getHighlightKeys()
      {
        return m_hlOids;
      }

      void HighlightDelegate::clearSelection()
      {
        m_hlOids.clear();
      }

      void HighlightDelegate::setGroupName(const QString& name)
      {
        m_grp_name = name;
      }
      
      QString HighlightDelegate::getGroupName() const
      {
        return m_grp_name;
      }

      bool HighlightDelegate::toHighlight(const QModelIndex& idx) const
      {
        std::string key = getPKey(idx);
        return (!key.empty() && m_enabled && (m_hlOids.find(key) != m_hlOids.end()));
      }

      void HighlightDelegate::copyAttributes(HighlightDelegate* other)
      {
        m_color = other->m_color;
        m_enabled = other->m_enabled;
        m_grp_name = other->m_grp_name;
        m_hlOids = other->m_hlOids;
        m_pkeys = other->m_pkeys;
      }
    } // namespace te::qt::widgets
  } // namespace te::qt
} // namespace te
