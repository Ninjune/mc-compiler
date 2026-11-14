int a(bool c) { if (c) return 1; else return 2; }
int b(bool c) { if (c) { return 1; } else { if (!c) return 2; else return 3; } }
int d(bool c) { while (c) { return 1; } return 0; }
int e(bool c) { { { return 4; } } }
bool g(int x) { if (x > 0) return true; return false; }
int main() { if (g(1)) return a(true) + b(false) + d(true) + e(false); else return 0; }
