project('ysh', 'c', version : '0.3.0')
incdir = include_directories('include')
readline = dependency('readline', version : '>=4.2', method: 'pkg-config')
src = ['src/builtins.c', 'src/remchar.c', 'src/ysh.c']
executable('ysh', src, include_directories : incdir, dependencies: readline)