int f() {
    if (a) if (b) x = 1; else x = 2;
    if (a) { if (b) x = 1; } else x = 2;
    if (a) x = 1; else if (b) x = 2; else if (c) x = 3; else x = 4;
    if (a) if (b) if (c) x = 1; else x = 2; else x = 3;
    return x;
}
