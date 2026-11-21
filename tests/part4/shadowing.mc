int f(int a) {
    int b = a * 2;
    {
        int a = 100;
        b = b + a;
        {
            int b = 1;
            a = a + b;
            print(a);
        }
        print(a);
    }
    print(a);
    return b;
}

int main() {
    int a = 5;
    print(f(a));
    print(a);
    {
        bool a = false;
        if (!a) print(1);
    }
    print(a);
    return 0;
}
