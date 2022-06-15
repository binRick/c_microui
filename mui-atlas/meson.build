mui_atlas_srcs = [
  'mui-atlas.c',
]
mui_atlas_inc = [
 inc,
]
mui_atlas_deps = [
]

mui_atlas_lib = static_library('mui-atlas',
  mui_atlas_srcs,
  install: false,
  dependencies: mui_atlas_deps,
  include_directories: [
    mui_atlas_inc,
  ],
)

mui_atlas_dep = declare_dependency(
  include_directories: mui_atlas_inc,
  link_with: mui_atlas_lib,
)
