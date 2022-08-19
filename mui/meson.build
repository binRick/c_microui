mui_srcs = [
  'mui.c',
]
mui_inc = [
 inc,
]
mui_deps = [
  sdl2_image_dep,
  sdl2_dep,
  window_utils_dep,
]

mui_lib = static_library('mui',
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
