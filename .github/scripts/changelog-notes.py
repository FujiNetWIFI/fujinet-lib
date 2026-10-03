#!/usr/bin/env python3
"""Print a tag's Changelog.md section; fail unless it is the newest numbered one and not empty."""

import re
import sys

HEADING = re.compile(r"^## \[([^\]]+)\]")


def fail(message):
    print(f"::error::{message}", file=sys.stderr)
    sys.exit(1)


def main():
    version = sys.argv[1].removeprefix("v")
    path = sys.argv[2] if len(sys.argv) > 2 else "Changelog.md"
    with open(path) as f:
        lines = f.read().splitlines()

    start = None
    for i, line in enumerate(lines):
        match = HEADING.match(line)
        if match and match.group(1) != "Unreleased":
            if match.group(1) != version:
                fail(f"The newest section in {path} is [{match.group(1)}], but the tag is {sys.argv[1]}.")
            start = i + 1
            break
    if start is None:
        fail(f"{path} has no [{version}] section.")

    end = next((i for i in range(start, len(lines)) if lines[i].startswith("## ")), len(lines))
    section = "\n".join(lines[start:end]).strip()
    if not section:
        fail(f"The [{version}] section in {path} is empty.")
    print(section)


if __name__ == "__main__":
    main()
