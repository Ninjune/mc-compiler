int main() {
    int n = read();
    int sum = 0;
    int evens = 0;
    int min = 0;
    int max = 0;
    int i = 0;
    while (i < n) {
        int x = read();
        sum = sum + x;
        if (x % 2 == 0) evens = evens + 1;
        if (i == 0 || x < min) min = x;
        if (i == 0 || x > max) max = x;
        i = i + 1;
    }
    print(sum);
    print(evens);
    print(min);
    print(max);
    return 0;
}
