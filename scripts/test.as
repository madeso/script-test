void main()
{
  print("Hello world\n");
}

int fib_rec(int id) {
  if( id == 1 ) {
    return 1;
  } else if( id == 2) {
    return 2;
  } else {
    return fib_rec(id-1) + fib_rec(id-2);
  }
}

int fib_loop(int id) {
  if ( id == 1 ) {
    return 1;
  } else if (id == 2) {
    return 2;
  } else {
    int val = 0;
    int last1 = 1;
    int last2 = 2;

    for(int i=3; i<=id; ++i) {
      val = last1 + last2;
      last1 = last2;
      last2 = val;
    }

    return val;
  }
}

int string_test(int count) {
  string s = "";
  int v = 0;
  string x = "";

  for(int i=1; i<=count; ++i) {
    x = build_string(s);
    s = s + x;
    v = v + x.length();
  }

  return v;
}


