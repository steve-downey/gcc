// { dg-additional-options "-fmodules-ts -fbacktick" }
// escape-module-names: a module-partition, and a first component, escaped.
// An alternative token is a word like any other, so `and` is the
// identifier and.

export module `new`:`and`;
// { dg-module-cmi new:and }

export int g () { return 2; }

// { dg-final { scan-assembler "_ZW3new1gv" } }
