bool same(bool p, bool q) { return p == q; }
int pick(bool c, int x, int y) { if (c) return x; return y; }

int main() {
    int n = read();
    bool big = n > 100;
    bool b = same(big, !big) != (1 == 2);
    n = pick(b && big || !big, n, -n);
    big = (n != 0) == true;
    print(n % 7 - -n / 3);
    return pick(big, 0, 1);
}
