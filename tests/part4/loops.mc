int firstDivisor(int n) {
    int d = 2;
    while (d * d <= n) {
        if (n % d == 0) return d;
        d = d + 1;
    }
    return n;
}

int main() {
    int i = 1;
    while (i <= 4) {
        int j = 1;
        int row = 0;
        while (j <= i) { row = row * 10 + j; j = j + 1; }
        print(row);
        i = i + 1;
    }
    print(firstDivisor(91));
    print(firstDivisor(97));
    print(firstDivisor(1000001));
    while (false) print(0);
    return 0;
}
