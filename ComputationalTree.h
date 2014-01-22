#pragma once

#include <map>
#include <list>
#include <iostream>
#include <algorithm>
#include "Operations.h"

class ComputationalTree
{
private:
   struct node
   {
      Expression* key;
      node* left;
      node* right;
   };

   void insertRoot(Expression* expr)
   {
      node* tmp = root;
      root = new node;
      root->key = expr;
      root->left = tmp;
      root->right = 0;
   }

   void insertRightOperChild(node *&n, Expression* expr)
   {
      if (n)
      { 
         if (n->key->getType() == SIGN)
         {
            insertRightOperChild(n->right, expr);
         }
         else
         {
            node* tmp = n;
            insert(n, expr);
            n->left = tmp;
         }
      }
   }

   void insertRightValueChild(node *&n, Expression* expr)
   {
      if (n)
      {
         insertRightValueChild(n->right, expr);
      }
      else
      {
         insert(n, expr);
      }
   }

   void insert(node *&n, Expression* expr)
   {
      n = new node;
      n->key = expr;
      n->left = 0;
      n->right = 0;
   }

   void destroy(node *&n)
   {
      if (n)
      {
         destroy(n->left);
         destroy(n->right);
         delete n;
      }
   }

   void printLeftTraversal(node *&n, std::multimap<int, Expression*>& storage, int level)
   {
      if (n)
      {
         storage.insert(std::make_pair(level, n->key));
         level++;
         printLeftTraversal(n->left, storage, level);
         printLeftTraversal(n->right, storage, level);
      }
   }

   int height(node *&n, int level)
   {
      if (n)
      {
         int leftHeight = height(n->left, level);
         int rightHeight = height(n->right, level);
         return level + std::max(leftHeight, rightHeight) + 1;
      }
      return 0;
   }

   double result(node *&n)
   {
      if (n->left)
      {
         if (n->key->getValue() == OPER_MINUS)
            return result(n->left) - resultRight(n->right);
         else if (n->key->getValue() == OPER_PLUS)
            return result(n->left) + resultRight(n->right);
      }
      return n->key->getValue();
   }
   
   double resultRight(node *&n)
   {
      if (n->right)
      {
         if (n->key->getValue() == OPER_MULTI)
            return n->left->key->getValue() * resultRight(n->right);
         else if (n->key->getValue() == OPER_DIV)
            return n->left->key->getValue() / resultRight(n->right);
      }
      return n->key->getValue();
   }

public:
   ComputationalTree()
   {
      root = 0;
      // Добавляются слева фиктивный ноль и операция сложения. Нужно для выравнивания структуры дерева.
      insert(new Value(0));
      insert(new Operation(OPER_PLUS));
   }

   ~ComputationalTree()
   {
      destroy(root);
   }

   // Общие правила для такого дерева: в самом низу дерева константы, в середине операции высокого приоритета, вверху - низкого.
   // 1. Константы(VALUE) вставляются всегда в самое нижнее правое место отностильно root
   // 2. Операции(SIGN) сложения и вычитания всталяются всегда справа выше root. 
   // 3. Операции умножения и деления вставляются всегда справа ниже root. Причем константа смещается влево, а на ее место помещается знак операции.
   void insert(Expression* expr)
   {
      if (!root)
      {
         insert(root, expr);
      }
      else
      {
         if ( (expr->getType() == SIGN) )
         {
            if ( (expr->getValue() == OPER_MULTI) || (expr->getValue() == OPER_DIV) )
               insertRightOperChild(root->right, expr);
            else if ((expr->getValue() == OPER_PLUS) || (expr->getValue() == OPER_MINUS))
               insertRoot(expr);
         }
         else if ( (expr->getType() == VALUE) )   
         {
            insertRightValueChild(root, expr);
         }
      }
   }

   void printLeftTraversal()
   {
      int level = 0;
      std::multimap<int, Expression*> storage;
      printLeftTraversal(root, storage, level++);
      for (int i = 0; i < height(); i++)
      {
         std::pair<std::multimap<int, Expression*>::iterator, std::multimap<int, Expression*>::iterator> iters = storage.equal_range(i);
         for (std::multimap<int, Expression*>::iterator iter = iters.first; iter != iters.second; ++iter)
         {
            std::cout << iter->second->getValue() << " ";
         }
         std::cout << std::endl;
      }
   }

   int height()
   {
      return height(root, 0);
   }

   double result()
   {
      return result(root);
   }

private:
   node* root;
};
