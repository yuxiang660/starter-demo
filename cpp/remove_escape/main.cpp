#include <stdexcept>
#include <iostream>
#include <algorithm>

void escapeHandler(std::string &pfxfull, size_t &pos, bool bBracket)
{
   char cdiv = '.';
   std::string esptxt;
   size_t espsz = 0;
   pfxfull = pfxfull.substr(0, pos) + pfxfull.substr(pos + 1);
   size_t bkslash = pfxfull.find_last_of(cdiv, pos - 1);
   bkslash = (bkslash != std::string::npos) ? bkslash + 1 : 0;
   pfxfull.insert(bkslash, "\\");
   pos = pos + 1;
   size_t endp = pos;
   size_t fsz(pfxfull.size());
   if (bBracket)
   {
      while (endp < fsz)
      {
         if (pfxfull[endp] == ']' && pfxfull[endp - 1] == '\\')
         {
            if (endp == fsz - 1 || pfxfull[endp + 1] == '[' || pfxfull[endp + 1] == cdiv)
            {
               esptxt += "] ";
               espsz = esptxt.size();
               if (endp != fsz - 1)
               {
                  esptxt += pfxfull.substr(endp + 1);
               }
               break;
            }
         }
         else if (pfxfull[endp] == cdiv)
         {
            esptxt += " ";
            espsz = esptxt.size();
            esptxt += pfxfull.substr(endp);
            break;
         }
         if (pfxfull[endp] != '\\')
         {
            esptxt.push_back(pfxfull[endp]);
         }
         ++endp;
      }
   }
   else
   {
      while (endp < fsz)
      {
         if (pfxfull[endp] == cdiv)
         {
            esptxt += " ";
            espsz = esptxt.size();
            esptxt += pfxfull.substr(endp);
            break;
         }
         if (pfxfull[endp] != '\\')
         {
            esptxt.push_back(pfxfull[endp]);
         }
         ++endp;
      }
   }
   pfxfull = pfxfull.substr(0, pos) + esptxt;
   if (endp == fsz)
   {
      pfxfull += " ";
      pos = std::string::npos;
   }
   else
   {
      pos = pos + espsz;
   }
   esptxt.clear();
}

// unify different escape name to same format: "\abc "
// * start with one escape character '\'
// * end with one space character ' '
// * no other escape in the string
std::string unifyEscape(const std::string &name)
{
   if (name.find('\\') == std::string::npos)
      return name;

   std::string nameRes;
   nameRes.reserve(name.size());

   auto hasEscape = [&name](size_t start, size_t end)
   {
      for (size_t pos = start; pos < end; pos++)
         if (name[pos] == '\\')
            return true;
      return false;
   };

   auto processEscape = [&name](size_t start, size_t end)
   {
      std::string subName;
      subName.push_back('\\'); // start with escape
      for (size_t pos = start; pos < end; pos++)
      {
         if (name[pos] != '\\') // only save none escape character
            subName.push_back(name[pos]);
      }
      subName.push_back(' '); // end with one space
      return subName;
   };

   size_t pos = 0;
   while (true)
   {
      size_t nextDivPos = name.find_first_of('.', pos);
      if (nextDivPos != std::string::npos)
      {
         if (hasEscape(pos, nextDivPos))
            nameRes += processEscape(pos, nextDivPos);
         else
            nameRes += name.substr(pos, nextDivPos - pos);
         nameRes.push_back('.');
         pos = nextDivPos + 1;
      }
      else
      {
         // last sub-name
         if (hasEscape(pos, name.size()))
            nameRes += processEscape(pos, name.size());
         else
            nameRes += name.substr(pos);
         break; // break because it has handled last sub-name
      }
   }

   return nameRes;
}

int main()
{
   std::cout << "Escape Remover" << std::endl;

   std::string pfxfull = "a.b.c\\\\\\$.d.e.f\\@.g";
   std::cout << "Before: " << pfxfull << std::endl;

   for (auto pos = pfxfull.find("\\"); pos != std::string::npos; pos = pfxfull.find("\\", pos))
   {
      if (pfxfull[pos + 1] == '\\')
      {
         pfxfull = pfxfull.substr(0, pos) + pfxfull.substr(pos + 1);
         continue;
      }
      escapeHandler(pfxfull, pos, false);
   }

   std::cout << "After: " << pfxfull << std::endl;

   std::string pfxfull2 = "a.b.d\\[c\\$\\]\\[x\\*\\].d.e.\\f@ .g";
   std::cout << "Before: " << pfxfull2 << std::endl;
   for (auto pos = pfxfull2.find("\\"); pos != std::string::npos; pos = pfxfull2.find("\\", pos))
   {
      if (pfxfull2[pos + 1] == '\\')
      {
         pfxfull2 = pfxfull2.substr(0, pos) + pfxfull2.substr(pos + 1);
         continue;
      }
      if (pfxfull2[pos + 1] == '[')
      {
         escapeHandler(pfxfull2, pos, true);
      }
      else
      {
         escapeHandler(pfxfull2, pos, false);
      }
   }

   std::cout << "After: " << pfxfull2 << std::endl;

   std::cout << unifyEscape("a.b.c\\\\\\$.d.e.f\\@.g") << std::endl;
   std::cout << unifyEscape("a.b.d\\[c\\$\\]\\[x\\*\\].d.e.\\f@ .g") << std::endl;
}
