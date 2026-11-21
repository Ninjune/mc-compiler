// t and f print their argument, so the output shows which operands ran.
bool t(int id) { print(id); return true; }
bool f(int id) { print(id); return false; }

int main() {
    if (t(1) || f(2)) print(10);
    if (f(3) && t(4)) print(20); else print(21);
    if (!(f(5) || f(6)) && (t(7) || t(8))) print(30);
    bool b = f(9) || (t(10) && f(11)) || t(12);
    if (b) print(40);
    while (t(13) && f(14)) print(99);
    bool c = !t(15) || !f(16);
    if (c) print(50);
    return 0;
}
