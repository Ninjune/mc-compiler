int steps(int n) {
    int s = 0;
    while (n != 1) {
        if (n % 2 == 0) n = n / 2; else n = 3 * n + 1;
        s = s + 1;
    }
    return s;
}

int main() {
    int n = read();
    while (n != 0) {
        print(steps(n));
        n = read();
    }
    return 0;
}
