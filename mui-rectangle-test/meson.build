mui_rectangle_test_timeout_seconds = '.8'
mui_rectangle_test_srcs = [
  'mui-rectangle-test.c',
]
mui_rectangle_test_inc = [
 inc,
]
mui_rectangle_test_deps = [
 mui_rectangle_dep,
 sdl2_ttf_dep,
 sdl2_dep,
]

if get_option('enable-binaries')
  mui_rectangle_test_exec = executable('mui-rectangle-test',
     mui_rectangle_test_srcs,
     dependencies: mui_rectangle_test_deps,
     include_directories: mui_rectangle_test_inc,
     install : true, 
     install_dir : '/usr/local/bin',
  )
  if get_option('enable-binary-tests')
   if get_option('enable-mui-rectangle-test')
    if get_option('timeout-test')
      test('mui-rectangle-test', timeout_bin,
        args: [
          '--preserve-status',
          get_option('timeout').to_string(),
          mui_rectangle_test_exec,
        ]
      )
    else
      test('mui-rectangle-test', mui_rectangle_test_exec,
        args: [
        ]
      )
    endif
   endif
  endif
endif
