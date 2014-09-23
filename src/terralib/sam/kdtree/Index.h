/*  Copyright (C) 2001-2009 National Institute For Space Research (INPE) - Brazil.

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
  \file terralib/sam/kdtree/Index.h

  \brief A class that represents a two dimensional K-d Tree (2-d Tree).
*/

#ifndef __TERRALIB_SAM_KDTREE_INTERNAL_INDEX_H
#define __TERRALIB_SAM_KDTREE_INTERNAL_INDEX_H

// TerraLib
#include "../../geometry/Envelope.h"
#include "Node.h"

// STL
#include <limits>
#include <vector>
#include <utility>

namespace te
{
  namespace sam
  {
    namespace kdtree
    {
      /*!
        \class Index
        
        \brief A class that represents a two dimensional K-d Tree (2-d Tree).

        \note This type of tree stores the data into nodes (not only in the leafs node).

        \note This tree may be built by two ways:
              1) Inserting each element in the tree. In this case the tree can becomes unbalanced, but in practical
                 cases this is not the expected, and is the best way to construct the tree (faster way).
              2) Passing a container with pairs (key/data-item) and using the method buildOptimized after
                 calling kdsort. The tree built this way is almost balanced but will be construct in time O(N log N).
                 Warning: In this case items with the same key will be stores in different nodes!

        \note This type of tree may be of special interest of EXTENT SEARCH QUERIES.

        \note If the node type is kd_node_data_single_tag than NodeData and NodeDataItem are the same types.
              And if one entry with the same key already exist, so they will be overwrite.

        \note If the node type is kd_node_data_set_tag than NodeData mus have a method called push_back(NodeDataItem)
              that permits to store elements with the same key in the node.
      */
      template<class KdTreeNode> class Index
      {
        public:

          //! Export key type.
          typedef typename KdTreeNode::kdKey kdKey;

          //! Export data type.
          typedef typename KdTreeNode::kdData kdData;

          //! Export data item type.
          typedef typename KdTreeNode::kdDataItem kdDataItem;

          //! Export data type.
          typedef typename KdTreeNode::kdDataTag kdDataTag;

          /*! \brief Constructor. */
          Index(const te::gm::Envelope& mbr)
            : m_root(0),
              m_mbr(mbr),
              m_size(0)
          {
          }

          /*! \brief Destructor. */
          ~Index()
          {
            clear();
          }

          /*! \brief It clears all tree nodes. */
          void clear()
          {
            if(m_root)
            {
              erase(m_root);
              m_root = 0;
              m_size = 0;
            }
          }

          /*! \brief It returns the number of tree nodes. */
          const std::size_t& size() const
          {
            return m_size;
          }

          /*! \brief It returns true if the tree is empty and false otherwise. */
          bool isEmpty() const
          {
            return m_root == 0;
          }

          /*! \brief It sets the minimum bounding box of all elements in the tree. */
          void setMBR(const te::gm::Envelope& mbr)
          {
            m_mbr = mbr;
          }

          /*! \brief It gets the minimum bounding box of all elements in the tree. */
          const te::gm::Envelope& getMBR() const
          {
            return m_mbr;
          }

          /*! \brief It inserts the data with a given key in tree. */
          inline void insert(const kdKey& key, const kdDataItem& item);

          /*! \brief It inserts the data in the tree and and keeps it balanced: the kdsort algorithm must be called before. */
          void buildOptimized(std::vector<std::pair<kdKey, kdDataItem> >& dataSet)
          {
            const std::size_t last = dataSet.size() - 1;
            m_root = buildOptimized(dataSet, 0, last);
          }

          /*! \brief Range search query. */
          void search(const te::gm::Envelope& e, std::vector<KdTreeNode*>& report) const
          {
            if(m_root)
              search(e, m_root, 'x', report);
          }

        private:

          /*! \brief It Erases a node from the tree and all nodes below it. */
          void erase(KdTreeNode* node)
          {
            if(node->hasLeft())
              erase(node->getLeft());

           if(node->hasRight())
              erase(node->getRight());

            delete node;
          }

          // Not build!

          /*! \brief It inserts data for single nodes, i.e. nodes that stores only one element. */
          /*void insertData(KdTreeNode*& node, const kdDataItem& data, const kd_node_m_datasingle_tag&)
          {
            node->setData(data);
          }*/

          /*! \brief It inserts data for set nodes, i.e., nodes that may stores many element. */
          /*void insertData(KdTreeNode*& node, const kdDataItem& data, const kd_node_data_set_tag&)
          {
            node->getData().push_back(data);
          }*/

          /*! \brief Recursive range query. */
          inline void search(const te::gm::Envelope& e, KdTreeNode* node, const char& level, std::vector<KdTreeNode*>& report) const;

          /*! \brief It builds the tree recursively. */
          KdTreeNode* buildOptimized(std::vector<std::pair<kdKey, kdDataItem> >& dataSet, const std::size_t& first, const std::size_t& last)
          {
            const std::size_t kth = (last - first + 1) / 2;

            KdTreeNode* newNode = new KdTreeNode(dataSet[first + kth].first);

            newNode->setData(dataSet[first + kth].second);

            ++m_size;

            if(first + kth > first)
              newNode->setLeft(buildOptimized(dataSet, first, first + kth - 1));

            if(first + kth < last)
              newNode->setRight(buildOptimized(dataSet, first + kth + 1, last));

            return newNode;
          }

          /*!
            \brief No copy constructor allowed.

            \param rhs The other index.
          */
          Index(const Index& rhs);

          /*!
            \brief No assignment operator allowed.

            \param rhs The other index.

            \return A reference for this.
          */
          Index& operator=(const Index& rhs);

        private:

          KdTreeNode* m_root;     //!< Pointer to the root node.
          te::gm::Envelope m_mbr; //!< Minimum bounding box of all nodes.
          std::size_t m_size;     //!< The size of the K-d Tree (number of nodes).
      };

