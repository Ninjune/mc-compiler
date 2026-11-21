bool t(int id) { print(id); return true; }
bool f(int id) { print(id); return false; }
int main() {
    bool a = f(1) && t(2);
    bool b = t(3) || f(4);
    bool c = !(t(5) || t(6)) && t(7);
    if (a || b) print(10);
    if (c) print(11); else print(12);
    return 0;
}
