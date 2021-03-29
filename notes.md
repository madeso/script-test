## Javascript
* v8 is to complicated to integrate, duktape is better but has a (cumbersome) 'lua' c-api
* doesn't support operator overloading


## lua + sol2
* advanced template uses sometimes comfuses error highlightning in CLion (was true in 2015 but is it still valid in 2021?)
* since to work with


## chai
* slow compilation, ~10x of clua
* slow runtime, ~50-100x of clua
* in maintenence mode? https://github.com/ChaiScript/ChaiScript/issues/532


## AngelScript
* verbose/tedious/low-level scripting API
* no function calls outside of functions, ie you need to use the "main method"
