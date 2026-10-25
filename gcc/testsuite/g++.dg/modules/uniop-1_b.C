// { dg-module-do compile }
// { dg-additional-options "-fmodules-ts -funicode-operators" }
// The importing side: the operator syntax finds the exported operators, the
// imported templates instantiate their dependent uses here, and the operator
// functions mangle with the module-attachment prefix as any other exported
// entity does.

import uniop;

static_assert ((W{1} ⊞ W{2}).v == 12);
static_assert ((⊖W{3}).v == -3);
static_assert ((M{1} ⊗ M{2}) == 102);

static_assert (combine (W{1}, W{2}).v == 12);
static_assert (negate (W{3}).v == -3);
static_assert (mem (M{1}, M{2}) == 102);

// The importer's own operator⊞ overloads the imported one.
constexpr int operator⊞ (int a, int b) { return a + b; }
static_assert ((1 ⊞ 2) == 3);
static_assert ((W{1} ⊞ W{2}).v == 12);

W use (W a, W b) { return a ⊞ b ⊞ ⊖a; }

// { dg-final { scan-assembler "_ZW5uniopv28op_u229ES_1WS0_" } }
// { dg-final { scan-assembler "_ZW5uniopv18op_u2296S_1W" } }
