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

// --- a keyword-escaped name as the *final* component of a qualified type ----
// The Clang side needed several new parser arms for these; GCC reaches them
// all out of cp_parser_identifier.  Kept in both suites so a divergence in
// either direction is caught by a test rather than by a probe sweep.
namespace `switch` {
  struct `union` { int a; struct S { int b; }; };
  struct `while` : `union` { };
  int `new` = 1;
}
`switch`::`union` q0;
struct `switch`::`union` q1;
using QA = `switch`::`union`;
`switch`::`union`::S q2;
void qparam (`switch`::`union`);
`switch`::`union` qret ();
template<class T> struct QW { };
QW<`switch`::`union`> q3;
struct QD : `switch`::`union` { QD () : `switch`::`union` () { } };
int qblock ()
{
  `switch`::`union` q4;
  return sizeof (`switch`::`union`) + static_cast<`switch`::`union`> (q4).a
	 + `switch`::`new`;
}
template<class T> struct QT { typename T::`union` m; };
QT<`switch`::`union`::S> *qt0;

// --- an escaped class name defined out of line, including its constructor ---
struct `static` { `static` (); void `new` (); };
`static`::`static` () { }
void `static`::`new` () { }

// --- an escape whose keyword is a *type* keyword ----------------------------
// GCC binds `int' and its siblings at global scope so that code which looks
// builtin types up by name can find them; nothing written in C++ can name
// that binding, because `int' is a keyword token, so an escaped declaration
// is not redeclaring anything.  The keyword still names the builtin in the
// same translation unit.
int `int` = 0;
void `long` () { }
using `char` = double;
struct `bool` { int a; };
template<class T> using `float` = T;
enum `short` { SA };
namespace `void` { int x; }
int type_keywords ()
{
  int builtin = 1;			// the keyword still means the type
  long builtin2 = 2;
  `bool` v { 3 };
  `char` d = 1.5;
  `float`<int> f = 4;
  `short` e = SA;
  `long` ();
  return `int` + builtin + (int) builtin2 + v.a + (int) d + f + (int) e
	 + `void`::x;
}
