int bump(int n) { n = n + 1; return n; }
int main() {
    int n = 1;
    print(bump(n));
    print(n);
    int k = bump(bump(n));
    print(k);
    print(n);
    return 0;
}
