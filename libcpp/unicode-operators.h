/* Unicode user-defined operator code points for -funicode-operators.
   Copyright (C) 2026 Free Software Foundation, Inc.

This program is free software; you can redistribute it and/or modify it
under the terms of the GNU General Public License as published by the
Free Software Foundation; either version 3, or (at your option) any
later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; see the file COPYING3.  If not see
<http://www.gnu.org/licenses/>.  */

/* GENERATED TABLES -- the same derivation as Clang's
   clang/lib/Lex/UnicodeOperatorCharSets.h, from the five UCD 17.0.0 data
   files (PropList.txt, DerivedAge.txt, UnicodeData.txt,
   DerivedCoreProperties.txt, emoji/emoji-data.txt), by the backtick
   repository's docs/pattern-syntax-audit.py.  UCD version 17.0.0,
   DerivedAge.txt dated 2025-07-30.

   Derivation (unicode-operators.md section 5, predicates 1-5): a code
   point is a user-operator token iff it has Pattern_Syntax, is outside
   ASCII, lies in the mathematical and arrow blocks (U+2190-21FF,
   U+2200-22FF, U+2300-23FF, U+27C0-27EF, U+27F0-27FF, U+2900-297F,
   U+2980-29FF, U+2A00-2AFF, U+2B00-2BFF), has General_Category Sm or So,
   and is not one of the named exclusions below.  The predicates are
   derivation inputs applied once, offline: the enumeration is frozen at
   UCD 17.0.0 by fiat, so these tables do not track later UCD versions.

   Result: 1381 code points in 32 contiguous ranges.  */

#ifndef LIBCPP_UNICODE_OPERATORS_H
#define LIBCPP_UNICODE_OPERATORS_H

/* The frozen user-operator set: sorted, non-overlapping, inclusive
   ranges.  */
struct user_operator_range
{
  cppchar_t first;
  cppchar_t last;
};

static const struct user_operator_range user_operator_ranges[] = {
  { 0x2190, 0x21CF }, { 0x21D1, 0x21D1 }, { 0x21D3, 0x21D3 },
  { 0x21D5, 0x2201 }, { 0x2203, 0x2206 }, { 0x2208, 0x2211 },
  { 0x2213, 0x2214 }, { 0x2216, 0x2216 }, { 0x2218, 0x2218 },
  { 0x221A, 0x221D }, { 0x221F, 0x2222 }, { 0x2224, 0x2235 },
  { 0x2237, 0x2263 }, { 0x2266, 0x22C4 }, { 0x22C6, 0x2307 },
  { 0x230C, 0x2319 }, { 0x231C, 0x2328 }, { 0x232B, 0x23E8 },
  { 0x23ED, 0x23EF }, { 0x23F1, 0x23F2 }, { 0x23F4, 0x23FF },
  { 0x27C0, 0x27C4 }, { 0x27C7, 0x27E5 }, { 0x27F0, 0x27FF },
  { 0x2900, 0x2982 }, { 0x2999, 0x29D7 }, { 0x29DC, 0x29FB },
  { 0x29FE, 0x2B1A }, { 0x2B1D, 0x2B4F }, { 0x2B51, 0x2B54 },
  { 0x2B56, 0x2B73 }, { 0x2B76, 0x2BFF },
};

/* Why a code point a reader might expect to be a user operator is not one.
   The exclusions exist for the reader's protection, so the lexer says which
   class a code point was excluded under rather than giving the generic
   stray-character error.  CONFUSABLE carries the ASCII token the code point
   apes; it is a curated editorial judgement (the token a reader is most
   likely to mistake the character for), not a derivation from UTS #39.  */
struct user_operator_exclusion
{
  cppchar_t code_point;
  enum cpp_user_operator_exclusion reason;
  const char *confusable;
};

/* The named exclusions of section 5 predicate 5 (16) plus the 12
   emoji-presentation code points inside the blocks.  Sorted by code
   point.  */
