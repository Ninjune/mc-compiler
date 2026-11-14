int f(int a) {
    {
        bool a = true;
        if (a) { int a = 5; print(a); }
    }
    return a;
}

int main() {
    int f = 3; // a variable may share a function's name
    print(f(f));
    return 0;
}