      template<class KdTreeNode>
      void Index<KdTreeNode>::insert(const kdKey& key, const kdDataItem& item)
      {
        if(m_root == 0)
        {
          m_root = new KdTreeNode(key);
          insertData(m_root, item, kdDataTag());
        }
        else
        {
          char level = 'x';

          bool left = false;

          KdTreeNode* x = m_root;
          KdTreeNode* y = 0;

          while(x != 0)
          {
            y = x;

            if(level == 'x')
            {
              if(key.x() > x->getKey().x()) // if the key is greater than, inserts in the right subtree
              {
                x = x->getRight();
                left = false;
              }
              else if(key.x() < x->getKey().x())  // if the key is smaller than, inserts in the left subtree
              {
                x = x->getLeft();
                left = true;
              }
              else if(key.y() == x->getKey().y()) // if the key already exist, in the case of single node the data will be overwrite and in the case of set node they will push_back the item
              {
                insertData(x, item, kdDataTag());
                return;
              }
              else // found the same axis partition, so go left
              {
                x = x->getLeft();
                left = true;
              }

              level = 'y';
            }
            else
            {
              if(key.y() > x->getKey().y())
              {
                x = x->getRight();
                left = false;
              }
              else if(key.y() < x->getKey().y())
              {
                x = x->getLeft();
                left = true;
              }
              else if(key.x() == x->getKey().x())
              {
                insertData(x, item, kdDataTag());
                return;
              }
              else
              {
                x = x->getLeft();
                left = true;
              } 

              level = 'x';
            }
          }

          KdTreeNode* newNode = new KdTreeNode(key);

          insertData(newNode, item, kdDataTag());

          if(left)
            y->setLeft(newNode);
          else
            y->setRight(newNode);
        }

        ++m_size;
      }

      template<class KdTreeNode>
      void Index<KdTreeNode>::search(const te::gm::Envelope& e, KdTreeNode* node, const char& level, std::vector<KdTreeNode*>& report) const
      {
        if((node->getKey().x() >= e.m_llx) && (node->getKey().x() <= e.m_urx) &&
           (node->getKey().y() >= e.m_lly) && (node->getKey().y() <= e.m_ury))
        {
          report.push_back(node);
        }

        if(level == 'x')
        {
          if(node->hasLeft())
            if(node->getKey().x() >= e.m_llx)
              search(e, node->getLeft(), 'y', report);

          if(node->hasRight())
            if(node->getKey().x() <= e.m_urx)
              search(e, node->getRight(), 'y', report);
        }
        else
        {
          if(node->hasLeft())
            if(node->getKey().y() >= e.m_lly)
              search(e, node->getLeft(), 'x', report);

          if(node->hasRight())
            if(node->getKey().y() <= e.m_ury)
              search(e, node->getRight(), 'x', report);
        }
      }