static const struct user_operator_exclusion user_operator_exclusions[] = {
  /* ⁄ U+2044 FRACTION SLASH */
  { 0x2044, CPP_UOP_CONFUSABLE, "/" },
  /* ⇐ U+21D0 LEFTWARDS DOUBLE ARROW */
  { 0x21D0, CPP_UOP_CONFUSABLE, "<=" },
  /* ⇒ U+21D2 RIGHTWARDS DOUBLE ARROW */
  { 0x21D2, CPP_UOP_CONFUSABLE, "=>" },
  /* ⇔ U+21D4 LEFT RIGHT DOUBLE ARROW */
  { 0x21D4, CPP_UOP_CONFUSABLE, "<=>" },
  /* ∂ U+2202 PARTIAL DIFFERENTIAL */
  { 0x2202, CPP_UOP_IDENTIFIER_PROFILE, NULL },
  /* ∇ U+2207 NABLA */
  { 0x2207, CPP_UOP_IDENTIFIER_PROFILE, NULL },
  /* − U+2212 MINUS SIGN */
  { 0x2212, CPP_UOP_CONFUSABLE, "-" },
  /* ∕ U+2215 DIVISION SLASH */
  { 0x2215, CPP_UOP_CONFUSABLE, "/" },
  /* ∗ U+2217 ASTERISK OPERATOR */
  { 0x2217, CPP_UOP_CONFUSABLE, "*" },
  /* ∙ U+2219 BULLET OPERATOR */
  { 0x2219, CPP_UOP_CONFUSABLE, "." },
  /* ∞ U+221E INFINITY */
  { 0x221E, CPP_UOP_IDENTIFIER_PROFILE, NULL },
  /* ∣ U+2223 DIVIDES */
  { 0x2223, CPP_UOP_CONFUSABLE, "|" },
  /* ∶ U+2236 RATIO */
  { 0x2236, CPP_UOP_CONFUSABLE, ":" },
  /* ≤ U+2264 LESS-THAN OR EQUAL TO */
  { 0x2264, CPP_UOP_CONFUSABLE, "<=" },
  /* ≥ U+2265 GREATER-THAN OR EQUAL TO */
  { 0x2265, CPP_UOP_CONFUSABLE, ">=" },
  /* ⋅ U+22C5 DOT OPERATOR */
  { 0x22C5, CPP_UOP_CONFUSABLE, "." },
  /* ⌚ U+231A WATCH */
  { 0x231A, CPP_UOP_EMOJI, NULL },
  /* ⌛ U+231B HOURGLASS */
  { 0x231B, CPP_UOP_EMOJI, NULL },
  /* ⏩ U+23E9 BLACK RIGHT-POINTING DOUBLE TRIANGLE */
  { 0x23E9, CPP_UOP_EMOJI, NULL },
  /* ⏪ U+23EA BLACK LEFT-POINTING DOUBLE TRIANGLE */
  { 0x23EA, CPP_UOP_EMOJI, NULL },
  /* ⏫ U+23EB BLACK UP-POINTING DOUBLE TRIANGLE */
  { 0x23EB, CPP_UOP_EMOJI, NULL },
  /* ⏬ U+23EC BLACK DOWN-POINTING DOUBLE TRIANGLE */
  { 0x23EC, CPP_UOP_EMOJI, NULL },
  /* ⏰ U+23F0 ALARM CLOCK */
  { 0x23F0, CPP_UOP_EMOJI, NULL },
  /* ⏳ U+23F3 HOURGLASS WITH FLOWING SAND */
  { 0x23F3, CPP_UOP_EMOJI, NULL },
  /* ⬛ U+2B1B BLACK LARGE SQUARE */
  { 0x2B1B, CPP_UOP_EMOJI, NULL },
  /* ⬜ U+2B1C WHITE LARGE SQUARE */
  { 0x2B1C, CPP_UOP_EMOJI, NULL },
  /* ⭐ U+2B50 WHITE MEDIUM STAR */
  { 0x2B50, CPP_UOP_EMOJI, NULL },
  /* ⭕ U+2B55 HEAVY LARGE CIRCLE */
  { 0x2B55, CPP_UOP_EMOJI, NULL },
};

#endif /* LIBCPP_UNICODE_OPERATORS_H */
