#!/usr/bin/env python3
"""Usage: testmenu.py MENU SPEC CACHE

Print a directory holding MENU's files, fetching them as SPEC says. Like
FUJINET_LIB, SPEC can be
  - empty, for the latest release
  - a version number such as 1.0.0
  - a git URL, optionally with #branch, which is cloned and built
  - a directory, which is used as is if it holds the files, else built
"""

import json
import os
import re
import subprocess
import sys
import urllib.request
import zipfile

OWNER = "RichStephens"

# The files a release zip holds, the make targets that build them, and where
# a build leaves them.
MENUS = {
    "cocomenu": {
        "files": ("cocomenu.bin", "AUTOEXEC.BAS"),
        "targets": ("cocomenu.bin", "AUTOEXEC.BAS"),
        "out": "",
    },
    "adam-menu": {
        "files": ("adam-menu.ddp",),
        "targets": ("adam",),
        "out": "r2r/adam",
    },
    "msdos-menu": {
        "files": ("dosmenu.exe",),
        "targets": ("dosmenu.exe",),
        "out": "",
    },
}

VERSION_RE = re.compile(r"v?([0-9]+[.][0-9]+[.][0-9]+)$")


def fail(*args):
    print(*args, file=sys.stderr)
    sys.exit(1)


class Menu:
    def __init__(self, name, cache):
        if name not in MENUS:
            fail(f"No test menu called {name!r}; known: {', '.join(MENUS)}")
        self.name = name
        self.cache = cache
        self.__dict__.update(MENUS[name])

    def has_files(self, path):
        return all(os.path.isfile(os.path.join(path, f)) for f in self.files)

    def build(self, path):
        # Neither the parent make's flags nor its library are meant for the menu.
        env = {k: v for k, v in os.environ.items()
               if k not in ("MAKEFLAGS", "MFLAGS", "MAKELEVEL", "FUJINET_LIB")}
        subprocess.run(["make", "-C", path, *self.targets], check=True,
                       stdout=sys.stderr, env=env)
        return os.path.join(path, self.out) if self.out else path

    def cached_versions(self):
        found = []
        prefix = re.escape(self.name)
        if os.path.isdir(self.cache):
            for entry in os.listdir(self.cache):
                m = re.match(prefix + r"-([0-9]+)[.]([0-9]+)[.]([0-9]+)$", entry)
                if m and self.has_files(os.path.join(self.cache, entry)):
                    found.append(tuple(int(n) for n in m.groups()))
        return [".".join(map(str, v)) for v in sorted(found)]

    def latest(self):
        try:
            url = f"https://api.github.com/repos/{OWNER}/{self.name}/releases/latest"
            with urllib.request.urlopen(url) as response:
                tag = json.loads(response.read().decode("UTF-8")).get("tag_name", "")
        except OSError as e:
            cached = self.cached_versions()
            if not cached:
                fail(f"Can't look up the latest {self.name} release: {e}")
            return cached[-1]
        m = VERSION_RE.match(tag)
        if not m:
            fail(f"The latest {self.name} release is tagged {tag!r}, not a version")
        return m.group(1)

    def release(self, version):
        path = os.path.join(self.cache, f"{self.name}-{version}")
        if self.has_files(path):
            return path
        os.makedirs(self.cache, exist_ok=True)
        zip_path = path + ".zip"
        url = (f"https://github.com/{OWNER}/{self.name}/releases/download"
               f"/v{version}/{self.name}-{version}.zip")
        try:
            urllib.request.urlretrieve(url, zip_path)
        except OSError as e:
            fail(f"Can't download {url}: {e}")
        with zipfile.ZipFile(zip_path) as zf:
            zf.extractall(path)
        return path

    def clone(self, url):
        branch = ""
        if "#" in url:
            url, branch = url.split("#", 1)
        base = url.rstrip("/").split("/")[-1].removesuffix(".git")
        path = os.path.join(self.cache, base + (f"-{branch}" if branch else ""))
        if not os.path.isdir(path):
            os.makedirs(self.cache, exist_ok=True)
            cmd = ["git", "clone", "--depth", "1", url, path]
            if branch:
                cmd[2:2] = ["--branch", branch]
            subprocess.run(cmd, check=True, stdout=sys.stderr)
        return self.build(path)

    def locate(self, spec):
        m = VERSION_RE.match(spec)
        if not spec:
            return self.release(self.latest())
        if m:
            return self.release(m.group(1))
        if "://" in spec or "@" in spec:
            return self.clone(spec)
        if os.path.isdir(spec):
            if self.has_files(spec):
                return spec
            if os.path.isfile(os.path.join(spec, "Makefile")):
                return self.build(spec)
            return spec
        fail(f"{spec!r} is not a version, a git URL or a directory")


def main():
    if len(sys.argv) != 4:
        fail(__doc__)
    name, spec, cache = sys.argv[1:]
    menu = Menu(name, cache)
    path = menu.locate(spec)
    if not menu.has_files(path):
        fail(f"{path} has no {' and '.join(menu.files)}")
    print(path)


if __name__ == "__main__":
    main()
