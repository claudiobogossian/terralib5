/*  Copyright (C) 2008 National Institute For Space Research (INPE) - Brazil.

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
  \file Index.h

  \brief An implementation of R-tree data structure for main memory.
*/

#ifndef __TERRALIB_SAM_RTREE_INTERNAL_INDEX_H
#define __TERRALIB_SAM_RTREE_INTERNAL_INDEX_H

// TerraLib
#include "Node.h"
#include "PartitionVars.h"

// STL
#include <cassert>
#include <vector>

namespace te
{
  namespace sam
  {
    namespace rtree
    {
      /*!
        \class Index
        
        \brief A class that represents an R-tree.

        This implementation is based on: 

        <i>Antonin Guttman. R-Trees: A Dynamic Index Structure for Spatial Searching. ACM SIGMOD: International Conference on Management of Data, 1984, pp. 47-57</i>.
        
        and in his original source code.

        \todo Implement nearest neighbour query.
      */
      template<class DATATYPE, int MAXNODES = 8, int MINNODES = MAXNODES / 2> class Index
      {
        public:

          typedef Node<DATATYPE, MAXNODES, MINNODES> NodeType;
          typedef typename NodeType::BranchType BranchType;
          typedef typename te::sam::rtree::PartitionVars<BranchType, MAXNODES> PartitionVarsType;

          /*! \brief Constructor. */
          Index();

          /*!
            \brief Constructor.

            \param mbr The MBR of all elements in the R-tree.

            \pre The given MBR must be valid.
          */
          Index(const te::gm::Envelope& mbr);

          /*! \brief Destructor. */
          ~Index();

          /*!
            \brief It returns the number of elements in the tree.

            \return The number of indexed elements.
          */
          std::size_t size(void) const;
          
          /*!
            \brief It returns true if the tree is empty.

            \return True if the tree is empty.
          */
          bool isEmpty(void) const;

          /*! It clears all tree nodes and make it ready for new insertions. */
          void clear(void);

          /*!
            \brief It inserts an item into the tree.

            \param mbr  The object MBR.
            \param data The object to be indexed.
          */
          void insert(const te::gm::Envelope& mbr, const DATATYPE& data);

          /*!
            \brief It removes an item from the tree.

            \param mbr  The object MBR.
            \param data The object to be removed from the tree.

            \return True if the object is found and removed, false otherwise.
          */
          bool remove(const te::gm::Envelope& mbr, const DATATYPE& data);

          /*!
            \brief Range search query.

            \param mbr    The search rectangle.
            \param report A vector to output the founded objects.

            \return the number of found objects.
          */
          int search(const te::gm::Envelope& mbr, std::vector<DATATYPE>& report) const;

          /*!
            \brief It sets the bounding box of all elements in the tree.

            \param mbr The bounding box of all elements in the tree.
          */
          void setMBR(const te::gm::Envelope& mbr);

          /*!
            \brief It returns the bounding box of all elements in the tree.

            \return The bounding box of all elements in the tree.
          */
          const te::gm::Envelope& getMBR(void) const;

        protected:

          /*!
            \brief It inserts a data rectangle into an index structure.

            \return It returns true if root was split, false if it was not.
          */
          bool insert(const te::gm::Envelope& mbr, const DATATYPE& data, NodeType** root, int level);

          /*!
            \brief It deletes a data rectangle from an index structure.       

            \return It returns 1 if record not found, 0 if success.
          */
          bool remove(const te::gm::Envelope& mbr, const DATATYPE& data, NodeType** root);

          /*!
            \brief It deletes a rectangle from non-root part of an index structure.

            \return 
          */
          bool remove2(const te::gm::Envelope& mbr, const DATATYPE& data, NodeType* n, std::vector<NodeType*>& ee);

          /*!
            \brief It disconnects a dependent node.
          */
          void disconBranch(NodeType* n, int i);

          /*!
            \brief It inserts a new data rectangle into the index structure.
          */
          bool chooseLeaf(const te::gm::Envelope& mbr,
                          const DATATYPE& data,
                          NodeType* node,
                          NodeType** newNode,
                          int level);

          /*!
            \brief Recursive range query.
          */
          void search(const te::gm::Envelope& mbr,
                      NodeType* node,
                      std::vector<DATATYPE>& report,
                      int& foundObjs) const;

          /*!
            \brief Find the smallest rectangle that includes all rectangles in branches of a node.
          */
          te::gm::Envelope nodeCover(NodeType* n) const;

          /*!
            \brief Combine two rectangles into larger one containing both.
          */
          te::gm::Envelope combineRect(const te::gm::Envelope& mbrA,
                                       const te::gm::Envelope& mbrB) const;

          /*!
            \brief Add a branch to a node.
          */
          bool addBranch(BranchType* branch,
                         NodeType* node,
                         NodeType** newNode) const;

          /*!
            \brief Pick a branch.
          */
          int pickBranch(const te::gm::Envelope& mbr, NodeType* node) const;

          /*!
            \brief Split a node.
          */
          void splitNode(NodeType* node, BranchType* branch, NodeType** newNode) const;

          void getBranches(NodeType* n, BranchType* b, PartitionVarsType& p) const;

          void methodZero(PartitionVarsType& p) const;

          void pickSeeds(PartitionVarsType& p) const;

          void classify(int i, int group, PartitionVarsType& p) const;

          void pigeonhole(PartitionVarsType& p) const;

          void loadNodes(NodeType* n, NodeType* q, PartitionVarsType& p) const;

          /*!
            \brief Erases a node from the tree and all nodes below it.

            \param node The node to be removed.

            \pos The node pointer will be invalidated.
          */
          void erase(NodeType* node);


        private:


          /*!
            \brief No copy constructor allowed.

            \param rhs The other rtree.
          */
          Index(const Index& rhs);

          /*!
            \brief No assignment operator allowed.

            \param rhs The other rtree.

            \return A reference for this instance.
          */
          Index& operator=(const Index& rhs);


        private:

          NodeType* m_root;             //!< Pointer to the root node.
          te::gm::Envelope m_mbr;       //!< Bounding box of the tree.
          mutable std::size_t m_size;   //!< The size of R-Tree (number of nodes).
      };

