# script-test

This is my test to find my *personal* choice of scripting engine + language. It's currently a work in progress.

## Criteria

The criteria a language would be judged would be, among others:

* Speed. The scripting language can't be a crappy one. It is, on the other hand a scripting langue, not C.
* Ease of use in C++. If binding or something else becomes hard, we have a problem.
* Ease of use in scripting language. If the language isn't easier, we can just use C++.
* Fast turnaround. We don't want to be waiting on a compiler. This includes both on the scripting side and on the C++ side when writing the binding code.
* Familiarity. We should be able to google and find answers, or atleast be able to ask other people.
* Activity. Like the last point, the system needs to be "active", not dormant.
* Portability. It should work on different platforms.
* Editor support. The more the merrier but we don't want to editor plugins to just get syntax highlighers.
* Debugging. Bugs happen and it would be prefereable if we can use something that is more than printf debugging.
* Size. We don't want to add a lot of bulk for (just) a scripting language.

## Additional requirements

* Operator overloading. `a + b` for custom type is prefered over `add(a, b)` (if the language supports operators in general).
* Stability. Bad code shouldn't be able to crash the rest of the program.
* Error reporting. Errors should be understandable and C++ shouldn't leak into the scripting language.

## Languages

### Languages that are 'complete'

* [AngelScript](http://www.angelcode.com/angelscript/)
* [ChaiScript](http://chaiscript.com/)

### Languages in progress

* [lua](http://www.lua.org/)

Besides lua jit there are several languages that compiles to lua so we have to try different flavours:

* Standard lua
* [MoonScript](http://moonscript.org/)
* [more languages](https://github.com/hengestone/lua-languages)


### Languages that will be tested

* [GameMonkey](http://www.gmscript.com/)
* [Pawn](https://github.com/compuphase/pawn)
* [Falcon](http://www.falconpl.org/)
* [NekoVM](https://nekovm.org)
* [python](https://docs.python.org/3.4/extending/embedding.html)
* [squirrel](http://www.squirrel-lang.org/)
