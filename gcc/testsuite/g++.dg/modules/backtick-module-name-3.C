// { dg-additional-options "-fmodules-ts -fbacktick" }
// escape-module-names: an escaped component is still a component.  An
// object-like macro may not name one (P3034), and the escape holds a word.

#define util 3
export module acme.`util`;	// { dg-error "cannot be an object-like macro" }
// { dg-prune-output "not writing module" }
