/**********************************************

License: BSD
Project Webpage: http://cajun-jsonapi.sourceforge.net/
Author: Terry Caton

***********************************************/

#include "writer.h"
#include <iostream>
#include <iomanip>

/*  

TODO:
* better documentation
* unicode character encoding

*/

namespace json
{


inline void Writer::Write(const UnknownElement& elementRoot, std::wostream& ostr) { Write_i(elementRoot, ostr); }
inline void Writer::Write(const Object& object, std::wostream& ostr)              { Write_i(object, ostr); }
inline void Writer::Write(const Array& array, std::wostream& ostr)                { Write_i(array, ostr); }
inline void Writer::Write(const Number& number, std::wostream& ostr)              { Write_i(number, ostr); }
inline void Writer::Write(const String& string, std::wostream& ostr)              { Write_i(string, ostr); }
inline void Writer::Write(const Boolean& boolean, std::wostream& ostr)            { Write_i(boolean, ostr); }
inline void Writer::Write(const Null& null, std::wostream& ostr)                  { Write_i(null, ostr); }


inline Writer::Writer(std::wostream& ostr) :
   m_ostr(ostr),
   m_nTabDepth(0)
{}

template <typename ElementTypeT>
void Writer::Write_i(const ElementTypeT& element, std::wostream& ostr)
{
   Writer writer(ostr);
   writer.Write_i(element);
   ostr.flush(); // all done
}

inline void Writer::Write_i(const Array& array)
{
   if (array.Empty())
      m_ostr << L"[]";
   else
   {
      m_ostr << L'[' << std::endl;
      ++m_nTabDepth;

      Array::const_iterator it(array.Begin()),
                            itEnd(array.End());
      while (it != itEnd) {
         m_ostr << std::wstring(m_nTabDepth, L'\t');
         
         Write_i(*it);

         if (++it != itEnd)
            m_ostr << L',';
         m_ostr << std::endl;
      }

      --m_nTabDepth;
      m_ostr << std::wstring(m_nTabDepth, L'\t') << L']';
   }
}

inline void Writer::Write_i(const Object& object)
{
   if (object.Empty())
      m_ostr << L"{}";
   else
   {
      m_ostr << L'{' << std::endl;
      ++m_nTabDepth;

      Object::const_iterator it(object.Begin()),
                             itEnd(object.End());
      while (it != itEnd) {
         m_ostr << std::wstring(m_nTabDepth, L'\t') << L'"' << it->name << L"\" : ";
         Write_i(it->element); 

         if (++it != itEnd)
            m_ostr << L',';
         m_ostr << std::endl;
      }

      --m_nTabDepth;
      m_ostr << std::wstring(m_nTabDepth, L'\t') << L'}';
   }
}

inline void Writer::Write_i(const Number& numberElement)
{
   m_ostr << std::setprecision(20) << numberElement.Value();
}

inline void Writer::Write_i(const Boolean& booleanElement)
{
   m_ostr << (booleanElement.Value() ? L"true" : L"false");
}

inline void Writer::Write_i(const String& stringElement)
{
   m_ostr << L'"';

   const std::wstring& s = stringElement.Value();
   std::wstring::const_iterator it(s.begin()),
                               itEnd(s.end());
   for (; it != itEnd; ++it)
   {
      switch (*it)
      {
         case L'"':         m_ostr << L"\\\"";   break;
         case L'\\':        m_ostr << L"\\\\";   break;
         case L'\b':        m_ostr << L"\\b";    break;
         case L'\f':        m_ostr << L"\\f";    break;
         case L'\n':        m_ostr << L"\\n";    break;
         case L'\r':        m_ostr << L"\\r";    break;
         case L'\t':        m_ostr << L"\\t";    break;
         //case L'\u':        m_ostr << L"";    break;  ??
         default:          m_ostr << *it;       break;
      }
   }

   m_ostr << L'"';   
}

inline void Writer::Write_i(const Null& )
{
   m_ostr << L"null";
}

inline void Writer::Write_i(const UnknownElement& unknown)
{
   unknown.Accept(*this); 
}

inline void Writer::Visit(const Array& array)       { Write_i(array); }
inline void Writer::Visit(const Object& object)     { Write_i(object); }
inline void Writer::Visit(const Number& number)     { Write_i(number); }
inline void Writer::Visit(const String& string)     { Write_i(string); }
inline void Writer::Visit(const Boolean& boolean)   { Write_i(boolean); }
inline void Writer::Visit(const Null& null)         { Write_i(null); }



} // End namespace
