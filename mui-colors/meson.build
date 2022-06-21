mui_colors_timeout_seconds = '.8'
mui_colors_srcs = [
  'mui-colors.c',
]
mui_colors_inc = [
 inc,
 '../mui-colors',
 '../mui',
 '../mui-render',
]
mui_colors_deps = [
 sdl2_dep,
 mui_dep,
 mui_render_dep,
]


mui_colors_lib = static_library('mui-colors',
  mui_colors_srcs,
  install: false,
  dependencies: mui_colors_deps,
  c_args: mui_cargs,
  include_directories: [
    mui_colors_inc,
  ],
)

mui_colors_dep = declare_dependency(
  include_directories: mui_colors_inc,
  link_with: mui_colors_lib,
)
