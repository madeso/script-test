print("in test.lua")

-- accessing variables
foo = 4
bar = {}
bar[3] = "hi"
bar["key"] = "there"

-- calling lua functions
function ffoo()
end

function add(a, b)
  return a + b
end

function sum_and_difference(a, b)
  return (a+b), (a-b);
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

-- table.pack isn't abailable in 5.1
function custom_pack(...)
  return { n = select("#", ...), ... }
end

function print_wrapper(...)
  local args = custom_pack(...)
  local str = ""
  local first = true
  for i=1,args.n do
    if first then
      first = false
    else
      str = str .. " "
    end
    str = str .. tostring(args[i])
  end
  c_print(str)
end

old_print = print
print = print_wrapper

function fib_rec(id)
  if id == 1 then
    return 1
  elseif id == 2 then
    return 2
  else
    return fib_rec(id-1) + fib_rec(id-2)
  end
end

function fib_loop(id)
  if id == 1 then
    return 1
  elseif id == 2 then
    return 2
  else
    local val = 0
    local last1 = 1
    local last2 = 2

    for i=3, id do
      val = last1 + last2
      last1 = last2
      last2 = val
    end

    return val
  end
end

function string_test(count)
  local s = ""
  local v = 0
  local x = ""
  for i=1, count do
    x = build_string(s)
    s = s .. x
    v = v + string.len(x)
  end

  return v
end
