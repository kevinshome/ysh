<p align="center">
  <img src="ysh_sign_transparent.png">

<br>

<a href = "https://circleci.com/gh/kevinshome/workflows/ysh" alt="circleci">
  <img src="https://img.shields.io/circleci/build/github/kevinshome/ysh?token=abc123def456?style=flat-square" alt="circleci-badge"></a>
<a href="https://github.com/kevinshome/ysh/blob/master/LICENSE" alt="license">
  <img src="https://img.shields.io/github/license/kevinshome/ysh?style=flat-square" alt="license-badge"></a>
<a href="https://github.com/kevinshome/ysh/releases" alt="version">
  <img src="https://img.shields.io/github/v/release/kevinshome/ysh?include_prereleases?style=flat-square" alt="version-badge"></a>
  <img src="https://img.shields.io/maintenance/no/2019?style=flat-square">
  <img src="https://img.shields.io/github/commits-since/kevinshome/ysh/latest?style=flat-square">

</p>

** NOTE ** 
As of September 2019, this repository is no longer being actively maintained and updated, as I have decided to move on to other projects, I may return to actively update ysh in the future, but as of right now, I don't see it happening for a while.

the yikes shell (also known as ysh) is a basic shell for linux-based systems.
as of right now, it's basically just a hobby of mine and has no real competition with
the likes of bash, zsh, fish, etc., but, i guess only time will tell, because didn't linux
itself just start out as linus torvalds' side project?

go ahead and clone ysh today (fork it too if you want, why not, right?):

```
$ git clone https://github.com/kevinshome/ysh.git
```
## installation

requirements to build from source:

  - libreadline-dev
  - clang (for make)/gcc (for make gcc)
  - make


the installation process for ysh is pretty easy, as it
only revolves around the single Makefile in the root directory:

to install to /usr/bin:

```
$ cd ysh

$ make (or "$ make gcc" to use gcc as compiler)

$ make install
```

and then all you have to do is type:

```
$ ysh
```
and you'll be in!

## license

ysh is distributed under the MIT license. a copy of the license can be found in the LICENSE file
in the root directory of this repository.
