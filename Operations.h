#pragma once

enum Operations
{
   OPER_PLUS = 0,
   OPER_MINUS = 1,
   OPER_MULTI = 2,
   OPER_DIV = 3
};

enum LEXEM
{
   VALUE = 0,
   SIGN,
   BRACKET
};

class Expression
{
   public:
      virtual int getType() = 0;
      virtual int getValue() = 0;
      virtual ~Expression() {}
};

class Value : public Expression
{
   public:
      Value(int val) : val(val)
      {

      }

      virtual int getType()
      {
         return VALUE;
      }

      virtual int getValue()
      {
         return val;
      }

      virtual ~Value() {}

   private:
      int val;
};

class Operation : public Expression
{
   public:
      Operation(int oper) : oper(oper)
      {

      }

      virtual int getType()
      {
         return SIGN;
      }

      virtual int getValue()
      {
         return oper;
      }

      virtual ~Operation() {}

   private:
      int oper;
};