#!/usr/bin/env python3
"""Print a directory holding cocomenu.bin and AUTOEXEC.BAS, fetching them as
COCOMENU says. Like FUJINET_LIB, COCOMENU can be
  - empty, for the latest release
  - a version number such as 1.0.0
  - a git URL, optionally with #branch, which is cloned and built
  - a directory, which is built if it has a Makefile
"""

import json
import os
import re
import subprocess
import sys
import urllib.request
import zipfile

REPO = "RichStephens/cocomenu"
FILES = ("cocomenu.bin", "AUTOEXEC.BAS")
VERSION_RE = re.compile(r"v?([0-9]+[.][0-9]+[.][0-9]+)$")


def fail(*args):
    print(*args, file=sys.stderr)
    sys.exit(1)


def has_files(path):
    return all(os.path.isfile(os.path.join(path, f)) for f in FILES)


def build(path):
    # The parent make's flags and command-line variables are not for cocomenu.
    env = {k: v for k, v in os.environ.items()
           if k not in ("MAKEFLAGS", "MFLAGS", "MAKELEVEL", "COCOMENU")}
    subprocess.run(["make", "-C", path, *FILES], check=True, stdout=sys.stderr, env=env)


def cached_versions(cache):
    found = []
    if os.path.isdir(cache):
        for name in os.listdir(cache):
            m = re.match(r"cocomenu-([0-9]+)[.]([0-9]+)[.]([0-9]+)$", name)
            if m and has_files(os.path.join(cache, name)):
                found.append(tuple(int(n) for n in m.groups()))
    return [".".join(map(str, v)) for v in sorted(found)]


def latest(cache):
    try:
        url = f"https://api.github.com/repos/{REPO}/releases/latest"
        with urllib.request.urlopen(url) as response:
            tag = json.loads(response.read().decode("UTF-8")).get("tag_name", "")
    except OSError as e:
        cached = cached_versions(cache)
        if not cached:
            fail(f"Can't look up the latest cocomenu release: {e}")
        return cached[-1]
    m = VERSION_RE.match(tag)
    if not m:
        fail(f"The latest cocomenu release is tagged {tag!r}, not a version")
    return m.group(1)


def release(version, cache):
    path = os.path.join(cache, f"cocomenu-{version}")
    if has_files(path):
        return path
    os.makedirs(cache, exist_ok=True)
    zip_path = path + ".zip"
    url = f"https://github.com/{REPO}/releases/download/v{version}/cocomenu-{version}.zip"
    try:
        urllib.request.urlretrieve(url, zip_path)
    except OSError as e:
        fail(f"Can't download {url}: {e}")
    with zipfile.ZipFile(zip_path) as zf:
        zf.extractall(path)
    return path


def clone(url, cache):
    branch = ""
    if "#" in url:
        url, branch = url.split("#", 1)
    base = url.rstrip("/").split("/")[-1].removesuffix(".git")
    path = os.path.join(cache, base + (f"-{branch}" if branch else ""))
    if not os.path.isdir(path):
        os.makedirs(cache, exist_ok=True)
        cmd = ["git", "clone", "--depth", "1", url, path]
        if branch:
            cmd[2:2] = ["--branch", branch]
        subprocess.run(cmd, check=True, stdout=sys.stderr)
    build(path)
    return path


def main():
    spec, cache = sys.argv[1], sys.argv[2]

    m = VERSION_RE.match(spec)
    if not spec:
        path = release(latest(cache), cache)
    elif m:
        path = release(m.group(1), cache)
    elif "://" in spec or "@" in spec:
        path = clone(spec, cache)
    elif os.path.isdir(spec):
        path = spec
        if os.path.isfile(os.path.join(path, "Makefile")):
            build(path)
    else:
        fail(f"COCOMENU={spec!r} is not a version, a git URL or a directory")

    if not has_files(path):
        fail(f"{path} has no {' and '.join(FILES)}")
    print(path)


if __name__ == "__main__":
    main()
