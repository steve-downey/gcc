// { dg-module-do compile }
// { dg-additional-options "-fmodules-ts -fbacktick" }
// The importing side sees ordinary identifiers, and the names mangle with
// the module-attachment prefix exactly as any other exported entity does.

import backtick_escape;

int use ()
{
  Widget w;
  return `new` (1, 2) + w.`delete` ();
}

// { dg-final { scan-assembler "_ZW15backtick_escape3newii" } }
// { dg-final { scan-assembler "_ZNW15backtick_escape6Widget6deleteEv" } }
