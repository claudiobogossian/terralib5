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
  \file terralib/sam/kdtree/Node.h

  \brief A class that represents an Kd-tree node.
*/

#ifndef __TERRALIB_SAM_KDTREE_INTERNAL_NODE_H
#define __TERRALIB_SAM_KDTREE_INTERNAL_NODE_H

namespace te
{
  namespace sam
  {
    namespace kdtree
    {
      /*! \brief Kd-tree node type for nodes with single elements (used by template instantiation). */
      struct kd_node_m_datasingle_tag {};

      /*! \brief Kd-Tree node type for nodes with multuple elements (used by template instantiation). */
      struct kd_node_m_dataset_tag {};

      /*!
        \class Node
        
        \brief A class that represents an Kd-tree node.

        Each node contains a pointer to its left and right subtree (NULL if it is not set),
        one key used for insertion of the data into the tree.

        \note The key must have methods called x() and y(). ==> Uba: Vamos colocar esses métodos no te::gm::Coord2D?

        \note These kind of node stores the data in each node.

        \note The nodes may contains one single element (kd_node_m_datasingle_tag) or a set of values (kd_node_m_dataset_tag).

        \note If the node type is kd_node_m_datasingle_tag than NodeData and NodeDataItem are the same types.
              And if one entry with the same key already exist, so they will be overwrite.

        \note If the node type is kd_node_m_dataset_tag than NodeData mus have a method called push_back(NodeDataItem)
              that permits to store elements with the same key in the node.
      */
      template<class NodeKey, class NodeData, class NodeDataItem, class NodeDataTag = kd_node_m_datasingle_tag> class Node
      {
        public:

          //! Export key type.
          typedef NodeKey kdKey;

          //! Export data type.
          typedef NodeData kdData;

          //! Export data item type.
          typedef NodeDataItem kdDataItem;

          //! Export data type.
          typedef NodeDataTag  kdDataTag;

          /*! \brief Constructor. */
          Node(const NodeKey& k)
            : m_key(k),
              m_left(0),
              m_right(0)
          {
          }

          /*! \brief It sets the key to the node. */
          void setKey(const NodeKey& k)
          {
            m_key = k;
          }

          /*! \brief It returns a reference to node key. */
          const NodeKey& getKey() const
          {
            return m_key;
          }

          /*! \brief It sets the data in the node. */
          void setData(const NodeData& data)
          {
            m_data = data;
          }

          /*! \brief It returns a reference to data node. */
          NodeData& getData()
          {
            return m_data;
          }

          /*! \brief It sets the left child pointer. */
          void setLeft(Node* node)
          {
            m_left = node;
          }

          /*! \brief It sets the right child pointer. */
          void setRight(Node* node)
          {
            m_right = node;
          }

          /*! \brief It gets the left child. */
          Node* getLeft() const
          {
            return m_left;
          }

          /*! \brief It gets the right child. */
          Node* getRight() const
          {
            return m_right;
          }

          /*! \brief It checks if this node has a left child. */
          bool hasLeft() const
          {
            return m_left != 0;
          }

          /*! \brief It checks if this node has a right child. */
          bool hasRight() const
          {
            return m_right != 0;
          }

          /*! \brief It returns true if this node is a leaf node. */
          bool isLeaf() const
          {
            return !(hasLeft() || hasRight());
          }

          /*! \brief It counts the number of nodes below this. */
          std::size_t descendants() const
          {
            std::size_t totalLeft  = 0;
            std::size_t totalRight = 0;

            if(hasLeft())
              totalLeft = 1 + getLeft()->descendants();

            if(hasRight())
              totalRight = 1 + getRight()->descendants();

            return totalLeft + totalRight;
          }

        private:

          /*!
            \brief No copy constructor allowed.

            \param rhs The other node.
          */
          Node(const Node& rhs);

          /*!
            \brief No assignment operator allowed.

            \param rhs The other node.

            \return A reference for this.
          */
          Node& operator=(const Node& rhs);

        protected:

          NodeKey m_key;   //!< The key used to access this record.
          NodeData m_data; //!< The data stored in this record.
          Node* m_left;    //!<  Pointer to the left sub-tree.
          Node* m_right;   //!< Pointer to the right sub-tree.
      };