      template<class DATATYPE, int MAXNODES, int MINNODES> inline
      Index<DATATYPE, MAXNODES, MINNODES>::Index()
        : m_root(0), m_size(0)
      {
        ++m_size;
        m_root = new NodeType();
              m_root->m_level = 0;
      }

      template<class DATATYPE, int MAXNODES, int MINNODES> inline
      Index<DATATYPE, MAXNODES, MINNODES>::Index(const te::gm::Envelope& mbr)
        : m_root(0), m_mbr(mbr), m_size(0)
      {
        assert(m_mbr.isValid());
        ++m_size;
        m_root = new NodeType();
              m_root->m_level = 0;
      }

      template<class DATATYPE, int MAXNODES, int MINNODES> inline
      Index<DATATYPE, MAXNODES, MINNODES>::~Index()
      {
        clear();
        delete m_root;
      }

      template<class DATATYPE, int MAXNODES, int MINNODES> inline
      std::size_t Index<DATATYPE, MAXNODES, MINNODES>::size(void) const
      {
        return m_size;
      }

      template<class DATATYPE, int MAXNODES, int MINNODES> inline
      bool Index<DATATYPE, MAXNODES, MINNODES>::isEmpty(void) const
      {
        return (m_root->m_count == 0);
      }

      template<class DATATYPE, int MAXNODES, int MINNODES> inline
      void Index<DATATYPE, MAXNODES, MINNODES>::clear(void)
      {
        if(m_root)
        {
          erase(m_root);
          m_root = 0;
          
          m_size = 1;
          m_root = new NodeType();
          m_root->m_level = 0;
        }
      }

      template<class DATATYPE, int MAXNODES, int MINNODES> inline
      void Index<DATATYPE, MAXNODES, MINNODES>::insert(const te::gm::Envelope& mbr, const DATATYPE& data)
      {
        insert(mbr, data, &m_root, 0);
        m_mbr.Union(mbr);
      }

      template<class DATATYPE, int MAXNODES, int MINNODES> inline
      bool Index<DATATYPE, MAXNODES, MINNODES>::remove(const te::gm::Envelope& mbr, const DATATYPE& data)
      {
        return remove(mbr, data, &m_root);
      }

