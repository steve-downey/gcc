// { dg-module-do compile }
// { dg-additional-options "-fmodules-ts -fbacktick" }
// escape-module-names: importing, with the escape on any component, names
// the module the declaration named; the two spellings may be mixed.

import acme.`class`.util;
import acme.`class`.`util`;
import `new`;

int h () { return f () + g (); }

// { dg-final { scan-assembler "_ZW4acmeW5classW4util1fv" } }
// { dg-final { scan-assembler "_ZW3new1gv" } }
