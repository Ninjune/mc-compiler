int fib(int n) { if (n < 2) return n; return fib(n - 1) + fib(n - 2); }
int ack(int m, int n) {
    if (m == 0) return n + 1;
    if (n == 0) return ack(m - 1, 1);
    return ack(m - 1, ack(m, n - 1));
}
int gcd(int a, int b) { if (b == 0) return a; return gcd(b, a % b); }
int power(int b, int e) { if (e == 0) return 1; return b * power(b, e - 1); }
bool isEven(int n) { if (n == 0) return true; return isOdd(n - 1); }
bool isOdd(int n) { if (n == 0) return false; return isEven(n - 1); }
int digitSum(int n) { if (n < 10) return n; return n % 10 + digitSum(n / 10); }

int main() {
    print(fib(15));
    print(ack(2, 3));
    print(gcd(1071, 462));
    print(power(3, 19));
    if (isOdd(51)) print(1); else print(0);
    print(digitSum(2147483647));
    return 0;
}
