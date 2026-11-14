int f(int a, bool b, int c, bool d, int e, bool f, int g, bool h) {
    if (b && d && f && h) return a + c + e + g;
    return 0;
}
int main() { print(f(1, true, 2, true, 3, true, 4, true)); return 0; }
