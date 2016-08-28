# script-test

This is my test to find my *personal* choice of scripting engine + language. It's currently a work in progress.

## Criteria

The criteria a language would be judged would be, among others:

 * Speed. The scripting language can't be a crappy one. It is, on the other hand a scripting langue, not C.
 * Ease of use in C++. If binding or something else becomes hard, we have a problem.
 * Ease of use in scripting language. If the language isn't easier, we can use C++.
 * Fast turnaround. We don't want to be waiting on a compiler.
 * Familiarity. We should be able to google and find answers, or atleast be able to ask other people.
 * Activity. Like the last point, the system needs to be "active", not dormant.
 * Portability. It should work on different platforms.
 * Editor support. The more there is the better.
 * Size. We don't want to add a lot of bulk for (just) a scripting language.

 
 ## Languages
 
 Languages that will be tested:
 
 * [GameMonkey](http://www.gmscript.com/)
 * [Pawn](https://github.com/compuphase/pawn)
 * [AngelScript](http://www.angelcode.com/angelscript/)
 * [ChaiScript](http://chaiscript.com/)
 * [Falcon](http://www.falconpl.org/)
 * [python](https://docs.python.org/3.4/extending/embedding.html)
 * [squirrel](http://www.squirrel-lang.org/)
 * javascript and typescript through [v8](https://github.com/v8/v8/wiki/Getting%20Started%20with%20Embedding) and [SpiderMonkey](https://developer.mozilla.org/en-US/docs/Mozilla/Projects/SpiderMonkey).
 
 As lua has various bindings, and like js/typescript has various "variants" one has to try different variants:
 
 * Standard lua
 * [MoonScript](http://moonscript.org/)
 
 and binding via
 
 * lua API/custom binding
 * [Selene](https://github.com/jeremyong/Selene)
 * [LuaBind](http://www.rasterbar.com/products/luabind.html)
 * [Simple Lua Binder](https://code.google.com/archive/p/slb/) - dead?
 * [OOLua](https://oolua.org/)
 * [tolua++](http://webserver2.tecgraf.puc-rio.br/~celes/tolua/)
 * [MLuaBind](https://sourceforge.net/projects/mluabind/)
 * [SWIG](http://www.swig.org/)
 * [LuaBridge](https://github.com/vinniefalco/LuaBridge)
 
 ## Testing
 Everything will be tested with a few tests:
 * Calculate a number in the Fibonacci series, once with recurssion and once with looping
 * Calculate the length of a string by combinining smaller strings gotten from C++
 