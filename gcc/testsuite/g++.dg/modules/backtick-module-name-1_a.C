// { dg-additional-options "-fmodules-ts -fbacktick -MD" }
// escape-module-names: the keyword escape reaches each component of a
// module-name.  `class` is the identifier class, so the module is
// acme.class.util -- its CMI, its dependency name and its mangling are
// exactly those of the bare spelling, which no program could write.

export module acme.`class`.util;
// { dg-module-cmi acme.class.util }

export int f () { return 1; }

// { dg-final { scan-file backtick-module-name-1_a.d {\nacme\.class\.util\.c\+\+-module: gcm.cache/acme\.class\.util\.gcm} } }
// { dg-final { scan-assembler "_ZW4acmeW5classW4util1fv" } }
