int read2() { return 2; }
int printx(int x) { print(x); return x; }
bool main2() { return true; }
int main() {
    int read2 = read2();
    bool main2 = main2();
    int printx = printx(read2);
    if (main2) print(printx);
    return 0;
}
