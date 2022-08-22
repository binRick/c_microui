project('c_mui',['c'],default_options:['warning_level=0','c_std=gnu99','default_library=static'], version : '0.0.1',subproject_dir: 'subprojects')
cc = meson.get_compiler('c')
pwd = meson.current_source_dir()
#sdl2_config_bin = find_program('sdl2-config',required:true)

c_deps_dir = join_paths(meson.current_source_dir(),'submodules/c_deps')
add_global_arguments(
  '-Wno-unused-command-line-argument',
  '-lSDL2','-lSDL2_ttf','-lSDL2_image',
  language : 'c',
)
#############################################################
inc = [
  include_directories('./'),
  './',
  include_directories('./mui-windows'),
  include_directories('./submodules/c_deps/submodules/c_ansi'),
  include_directories('./submodules/c_deps/submodules/c_ansi/ansi-codes'),
  include_directories('./submodules/c_deps/submodules'),
  include_directories(
    './submodules/c_deps/submodules/c_ansi/ansi-codes',
    './submodules/c_deps/submodules/c_ansi',
  ),
  include_directories(
    './submodules/c_deps/submodules/c_darwin',
    './submodules/c_deps/submodules/c_darwin/active-app',
    './submodules/c_deps/submodules/c_darwin/window-utils',
    './submodules/c_deps/submodules/c_darwin/app-utils',
    './submodules/c_deps/submodules/c_darwin/process',
  ),
  './submodules/c_deps/submodules/djbhash',
  './submodules/c_deps/submodules/djbhash/src',
  include_directories('./mui','./mui-render'),
  './mui-atlas',
  './mui-render',
  './mui',
  './',
  './submodules',
  'submodules/iowow/src',
  'submodules/iowow/src/kv',
  'submodules/iowow/src/fs',
  'submodules/iowow/src/log',
  'submodules/iowow/src/json',
  'submodules/iowow/src/platform',
  'submodules/iowow',
  include_directories(
  'submodules/c_deps/submodules/iowow/src',
  'submodules/c_deps/submodules/iowow/src/kv',
  'submodules/c_deps/submodules/iowow/src/fs',
  'submodules/c_deps/submodules/iowow/src/log',
  'submodules/c_deps/submodules/iowow/src/json',
  'submodules/c_deps/submodules/iowow/src/platform',
  'submodules/c_deps/submodules/iowow',
  ),
  './submodules/c_deps/submodules/c_ansi',
  './submodules/c_deps/submodules/c_ansi/ansi-codes',
  './submodules/c_deps/submodules/c_darwin',
  './submodules/c_deps/submodules/c_darwin/active-app',
  './submodules/c_deps',
  './submodules/c_deps/meson',
  include_directories(
    './submodules',
    './submodules/c_deps',
    './submodules/c_deps/submodules',
    './submodules/c_deps/submodules/c_ansi',
  ),
  include_directories('.'),
]
subdir('meson')
pngs = [
  ]
fonts = [
  'sourcecodepro',
  'ibmplex',
  'nerd',
  'inconsolata',
  'devicons',
  'firacode',
  'retro',
  'froufrou',
  'fontawesome',
  'glasstty',
  ]
#//////////////////
foreach i : pngs
  configure_file(
    input: 'assets/'+i+'.png',
    output: i+'.png',
    copy: true,
  )
endforeach
foreach f : fonts
  configure_file(
    input: 'assets/'+f+'.ttf',
    output: f+'.ttf',
    copy: true,
  )
endforeach
configure_file(
  input: 'assets/window_icon.bmp',  
  output: 'window_icon.bmp',   
  copy: true,
)
#//////////////////
subdir('submodules')
#//////////////////
#subdir('mui-meson-repos')
#//////////////////
#subdir('mui-colors')
#subdir('mui-colors-test')
subdir('mui')
subdir('mui-atlas')
subdir('mui-render')
subdir('mui-rectangle')
subdir('mui-basic')
subdir('mui-windows')
#//////////////////
subdir('mui-test')
subdir('mui-basic-test')
subdir('mui-windows-test')
subdir('mui-rectangle-test')
#//////////////////
