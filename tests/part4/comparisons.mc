int b(bool x) { if (x) return 1; return 0; }
int row(int x, int y) {
    return b(x < y) * 100000 + b(x <= y) * 10000 + b(x > y) * 1000
         + b(x >= y) * 100 + b(x == y) * 10 + b(x != y);
}
int main() {
    int min = -2147483647 - 1;
    print(row(1, 2));
    print(row(2, 1));
    print(row(5, 5));
    print(row(-3, 3));
    print(row(min, 2147483647));
    print(row(2147483647, min));
    print(row(0, -1));
    return 0;
}
