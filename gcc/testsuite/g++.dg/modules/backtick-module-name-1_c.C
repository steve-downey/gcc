// { dg-additional-options "-fmodules-ts -fbacktick" }
// escape-module-names: an escaped partition in an import.

export module `new`;
// { dg-module-cmi new }

export import :`and`;
