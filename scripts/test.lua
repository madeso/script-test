-- accessing variables
foo = 4
bar = {}
bar[3] = "hi"
bar["key"] = "there"

-- calling lua functions
function foo()
end

function add(a, b)
    return a + b
end

function sum_and_difference(a, b)
    return (a+b), (a-b);
end

function bar()
    return 4, true, "hi"
end

mytable = {}
function mytable.foo()
    return 4
end

function use_bar_class()
    return Bar.new(5):add_this(2)
end

function take_bar_class(bar)
    return bar:add_this(2)
end

function return_bar_class()
    return Bar.new(5)
end
