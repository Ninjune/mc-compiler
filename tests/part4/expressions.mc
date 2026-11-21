int main() {
    int a = 3;
    int b = 4;
    print(((1 + 2) * (3 + 4) - (5 * 6) / (7 - 8)) % 1000 + ((9 * 9 * 9) - (8 * 8 * 8)) * 2);
    print(a * a + b * b - (a + b) * (a + b) + 2 * a * b);
    print(-(-(-(a))) + -b * -b);
    print((a + b) * (a - b) / (a % b + 1) - b / a % 2);
    return 0;
}
