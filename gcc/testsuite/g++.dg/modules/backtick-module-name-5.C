// { dg-additional-options "-fmodules-ts -fbacktick" }
// escape-module-names: an unterminated escape in a module name.

export module acme.`class;	// { dg-error "expected '`'" }
// { dg-prune-output "not writing module" }
