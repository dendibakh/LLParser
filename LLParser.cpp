#include "ComputationalTree.h"
#include "Operations.h"
#include <string>

double parse(const std::string &str, std::string::const_iterator& cit) 
{
    ComputationalTree tree;

    for (; cit != str.end(); ++cit) 
    {
        switch (*cit) 
        {
            case '(':
                tree.insert(new Value(parse(str, ++cit)));
                break;
            case ')':
                return tree.result();
                break;
            case '*':
                tree.insert(new Operation(OPER_MULTI));
                break;
            case '/':
                tree.insert(new Operation(OPER_DIV));
                break;
            case '+':
                tree.insert(new Operation(OPER_PLUS));
                break;
            case '-':
                tree.insert(new Operation(OPER_MINUS));
                break;
            case ' ':
                break;
            case '0': case '1': case '2': case '3': case '4': case '5': case '6': case '7': case '8': case '9':
                {
                    int curnum = 0;
                    while (cit != str.end()) 
                    {
                        curnum = 10 * curnum + (*cit - '0');
                        if ((cit + 1) == str.end() || !isdigit(*(cit+1))) {
                            break;
                        }
                        ++cit;
                    }
                    tree.insert(new Value(curnum));
                }
                break;
            default:
                throw std::domain_error("unexpected symbol");
        }
    }

    //tree.printLeftTraversal();
    return tree.result();
}

double parse(const std::string &str)
{
   std::string::const_iterator cit = str.begin();
   return parse(str, cit);
}

int main()
{
   std::string line;
   std::getline(std::cin, line);
   if (line.length() > 0) 
   {
      try 
      {
         std::cout << "result is: " << parse(line) << std::endl;
         system("pause");
      } 
      catch (std::exception &e) 
      {
         std::cout << "error: " << e.what() << std::endl;
      }
   }
   // Время алгоритма линейное O(N).
   // Пример 1: 25*10+13-35/7*5*2+10
   // Пример 2: 25*(10+15)
   // Пример 3: 25*(10+15*(5-2))
}