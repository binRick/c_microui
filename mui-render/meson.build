mui_render_srcs = [
  'mui-render.c',
  'mui-icons.c',
  ]
mui_render_link_flags = ['-Wl,-framework,CoreFoundation','-Wl,-framework,Carbon','-Wl,-framework,Foundation', '-Wl,-framework,AppKit','-WlX11',
  '-Wl,-framework,Cocoa',

  ]
mui_render_inc = [
 inc,
 '../',
 '../mui',
 '../mui-render',
 '../mui-atlas',
]
mui_render_deps = [
  sdl2_dep,
  sdl2_ttf_dep,
  sdl2_image_dep,
  mui_atlas_dep,
  stb_dep,
  mui_dep,
  incbin_dep,
]
mui_cargs = [
]

mui_render_lib = static_library('mui-render',
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
  link_args: window_utils_link_flags,
)
