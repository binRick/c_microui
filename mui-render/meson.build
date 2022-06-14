mui_render_srcs = [
  'mui-render.c',
]
mui_render_inc = [
 inc,
 '.',
 '../mui',
 '../mui-render',
 '../mui-atlas',
]
mui_render_deps = [
  sdl2_dep,
  mui_atlas_dep,
  mui_dep,
]
mui_cargs = [
  sdl2_cflags,
]

mui_render_lib = library('mui-render',
  mui_render_srcs,
  install: false,
  dependencies: mui_render_deps,
  c_args: mui_cargs,
  include_directories: [
    mui_render_inc,
  ],
)

mui_render_dep = declare_dependency(
  include_directories: mui_render_inc,
  link_with: mui_render_lib,
)