      template<class DATATYPE, int MAXNODES, int MINNODES> inline
      int Index<DATATYPE, MAXNODES, MINNODES>::search(const te::gm::Envelope& mbr, std::vector<DATATYPE>& report) const
      {
        int foundObjs = 0;

        if(m_root)
          search(mbr, m_root, report, foundObjs);

        return foundObjs;
      }

      template<class DATATYPE, int MAXNODES, int MINNODES> inline
      void Index<DATATYPE, MAXNODES, MINNODES>::setMBR(const te::gm::Envelope& mbr)
      {
        m_mbr = mbr;
      }

      template<class DATATYPE, int MAXNODES, int MINNODES> inline
      const te::gm::Envelope& Index<DATATYPE, MAXNODES, MINNODES>::getMBR(void) const
      {
        return m_mbr;
      }

      template<class DATATYPE, int MAXNODES, int MINNODES> inline
      bool Index<DATATYPE, MAXNODES, MINNODES>::insert(const te::gm::Envelope& mbr, const DATATYPE& data, NodeType** root, int level)
      {
// this is the algorithm insert
        NodeType* newRoot;
        NodeType* newNode;
        BranchType branch;

        if(chooseLeaf(mbr, data, *root, &newNode, level))  // I1
        {
// I4
// if root was split
// grow a new root, make tree taller
          ++m_size;
          newRoot = new NodeType();  

          newRoot->m_level = (*root)->m_level + 1;

// first half node
          branch.m_mbr  = nodeCover(*root);
          branch.m_child = *root;
          addBranch(&branch, newRoot, 0);

// second half node
          branch.m_mbr = nodeCover(newNode);
          branch.m_child = newNode;        
          addBranch(&branch, newRoot, 0);

          *root = newRoot;

          return true;
        }

        return false;
      }

      template<class DATATYPE, int MAXNODES, int MINNODES> inline
      bool Index<DATATYPE, MAXNODES, MINNODES>::remove(const te::gm::Envelope& mbr, const DATATYPE& data, NodeType** root)
      {
        int i;
        NodeType *t;
        std::vector<NodeType*> reInsertList;
        
        if(remove2(mbr, data, *root, reInsertList))
        {
// if we are here, so we have found and deleted a data item

// reinsert any branches from eliminated nodes
          while(!reInsertList.empty())
          {
            t = reInsertList[0];

            for(i = 0; i < t->m_count; ++i)
              insert(t->m_branch[i].m_mbr, t->m_branch[i].m_data, root, t->m_level);
        
// erase node from list
            reInsertList.erase(reInsertList.begin());

// remove node card from memory
            delete t;
            --m_size;
          }
          
// check for redundant root (not leaf, 1 child) and eliminate
          if(((*root)->m_count == 1) && ((*root)->isInternalNode()))
          {
            t = (*root)->m_branch[0].m_child;

            delete (*root);
            --m_size;

            *root = t;
          }
          
          return true;
        }
        
        return false;
      }

      template<class DATATYPE, int MAXNODES, int MINNODES> inline
      bool Index<DATATYPE, MAXNODES, MINNODES>::remove2(const te::gm::Envelope& mbr, const DATATYPE& data, NodeType* n, std::vector<NodeType*>& ee)
      {
        int i;  

        if(n->isInternalNode()) /* not a leaf node */
        {
          for(i = 0; i < n->m_count; ++i)
          {
            if(mbr.intersects(n->m_branch[i].m_mbr))
            {
              if(remove2(mbr, data, n->m_branch[i].m_child, ee))
              {
                if(n->m_branch[i].m_child->m_count >= MINNODES)
                  n->m_branch[i].m_mbr = nodeCover(n->m_branch[i].m_child);
                else
                {
                  /* not enough entries in child, eliminate child node */
                  ee.push_back(n->m_branch[i].m_child);  //reInsert(n->m_branch[i].m_child, ee);
                  disconBranch(n, i);
                }

                return true;  // found item
              }
            }
          }

          return false;  // din't find item
        }
        else  /* a leaf node */
        {
          for(i = 0; i < n->m_count; ++i)
          {
            if(n->m_branch[i].m_data == data)
            {
              disconBranch(n, i);
              return true;  // found item
            }
          }

          return false;  // didn't find item
        }
      }

