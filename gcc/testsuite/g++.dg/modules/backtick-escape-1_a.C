// { dg-additional-options "-fmodules-ts -fbacktick" }
// Keyword-escaped names (`kw`) crossing a module boundary.  The escape
// yields an ordinary identifier (design doc §12), so an entity named with
// one must stream through the CMI like any other.  module.cc's
// IDENTIFIER_KEYWORD_P checks are on that path and the GCC track never
// exercised them.

export module backtick_escape;
// { dg-module-cmi backtick_escape }

export int `new` (int a, int b) { return a + b; }

export struct Widget
{
  int `delete` ();
};

int Widget::`delete` () { return 7; }
