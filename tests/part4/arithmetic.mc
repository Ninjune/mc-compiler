int main() {
    int big = 2147483647;
    print(big + 1);
    print(-big - 1);
    print((-big - 1) - 1);
    print(65536 * 65536);
    print(46341 * 46341);
    print(7 / 2); print(-7 / 2); print(7 / -2); print(-7 / -2);
    print(7 % 3); print(-7 % 3); print(7 % -3); print(-7 % -3);
    int m = -2147483647 - 1;
    print(m / -1);
    print(m % -1);
    print(1 - 2 - 3);
    print(100 / 10 / 5);
    print(2 * 3 % 4);
    print(123456789 + 4096 - 2048 - -2049);
    return 0;
}
