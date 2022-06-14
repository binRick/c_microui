mui_srcs = [
  'mui.c',
]
mui_inc = [
 inc,
 '.',
 '../mui',
 '../mui-render',
]
mui_deps = [
  sdl2_dep,
]

mui_lib = library('mui',
  mui_srcs,
  install: false,
  dependencies: mui_deps,
  include_directories: [
    mui_inc,
  ],
)

mui_dep = declare_dependency(
  include_directories: mui_inc,
  link_with: mui_lib,
)
