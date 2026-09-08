// { dg-do compile }
// { dg-options "-fbacktick -std=c++20" }
// The keyword escape in every name position [lex.name] admits, not only the
// declarator-ids the first implementation reached.  Each position is declared
// and then used, because accepting the declaration is only half of it.

// --- class-head-name, and the type it declares ------------------------------
struct `union` { int x; };
`union` u0;
int read_u0 () { return u0.x; }

// --- constructor, base-specifier and mem-initializer ------------------------
struct `class` { `class` (int); };
struct Derived : `class` { Derived () : `class` (0) { } };

// --- enum-name, scoped and not, and the enumerators -------------------------
enum `namespace` { `new`, `delete` };
`namespace` e0 = `new`;
enum class `struct` { `try` };
`struct` e1 = `struct`::`try`;

// --- namespace-name, qualified use and using-directive ----------------------
namespace `template` { int v; }
int read_v () { return `template`::v; }
using namespace `template`;
int read_v_again () { return v; }

// --- template parameter names, type and template ----------------------------
template<class `typename`> struct Box { `typename` m; };
template<template<class> class `typedef`> struct Meta { `typedef`<int> b; };
Box<int> b0;
Meta<Box> m0;

// --- alias-declaration, alias-template and concept name ---------------------
using `enum` = int;
`enum` a0 = 0;
template<class T> using `friend` = T;
`friend`<int> a1 = 0;
template<class T> concept `explicit` = true;
template<`explicit` T> void constrained (T) { }

// --- a keyword-escaped name as a nested-name-specifier component -------------
namespace `template` { namespace `do` { int w; } }
int deep = `template`::`do`::w;

// --- label ------------------------------------------------------------------
void labelled ()
{
  int n = 0;
  `goto`:
  if (n++ < 1)
    goto `goto`;
}