      template<class DATATYPE, int MAXNODES, int MINNODES>
      void Index<DATATYPE, MAXNODES, MINNODES>::disconBranch(NodeType* n, int i)
      {
// remove element copying the last element in array
        n->m_branch[i] = n->m_branch[n->m_count - 1];

        --(n->m_count);
      }

      template<class DATATYPE, int MAXNODES, int MINNODES>
      bool Index<DATATYPE, MAXNODES, MINNODES>::chooseLeaf(const te::gm::Envelope& mbr, const DATATYPE& data, NodeType* node, NodeType** newNode, int level)
      {
        BranchType    b;
        NodeType* n2;  
        
        if(node->m_level > level)
        {
// Still above level for insertion, go down tree recursively
          int i = pickBranch(mbr, node);    // CL3

          if(!chooseLeaf(mbr, data, node->m_branch[i].m_child, &n2, level))
          {
// child was not split
            node->m_branch[i].m_mbr = combineRect(mbr, node->m_branch[i].m_mbr);

            return false;
          }
          else    
          {
// child was split
            node->m_branch[i].m_mbr = nodeCover(node->m_branch[i].m_child);

            b.m_child = n2;
            b.m_mbr = nodeCover(n2);

            return addBranch(&b, node, newNode);
          }
        }
        else if (node->m_level == level)
        {
// have reached level for insertion. Add mbr, split if necessary

          b.m_mbr = mbr;
          //b.m_child = (NodeType*) data;
          b.m_data = data;

// child field of leaves contains tid of data record
          return addBranch(&b, node, newNode);
        }
        else
        {
// Not supposed to happen
          //throw;
          return false;
        }
      }

      template<class DATATYPE, int MAXNODES, int MINNODES>
      void Index<DATATYPE, MAXNODES, MINNODES>::search(const te::gm::Envelope& mbr, NodeType* node, std::vector<DATATYPE>& report, int& foundObjs) const
      {
        int i;
// S1
          if(node->isInternalNode()) // This is an internal node in the tree
        {
              for(i = 0; i < node->m_count; ++i)
          {
                  if(mbr.intersects(node->m_branch[i].m_mbr))
                      search(mbr, node->m_branch[i].m_child, report, foundObjs);
          }
        }
// S2
        else  // This is a leaf node
        {
              for(i = 0; i < node->m_count; ++i)
          {
                  if(mbr.intersects(node->m_branch[i].m_mbr))
            {
                      DATATYPE& id = node->m_branch[i].m_data;

              report.push_back(id);

                      ++foundObjs;
            }
          }
        }

        return;
      }

      template<class DATATYPE, int MAXNODES, int MINNODES>
      te::gm::Envelope Index<DATATYPE, MAXNODES, MINNODES>::nodeCover(NodeType* n) const
      {
        bool flag = true;

        te::gm::Envelope r;
        
        for(int i = 0; i < n->m_count; ++i)
        {
              if(flag)
          {
            r = n->m_branch[i].m_mbr;
            flag = false;
          }
          else
            r = combineRect(r, n->m_branch[i].m_mbr);
        }

        return r;
      }

      template<class DATATYPE, int MAXNODES, int MINNODES> inline
      te::gm::Envelope Index<DATATYPE, MAXNODES, MINNODES>::combineRect(const te::gm::Envelope& mbrA, const te::gm::Envelope& mbrB) const
      {
        te::gm::Envelope newRect(mbrA);
        newRect.Union(mbrB);
        return newRect;
      }

      template<class DATATYPE, int MAXNODES, int MINNODES> inline
      bool Index<DATATYPE, MAXNODES, MINNODES>::addBranch(BranchType* branch, NodeType* node, NodeType** newNode) const
      {
        if(node->m_count < MAXNODES)    /* split won't be necessary */
        {
          node->m_branch[node->m_count] = *branch;
          ++(node->m_count);
          return false;
        }
        else
        {
          splitNode(node, branch, newNode);
          return true;
        }
      }

      template<class DATATYPE, int MAXNODES, int MINNODES> inline
      int Index<DATATYPE, MAXNODES, MINNODES>::pickBranch(const te::gm::Envelope& mbr, NodeType* node) const
      {
// CL3
        bool flag = true;
        double bestIncr = -1.0;
        double bestArea = 0.;
        int best = 0;
        
        for(int i = 0; i < node->m_count; ++i)
        {
          te::gm::Envelope rr = node->m_branch[i].m_mbr;
          double area = rr.getArea();

          rr = combineRect(mbr, rr);
              
          double increase = rr.getArea() - area;

          if((increase <  bestIncr) || flag)
          {
            best = i;
            bestArea = area;
            bestIncr = increase;
            flag = false;
          }
          else if((increase == bestIncr) && (area < bestArea))
          {
            best = i;
            bestArea = area;
            bestIncr = increase;
          }
        }

        return best;
      }