      /*!
        \class AdaptativeNode
        
        \brief A class that represents an Kd-tree node.

        Each node contains a pointer to its left and right subtree (NULL if it is not set),
        a discriminator that indicates the axis of partition, the partition key and a set of data-items.

        \note The key must have methods called x() and y(). ==> Uba: Vamos colocar esses métodos no te::gm::Coord2D?

        \note These kind of node stores the data only in the leafs.

        \note The leaf nodes contains a set of values that forms a bucket (the size is controlled by the tree methods tha use this class).
      */
      template<class NodeKey, class NodeData, class NodeDataItem> class AdaptativeNode
      {
        public:

          //! Export key type.
          typedef NodeKey kdKey;

          //! Export data type.
          typedef NodeData kdData;

          //! Export data item type.
          typedef NodeDataItem kdDataItem;

          /*! \brief Constructor. */
          AdaptativeNode(const double& k)
            : m_key(k),
              m_discriminator('x'),
              m_left(0),
              m_right(0)
          {
          }

          /*! \brief It sets the key to the node. */
          void setKey(const double& k)
          {
            m_key = k;
          }

          /*! \brief It returns a reference to node key. */
          const double& getKey() const
          {
            return m_key;
          }

          /*! \brief It sets the data in the node. */
          void setData(const NodeData& data)
          {
            m_data = data;
          }

          /*! \brief It returns a reference to data node. */
          NodeData& getData()
          {
            return m_data;
          }

          /*! \brief It sets the data in the node. */
          void setDiscriminator(const char& d)
          {
            m_discriminator = d;
          }

          /*! \brief It returns a reference to discriminator. */
          const char& getDiscriminator() const
          {
            return m_discriminator;
          }

          /*! \brief It sets the left child pointer. */
          void setLeft(AdaptativeNode* node)
          {
            m_left = node;
          }

          /*! \brief It sets the right child pointer. */
          void setRight(AdaptativeNode* node)
          {
            m_right = node;
          }

          /*! \brief It gets the left child. */
          AdaptativeNode* getLeft() const
          {
            return m_left;
          }

          /*! \brief It gets the right child. */
          AdaptativeNode* getRight() const
          {
            return m_right;
          }

          /*! \brief It checks if this node has a left child. */
          bool hasLeft() const
          {
            return m_left != 0;
          }

          /*! \brief It checks if this node has a right child. */
          bool hasRight() const
          {
            return m_right != 0;
          }

          /*! \brief It returns true if this node is a leaf node. */
          bool isLeaf() const
          {
            return !(hasLeft() || hasRight());
          }

          /*! \brief It counts the number of nodes below this. */
          std::size_t descendants() const
          {
            std::size_t totalLeft  = 0;
            std::size_t totalRight = 0;

            if(hasLeft())
              totalLeft = 1 + getLeft()->descendants();

            if(hasRight())
              totalRight = 1 + getRight()->descendants();

            return totalLeft + totalRight;
          }

        private:

          /*!
            \brief No copy constructor allowed.

            \param rhs The other node.
          */
          AdaptativeNode(const AdaptativeNode& rhs);

          /*!
            \brief No assignment operator allowed.

            \param rhs The other node.

            \return A reference for this.
          */
          AdaptativeNode& operator=(const AdaptativeNode& rhs);

        protected:

          double m_key;            //!< The key used to access this record.
          NodeData m_data;         //!< The data stored in this record.
          char m_discriminator;    //!< The discriminator used in partition.
          AdaptativeNode* m_left;  //!<  Pointer to the left sub-tree.
          AdaptativeNode* m_right; //!< Pointer to the right sub-tree.
      };

    } // end namespace kdtree
  }   // end namespace sam
}     // end namespace te

#endif  // __TERRALIB_SAM_KDTREE_INTERNAL_NODE_H
