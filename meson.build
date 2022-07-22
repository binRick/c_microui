project('c_mui',['c'],default_options:['warning_level=0','c_std=gnu99'], version : '0.0.1',subproject_dir: 'submodules/meson_deps/subprojects')
cc = meson.get_compiler('c')
pwd = meson.current_source_dir()
meson_deps_dir = join_paths(meson.current_source_dir(),'submodules/meson_deps')
#############################################################
inc = [
  include_directories('.'),
  include_directories('./submodules/meson_deps/submodules/c_ansi'),
  include_directories('./submodules/meson_deps/submodules/c_ansi/ansi-codes'),
  include_directories('./submodules/meson_deps/submodules'),
  include_directories(
    './submodules/meson_deps/submodules/c_ansi/ansi-codes',
    './submodules/meson_deps/submodules/c_ansi',
  ),
  './submodules/meson_deps/submodules/c_darwin',
  './submodules/meson_deps/submodules/c_darwin/active-app',
  './submodules/meson_deps/submodules/djbhash',
  './submodules/meson_deps/submodules/djbhash/src',
  include_directories('.','vendor','vendor/sqlite'),
  include_directories('./mui','./mui-render'),
  './mui-atlas',
  './mui',
  './submodules',
  './submodules/meson_deps/submodules/c_ansi',
  './submodules/meson_deps/submodules/c_ansi/ansi-codes',
  './submodules/meson_deps/submodules/c_darwin',
  './submodules/meson_deps/submodules/c_darwin/active-app',
  './submodules/meson_deps',
  './submodules/meson_deps/meson',
  include_directories(
    './submodules',
    './submodules/meson_deps',
    './submodules/meson_deps/submodules',
    './submodules/meson_deps/submodules/c_ansi',
    './submodules/meson_deps/vendor',
    './submodules/meson_deps/vendor/sqlite',
  ),
  include_directories('.'),
]
subdir('meson')
#//////////////////
configure_file(
  input: 'assets/window_icon.bmp',  
  output: 'window_icon.bmp',   
  copy: true,
)
#//////////////////
subdir('submodules')
#//////////////////
subdir('mui')
subdir('mui-atlas')
subdir('mui-render')
#//////////////////
subdir('mui-test')
#//////////////////
subdir('mui-meson-repos')
#//////////////////
subdir('mui-colors')
subdir('mui-colors-test')
#//////////////////
subdir('mui-windows')
subdir('mui-windows-test')
