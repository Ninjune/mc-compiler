// show and yes print their argument, so the output shows the order in which
// operands are evaluated (the handout says left to right).
int show(int v) { print(v); return v; }
bool yes(int v) { print(v); return true; }

int main() {
    print(show(1) - show(2));
    print(show(3) * show(4) + show(5));
    print(show(6) - (show(7) - show(8)));
    print(show(9) / show(10) + show(11) % show(12));
    if (show(13) < show(14)) print(100);
    bool b = show(15) == show(16);
    if (!b) print(200);
    while (show(17) > show(18)) print(300);
    if (yes(19) == yes(20)) print(400);
    print(-show(21) + show(22));
    return 0;
}