      template<class DATATYPE, int MAXNODES, int MINNODES> inline
      void Index<DATATYPE, MAXNODES, MINNODES>::splitNode(NodeType* node, BranchType* branch, NodeType** newNode) const
      {
        PartitionVarsType partitions;

// load all the branches into a buffer, initialize old node
        int level = node->m_level;

        getBranches(node, branch, partitions);

// find partition
        methodZero(partitions);

// put branches from buffer into 2 nodes according to chosen partition
        ++m_size;

        *newNode = new NodeType();
        (*newNode)->m_level = node->m_level = level;

        loadNodes(node, *newNode, partitions);
      }

      template<class DATATYPE, int MAXNODES, int MINNODES> inline
      void Index<DATATYPE, MAXNODES, MINNODES>::getBranches(NodeType* n, BranchType* b, PartitionVarsType& p) const
      {
// load the branch buffer
        for(int i = 0; i < MAXNODES; ++i)
        {
          p.m_branchBuf[i] = n->m_branch[i];
        }

        p.m_branchBuf[MAXNODES] = *b;

// calculate mbr containing all in the set
        p.m_coverSplit = p.m_branchBuf[0].m_mbr;

        for(int i = 1; i <= MAXNODES; ++i)
          p.m_coverSplit = combineRect(p.m_coverSplit, p.m_branchBuf[i].m_mbr);

        n->init();
      }

      template<class DATATYPE, int MAXNODES, int MINNODES> inline
      void Index<DATATYPE, MAXNODES, MINNODES>::methodZero(PartitionVarsType& p) const
      {
        p.init();
        pickSeeds(p);
        pigeonhole(p);
      }

      template<class DATATYPE, int MAXNODES, int MINNODES> inline
      void Index<DATATYPE, MAXNODES, MINNODES>::pickSeeds(PartitionVarsType& p) const
      {
        double w;
        double separation;
        double bestSep;
        double width[2];
        int leastUpper[2];
        int greatestLower[2];
        int seed0;
        int seed1;
        int i;
// LPS1
// find the rectangles farthest out in each dimbrion along dimens "x"
        greatestLower[0] = leastUpper[0] = 0;

        for(i = 1; i <= MAXNODES; ++i)
        {
          te::gm::Envelope& r = p.m_branchBuf[i].m_mbr;

          if(r.m_llx > p.m_branchBuf[greatestLower[0]].m_mbr.m_llx)
            greatestLower[0] = i;

          if(r.m_urx < p.m_branchBuf[leastUpper[0]].m_mbr.m_urx)
            leastUpper[0] = i;
        }

// LPS2
// LPS3
// find the width of the whole collection along this dimension
        width[0] = p.m_coverSplit.m_urx - p.m_coverSplit.m_llx;

// find the rectangles farthest out in each dimbrion along dimens "y"
        greatestLower[1] = leastUpper[1] = 0;

        for(i = 1; i <= MAXNODES; ++i)
        {
          te::gm::Envelope& r = p.m_branchBuf[i].m_mbr;

          if(r.m_lly > p.m_branchBuf[greatestLower[1]].m_mbr.m_lly)
            greatestLower[1] = i;

          if(r.m_ury < p.m_branchBuf[leastUpper[1]].m_mbr.m_ury)
            leastUpper[1] = i;
        }

// LPS2
// LPS3
// find the width of the whole collection along this dimension
        width[1] = p.m_coverSplit.m_ury - p.m_coverSplit.m_lly;


// pick the best separation dimension and the two seed mbrs

// divisor for normalizing by width

// x
        if(width[0] == 0.0)
          w = 1.0;
        else
          w = width[0];

        te::gm::Envelope rlow  = p.m_branchBuf[leastUpper[0]].m_mbr;
        te::gm::Envelope rhigh = p.m_branchBuf[greatestLower[0]].m_mbr;

        seed0 = leastUpper[0];
        seed1 = greatestLower[0];
          
        bestSep = (rhigh.m_llx - rlow.m_urx) / w;

// y
        if(width[1] == 0.0)
          w = 1.0;
        else
          w = width[1];

        rlow  = p.m_branchBuf[leastUpper[1]].m_mbr;
        rhigh = p.m_branchBuf[greatestLower[1]].m_mbr;

        separation = (rhigh.m_lly - rlow.m_ury) / w;

// LPS3
        if(separation > bestSep)
        {
          seed0 = leastUpper[1];
          seed1 = greatestLower[1];

          bestSep = separation;
        }

        if(seed0 != seed1)
        {
          classify(seed0, 0, p);
          classify(seed1, 1, p);
        }
      }