      /*!
        \class AdaptativeIndex
        
        \brief A class that represents a two dimensional K-d Tree (2-d Tree) that store data-elements into the leafs.

        \note This type of tree stores the data only in the leaf nodes.

        \note The process of construction expect that the tree is almost balanced.

        \note This type of tree may be of special interest of NEAREST NEIGHBOR SEARCH QUERIES.

        \note After a extent search it will be necessary to do a refinement.
      */
      template<class KdTreeNode> class AdaptativeIndex
      {
        public:

          //! Export key type.
          typedef typename KdTreeNode::kdKey kdKey;

          //! Export data type.
          typedef typename KdTreeNode::kdData kdData;

          //! Export data item type.
          typedef typename KdTreeNode::kdDataItem kdDataItem;

          //! Export node type.
          typedef KdTreeNode kdNode;

          /*! \brief Constructor. */
          AdaptativeIndex(const te::gm::Envelope& mbr, const std::size_t& bucketSize = 12)
            : m_root(0),
              m_mbr(mbr),
              m_size(0),
              m_bucketSize(bucketSize)
          {
          }

          /*! \brief Destructor. */
          ~AdaptativeIndex()
          {
            clear();
          }

          /*! \brief It clears all tree nodes. */
          void clear()
          {
            if(m_root)
            {
              erase(m_root);
              m_root = 0;
              m_size = 0;
            }
          }

          /*! \brief It returns the number of tree nodes. */
          const std::size_t& size() const
          {
            return m_size;
          }

          /*! \brief It returns true if the tree is empty and false otherwise. */
          bool isEmpty() const
          {
            return m_root == 0;
          }

          /*! \brief It sets the minimum bounding box of all elements in the tree. */
          void setMBR(const te::gm::Envelope& mbr)
          {
            m_mbr = mbr;
          }

          /*! \brief It gets the minimum bounding box of all elements in the tree. */
          const te::gm::Envelope& getMBR() const
          {
            return m_mbr;
          }

          /*! \brief It sets bucket size for leaf nodes. */
          void setBucketSize(const std::size_t& size)
          {
            m_bucketSize = size;
          }

          /*! \brief It gets bucket size for leaf nodes. */
          const std::size_t& getBucketSize() const
          {
            return m_bucketSize;
          }

          /*! \brief It inserts the data set into the tree. */
          void build(std::vector<std::pair<kdKey, kdDataItem> >& dataSet)
          {
            m_root = build(dataSet, 0.0, m_mbr);
          }

          /*!
            \brief It searches the nearest data in nodes: you must pass an array of kdDataItem of size "k"
                   with coordinates values (X() and Y()) adjusted to td::numeric_limits<double>::max() (this dummy values will be replaced at processing time),
                   and if not all neighbors are found so sqrDists will contains td::numeric_limits<double>::max() in array index.
          */
          void nearestNeighborSearch(const kdKey& key, std::vector<kdDataItem>& report, std::vector<double>& sqrDists, const std::size_t& k) const
          {
            if(m_root)
            {
              sqrDists.clear();

              for(std::size_t i = 0; i < k; ++i)
                sqrDists.push_back(std::numeric_limits<double>::max());

              te::gm::Envelope e(-std::numeric_limits<double>::max(), -std::numeric_limits<double>::max(),
                                 +std::numeric_limits<double>::max(), +std::numeric_limits<double>::max());

              nearestNeighborSearch(root_, key, report, sqrDists, e);
            }
          }

          /*! \brief Range search query. */
          void search(const te::gm::Envelope& e, std::vector<KdTreeNode*>& report) const
          {
            if(m_root)
              search(e, m_root, report);
          }

          /*! \brief Range search query: the refinement is already done. */
          inline void search(const te::gm::Envelope& e, std::vector<kdDataItem>& report) const;

        private:

          /*! \brief It Erases a node from the tree and all nodes below it. */
          void erase(KdTreeNode* node)
          {
            if(node->hasLeft())
              erase(node->getLeft());

           if(node->hasRight())
              erase(node->getRight());

            delete node;
          }

          /*! \brief It builds the tree recursivily. */
          inline KdTreeNode* build(std::vector<std::pair<kdKey, kdDataItem> >& dataSet, double averageValue, const te::gm::Envelope& mbr);

           /*! \brief Recursive nearest neighbor search. */
          inline void nearestNeighborSearch(KdTreeNode* node, const kdKey& key, std::vector<kdDataItem>& report, std::vector<double>& sqrDists, const te::gm::Envelope& e) const;

          /*! \brief Recursive range query. */
          inline void search(const te::gm::Envelope& e, KdTreeNode* node, std::vector<KdTreeNode*>& report) const;

          /*! \brief It updates the neighbor list. */
          inline void update(KdTreeNode* node, const kdKey& key, std::vector<kdDataItem>& report, std::vector<double>& sqrDists, const te::gm::Envelope& e) const;

          /*! \brief It returns the average value along the axis. */
          double average(std::vector<std::pair<kdKey, kdDataItem> >& dataSet, const char& discriminator) const
          {
            const std::size_t size = dataSet.size();

            double medianValue = 0.0;

            if(discriminator == 'x')
            {
              for(unsigned int i = 0; i < size; ++i)
                medianValue += dataSet[i].first.x();

              return medianValue / size;
            }
            else
            {
              for(unsigned int i = 0; i < size; ++i)
                medianValue += dataSet[i].first.y();

              return medianValue / size;
            }
          }

          /*!
            \brief No copy constructor allowed.

            \param rhs The other index.
          */
          AdaptativeIndex(const AdaptativeIndex& rhs);

          /*!
            \brief No assignment operator allowed.

            \param rhs The other index.

            \return A reference for this.
          */
          AdaptativeIndex& operator=(const AdaptativeIndex& rhs);

        private:

          KdTreeNode* m_root;       //!< Pointer to the root node.
          te::gm::Envelope m_mbr;   //!< Minimum bounding box of all nodes.
          std::size_t m_size;       //!< The size of the K-d Tree (number of nodes).
          std::size_t m_bucketSize; //!< Bucket size (maximum number of elements in each node).
      };

