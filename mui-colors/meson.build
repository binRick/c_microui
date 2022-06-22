mui_colors_timeout_seconds = '.8'
mui_colors_srcs = [
  'mui-colors.c',
]
mui_colors_inc = [
 inc,
 '.',
 '../mui-colors',
 '../mui-atlas',
 '../mui',
 '../mui-render',
 '../submodules/meson_deps',
 '../submodules/meson_deps/submodules',
 '../submodules/meson_deps/submodules/c_darwin/active-app',
 '../submodules/meson_deps/submodules/c_darwin',
 '../submodules/meson_deps/submodules/c_ansi',
 '../submodules/meson_deps/submodules/c_ansi/ansi-utils',
 '../submodules/meson_deps/submodules/djbhash',
 '../submodules/meson_deps/submodules/djbhash/src',
]
mui_colors_deps = [
   djbhash_dep,
   sdl2_dep,
   mui_dep,
   mui_render_dep,
   active_app_dep,
   cargs_dep,
   colors_csv_parser_dep,
   colors_json_parser_dep,
   c_stringfn_dep,
   parson_dep,
   fs_dep,
   rgba_dep,
   ansi_codes_dep,
   ansi_rgb_utils_dep,
   bytes_dep,
   c_string_buffer_dep,
   db_dep,
   ansi_utils_deps, 
]


mui_colors_lib = static_library('mui-colors',
  mui_colors_srcs,
  install: false,
  dependencies: mui_colors_deps,
  c_args: mui_cargs,
  include_directories: mui_colors_inc,
)

mui_colors_dep = declare_dependency(
  include_directories: mui_colors_inc,
  link_with: mui_colors_lib,
)
