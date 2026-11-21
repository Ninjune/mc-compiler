int show(int v) { print(v); return v; }
int add3(int a, int b, int c) { return a * 100 + b * 10 + c; }

int main() {
    print(add3(show(1), show(2), show(3)));
    print(add3(read(), read(), read()));
    print(add3(show(add3(1, 1, 1)), 0, show(9)));
    return 0;
}
