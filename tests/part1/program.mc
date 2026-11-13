// Euclid's algorithm
int gcd(int a, int b) {
    while (b != 0) { int t = a % b; a = b; b = t; }
    return a;
}

int main() {
    print(gcd(read(), read()));   /* reads two numbers */
    return 0;
}
