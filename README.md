<p align="center">
  <img src="ysh_sign_transparent.png">

<br>

<a href = "https://github.com/kevinshome/ysh/actions/workflows/build.yml" alt="build-status">
  <img src="https://github.com/kevinshome/ysh/actions/workflows/build.yml/badge.svg?branch=master&event=push" alt="build-status"></a>
<a href="https://github.com/kevinshome/ysh/blob/master/LICENSE" alt="license">
  <img src="https://img.shields.io/github/license/kevinshome/ysh" alt="license-badge"></a>
<a href="https://github.com/kevinshome/ysh/releases" alt="version">
  <img src="https://img.shields.io/github/v/release/kevinshome/ysh?include_prereleases" alt="version-badge"></a>
  <img src="https://img.shields.io/maintenance/yes/2022">
  <img src="https://img.shields.io/github/commits-since/kevinshome/ysh/latest">

</p>

the yikes shell (also known as ysh) is a basic shell for linux-based systems.
as of right now, it's basically just a hobby of mine and has no real competition with
the likes of bash, zsh, fish, etc., but, i guess only time will tell, because didn't linux
itself just start out as linus torvalds' side project?

go ahead and clone ysh today (fork it too if you want, why not, right?):

```
$ git clone https://github.com/kevinshome/ysh.git
```
## building

requirements to build from source:

  - libreadline-dev
  - C compiler (GCC recommended)
  - [meson](https://mesonbuild.com)

to build ysh from source, run the following commands in the root directory:
```
meson build
cd build && ninja
```
then you can start up the shell by simply running:
```
ysh
```

## installing & uninstalling

to install ysh to your system, run:
```
meson build
cd build && ninja install
```

and to uninstall ysh, run:
```
meson build
cd build && ninja uninstall
```

## license

ysh is distributed under the MIT license. a copy of the license can be found in the LICENSE file
in the root directory of this repository.
