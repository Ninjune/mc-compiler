int main() {
    print(collatz(27));
    if (isEven(10) && !isOdd(10)) print(1);
    return 0;
}

int collatz(int n) {
    if (n == 1) return 0;
    if (n % 2 == 0) return 1 + collatz(n / 2);
    return 1 + collatz(3 * n + 1);
}

bool isEven(int n) { if (n == 0) return true; return isOdd(n - 1); }
bool isOdd(int n) { if (n == 0) return false; return isEven(n - 1); }