      template<class KdTreeNode>
      void AdaptativeIndex<KdTreeNode>::search(const te::gm::Envelope& e, std::vector<kdDataItem>& report) const
      {
        std::vector<KdTreeNode*> reportNodes;

        search(e, reportNodes);

        std::size_t nNodes = reportNodes.size();

        for(std::size_t i = 0; i < nNodes; ++i)
        {
          std::size_t nElements = reportNodes[i]->getData().size();

          for(std::size_t j = 0; j < nElements; ++j)
            //if(TeIntersects((reportNodes[i])->getData()[j], e))
              //report.push_back((reportNodes[i])->getData()[j]);
        }
      }

      template<class KdTreeNode>
      KdTreeNode* AdaptativeIndex<KdTreeNode>::build(std::vector<std::pair<kdKey, kdDataItem> >& dataSet, double averageValue, const te::gm::Envelope& mbr)
      {
        // TODO!
      }

      template<class KdTreeNode>
      void AdaptativeIndex<KdTreeNode>::nearestNeighborSearch(KdTreeNode* node, const kdKey& key, std::vector<kdDataItem>& report, std::vector<double>& sqrDists, const te::gm::Envelope& e) const
      {
        // TODO!
      }

      template<class KdTreeNode>
      void AdaptativeIndex<KdTreeNode>::search(const te::gm::Envelope& e, KdTreeNode* node, std::vector<KdTreeNode*>& report) const
      {
        if(node->getDiscriminator() == 'x')
        {
          if(node->hasLeft())
            if(e.m_llx <= node->getKey())
              search(e, node->getLeft(), report);

          if(node->hasRight())
            if(e.m_urx >= node->getKey())
              search(e, node->getRight(), report);
        }
        else if(node->getDiscriminator() == 'y')
        {
          if(node->hasLeft())
            if(e.m_lly <= node->getKey())
              search(e, node->getLeft(), report);

          if(node->hasRight())
            if(e.m_ury >= node->getKey())
              search(e, node->getRight(), report);
        }
        else
          report.push_back(node);
      }

      template<class KdTreeNode>
      void AdaptativeIndex<KdTreeNode>::update(KdTreeNode* node, const kdKey& key, std::vector<kdDataItem>& report, std::vector<double>& sqrDists, const te::gm::Envelope& e) const
      {
        // TODO!
      }

    } // end namespace kdtree
  }   // end namespace sam
}     // end namespace te

#endif  // __TERRALIB_SAM_KDTREE_INTERNAL_INDEX_H
