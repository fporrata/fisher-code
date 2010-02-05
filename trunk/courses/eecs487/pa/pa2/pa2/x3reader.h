// X3D parser class. You should be able to complete the 
// assignment without modifying this file.

#ifndef __X3_READER_H
#define __X3_READER_H

#include <istream>
#include <stack>
#include <string>

#include "x3.h"

#ifndef _MSC_VER

#include <ext/hash_map>
#include <ext/hash_set>

namespace __gnu_cxx {
  template<> struct hash< std::string >
  {
    size_t operator()( const std::string& x ) const
    {
      return hash< const char* >()( x.c_str() );
    }
  };
}

#else

#include <hash_map>
#include <hash_set>

#define __gnu_cxx stdext

#endif

class X3Reader {
public:
  enum {NAME_NONE, NAME_USE, NAME_DEF};
public:
  X3Reader();
  X3Scene* Read(std::istream& ist);
  void StartElement(const char *name, const char **atts);
  void EndElement(const char *name);

  int ProcessNames(const char** atts, std::string* name);

  static X3NodeType GetType(const char* name);
private:
  void ResetSkip() {
    skip_level_ = INT_MAX;
  }
private:
  int level_, skip_level_;
  X3Scene* scene_;
  std::stack<X3Node*> node_stack_;
  static __gnu_cxx::hash_map<std::string, X3NodeType> name_hash;
  __gnu_cxx::hash_map<std::string, X3Node*> def_nodes;
};


#endif  // __X3_READER_H

