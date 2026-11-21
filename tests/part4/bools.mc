bool xor(bool a, bool b) { return a != b; }
bool isPos(int x) { return x > 0; }
int toInt(bool b) { if (b) return 1; return 0; }

int main() {
    bool p = true;
    bool q = false;
    print(toInt(xor(p, q)));
    print(toInt(xor(p, p)));
    bool r = isPos(5) && !isPos(-5);
    print(toInt(r));
    print(toInt(p == q));
    print(toInt(p == !q));
    q = p;
    print(toInt(q));
    bool s = 3 < 4 == 5 < 6;
    print(toInt(s));
    print(toInt(!(1 >= 2) && 2 <= 2 && -1 != 1));
    return 0;
}
