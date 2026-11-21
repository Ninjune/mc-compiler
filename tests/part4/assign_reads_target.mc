// Assignments whose right-hand side reads the variable being assigned.
// b = !b must test the old value of b, so the result cannot be built
// directly in b (b := 0 would destroy the value being tested).
int main() {
    bool b = true;
    bool c = false;
    b = !b;
    if (b) print(1); else print(0);
    b = !b;
    if (b) print(1); else print(0);
    c = c || b;
    if (c) print(1); else print(0);
    b = b && !c;
    if (b) print(1); else print(0);
    int x = 5;
    x = x * x - x;
    print(x);
    bool d = 3 < 4;
    d = d == (x < 0);
    if (d) print(1); else print(0);
    return 0;
}
