project('c_microui',['c'],default_options:['warning_level=0','c_std=gnu99'])
cc = meson.get_compiler('c')
pwd = meson.current_source_dir()
meson_deps_dir = meson.current_source_dir()
sdl2_dep = dependency('sdl2',fallback:['sdl2', 'sdl2'], required: true)
sdl2_cflags = run_command(
                find_program('sdl2-config', required : true), 
                ['--libs'], 
                check: true,
              ).stdout().strip()

#############################################################
osx_ldflags =  []
#osx_ldflags += ['-Wl,-framework,CoreFoundation']
#osx_ldflags += ['-Wl,-framework,Carbon']
#osx_ldflags += ['-Wl,-framework,Foundation', '-Wl,-framework,AppKit']
osx_ldflags += ['-Wl,-framework,OpenGL']
#osx_ldflags += ['-Wl,-framework OpenGL']
#add_project_link_arguments(osx_ldflags, language : ['objc', 'c'])
inc = [
  include_directories('.'),
  include_directories('./mui'),
  include_directories('./mui-render'),
]
mui_global_cflags = [
  sdl2_cflags,
]
mui_global_linkflags = [
  '-framework OpenGL',
]
_mui_global_linkflags = [
  ' -lm -O3 -g',
  '-Wall -std=c11 -pedantic',
]
add_project_arguments(mui_global_cflags, language: 'c')
add_project_link_arguments(osx_ldflags, language : ['objc', 'c'])

subdir('mui')
subdir('mui-atlas')
subdir('mui-render')
subdir('mui-test')