      template<class DATATYPE, int MAXNODES, int MINNODES> inline
      void Index<DATATYPE, MAXNODES, MINNODES>::classify(int i, int group, PartitionVarsType& p) const
      {
        p.m_partition[i] = group;
        p.m_taken[i] = 1;

        if(p.m_count[group] == 0)
          p.m_cover[group] = p.m_branchBuf[i].m_mbr;
        else
          p.m_cover[group] = combineRect(p.m_branchBuf[i].m_mbr, p.m_cover[group]);

        p.m_area[group] = p.m_cover[group].getArea();

        ++(p.m_count[group]);
      }

      template<class DATATYPE, int MAXNODES, int MINNODES> inline
      void Index<DATATYPE, MAXNODES, MINNODES>::pigeonhole(PartitionVarsType& p) const
      {
        te::gm::Envelope newCover[2];
        
        double newArea[2];
        double increase[2];

        for(int i = 0; i <= MAXNODES; ++i)
        {
          if(p.m_taken[i] == 0)
          {
// if one group too full, put mbr in the other regardless
            if(p.m_count[0] >= (MAXNODES + 1 - MINNODES))
            {
              classify(i, 1, p);
              continue;
            }
            else if(p.m_count[1] >= (MAXNODES + 1 - MINNODES))
            {
              classify(i, 0, p);
              continue;
            }

// find the areas of the two groups' old and new covers
            for(int group = 0; group < 2; ++group)
            {
              if(p.m_count[group] > 0)
              {
                newCover[group] = combineRect(p.m_branchBuf[i].m_mbr, p.m_cover[group]);
              }
              else
              {
                newCover[group] = p.m_branchBuf[i].m_mbr;
              }

              newArea[group] = newCover[group].getArea();
              increase[group] = newArea[group] - p.m_area[group];
            }

// put mbr in group whose cover will need to expand less
            if(increase[0] < increase[1])
              classify(i, 0, p);
            else if(increase[1] < increase[0])
              classify(i, 1, p);
// put mbr in group that will have a smaller area cover
            else if(p.m_area[0] < p.m_area[1])
              classify(i, 0, p);
            else if(p.m_area[1] < p.m_area[0])
              classify(i, 1, p);
// put mbr in group with fewer elements
            else if(p.m_count[0] < p.m_count[1])
              classify(i, 0, p);
            else
              classify(i, 1, p);
          }
        }
      }

      template<class DATATYPE, int MAXNODES, int MINNODES> inline
      void Index<DATATYPE, MAXNODES, MINNODES>::loadNodes(NodeType* n, NodeType* q, PartitionVarsType& p) const
      {
        for(int i = 0; i <= MAXNODES; ++i)
        {
          if(p.m_partition[i] == 0)
            addBranch(&(p.m_branchBuf[i]), n, 0);
          else if(p.m_partition[i] == 1)
            addBranch(&(p.m_branchBuf[i]), q, 0);
          //else
          //  throw;  // ERROR
        }
      }

      template<class DATATYPE, int MAXNODES, int MINNODES> inline
      void Index<DATATYPE, MAXNODES, MINNODES>::erase(NodeType* node)
      {
        if(node->isLeaf())
        {
          delete node;
          return;
        }

        for(int i = 0u; i < node->m_count; ++i)
          erase(node->m_branch[i].m_child);
    
        delete node;
        return;
      }

    } // end namespace rtree
  }    // end namespace sam
}      // end namespace te


#endif  // __TERRALIB_SAM_RTREE_INTERNAL_INDEX_H

