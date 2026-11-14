#!/usr/bin/env python3
"""Run all tests under tests/ using build/mcc and report pass/fail.

Test layout (sibling files next to each *.mc test):
  - *.out  -> expected stdout. Run mcc with --tokens, or --ast when the
              expected output starts with "(program", or --check for
              part3's OK tests.
  - *.err  -> expected stderr. Run mcc with --ast (or --check for part3's
              error tests, handled in main).
  - no sibling -> not a test.

Each test also asserts the compiler's exit code:
  - *.out  tests must exit 0 on success.
  - *.err  tests must exit 1 on failure.

Usage:
    python3 tests/run_tests.py [path/to/mcc]

The mcc binary defaults to build/mcc next to this file's repo root.
"""

from __future__ import annotations

import subprocess
import sys
from pathlib import Path
from typing import Any

REPO_ROOT: Path = Path(__file__).resolve().parent.parent
DEFAULT_MCC: Path = REPO_ROOT / "build" / "mcc"
TESTS_DIR: Path = REPO_ROOT / "tests"


def normalize(text: Any) -> str:
    """Drop trailing newlines.

    main.cpp prints an extra newline after a tokens dump, so a single
    trailing blank line must not count as a mismatch.
    """
    return text.rstrip("\n")


def run_mcc(
    mcc: Any,
    source: Any,
    flags: list[str],
) -> subprocess.CompletedProcess[Any]:
    return subprocess.run(
        [str(mcc), str(source), *flags],
        capture_output=True,
        text=True,
    )


def dirname(mc: Path) -> str:
    """Part directory name for a test, or "" if it is not in a part dir."""
    return mc.parent.name


def expected_flags(expected_text: str, mc: Path) -> list[str]:
    if dirname(mc) == "part1":
        return ["--tokens"]
    if dirname(mc) == "part2":
        return ["--ast"]
    if dirname(mc) == "part3":
        return ["--check"]
    return []


def discover_tests(tests_dir: Path) -> list[Path]:
    tests: list[Path] = []
    for mc in sorted(tests_dir.rglob("*.mc")):
        stem = mc.stem  # strip .mc so sibling files are matched correctly
        if (mc.parent / (stem + ".out")).exists() or (mc.parent / (stem + ".err")).exists():
            tests.append(mc)
    return tests


def main() -> int:
    mcc: Path = Path(sys.argv[1]) if len(sys.argv) > 1 else DEFAULT_MCC

    if not mcc.exists():
        print(f"mcc binary not found: {mcc}")
        print("Build it first, e.g. `make` or `cmake -S . -B build`.")
        return 1

    tests = discover_tests(TESTS_DIR)
    passed: int = 0
    failed: int = 0

    for mc in tests:
        stem = mc.stem  # strip .mc for sibling file names
        out_path: Path = mc.parent / (stem + ".out")
        err_path: Path = mc.parent / (stem + ".err")

        if out_path.exists() and err_path.exists():
            print(f"SKIP {mc.relative_to(TESTS_DIR)} (has both .out and .err)")
            continue

        if out_path.exists():
            expected: str = out_path.read_text()
            flags = expected_flags(expected, mc)
            proc = run_mcc(mcc, mc, flags)
            if normalize(proc.stdout) == normalize(expected) and proc.returncode == 0:
                passed += 1
                print(f"PASS  {mc.relative_to(TESTS_DIR)}")
            else:
                failed += 1
                print(f"FAIL  {mc.relative_to(TESTS_DIR)}")
                print(f"      flags: {flags}")
                print(f"      expected: {expected!r}")
                print(f"      got:      {proc.stdout!r}")
                print(f"      exit code: {proc.returncode} (expected 0)")
                if proc.stderr:
                    print(f"      stderr:   {proc.stderr!r}")
        elif err_path.exists():
            expected_err: str = err_path.read_text()
            flags = ["--check"] if dirname(mc) == "part3" else ["--ast"]
            proc = run_mcc(mcc, mc, flags)
            if proc.stderr == expected_err and proc.returncode == 1:
                passed += 1
                print(f"PASS  {mc.relative_to(TESTS_DIR)}")
            else:
                failed += 1
                print(f"FAIL  {mc.relative_to(TESTS_DIR)}")
                print(f"      expected stderr: {expected_err!r}")
                print(f"      got stderr:      {proc.stderr!r}")
                print(f"      exit code: {proc.returncode} (expected 1)")

    total = len(tests)
    print(f"\n{passed} passed, {failed} failed, {total - passed - failed} skipped.")
    return 0 if failed == 0 else 1


if __name__ == "__main__":
    sys.exit(main())
