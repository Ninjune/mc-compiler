bool f(int n) {
    int a = 1;
    bool b = a < 2 && !false;
    a = 2;
    int c = a * (a + 1);
    bool d = b == (c != 6);
    return d;
}
