// { dg-additional-options "-fmodules-ts -fbacktick" }
// escape-module-names: the escape holds a word, and closes.

export module acme.`+`;	// { dg-error "backtick escape requires an identifier" }
// { dg-prune-output "not writing module" }
