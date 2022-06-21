project('c_mui',['c'],default_options:['warning_level=0','c_std=gnu99'], version : '0.0.1')
#############################################################
timeout_bin = find_program('timeout', required : true)
sdl2_dep = dependency('sdl2',fallback:['sdl2', 'sdl2'], required: true)
#############################################################
cc = meson.get_compiler('c')
meson_deps_dir = join_paths(meson.current_source_dir(),'submodules/meson_deps')
pwd = join_paths(meson.current_source_dir(),'submodules/meson_deps')
mui_global_linkflags = ['-Wl,-framework,OpenGL']
add_project_link_arguments(mui_global_linkflags, language : ['c'])
#############################################################


#//////////////////
inc = [
  include_directories('.'),
  include_directories('./mui'),
  include_directories('./mui-render'),
  include_directories('./mui-atlas'),
]
#//////////////////



#//////////////////
configure_file(
  input: 'assets/window_icon.bmp',  
  output: 'window_icon.bmp',   
  copy: true,
)


subdir('submodules/meson_deps/meson/osx')
add_project_link_arguments(osx_ldflags, language : ['objc', 'c'])

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
