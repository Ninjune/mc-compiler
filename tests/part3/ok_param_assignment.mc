int f(int a, bool b) {
    a = a + 1;
    b = !b;
    if (b) return a;
    return -a;
}
int main() { print(f(1, false)); return 0; }
