int f() {
    x = a || b && c == d < e + f * -g;
    x = (a || b) && (c == d);
    x = !a == b;
    x = -a - -b;
    x = a * (b + c) % (d - e);
    x = a < b == c < d;
    x = !(a < b) != (c >= d);
    return x;
}
