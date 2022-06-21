mui_colors_test_timeout_seconds = '.8'
mui_colors_test_srcs = [
  'mui-colors-test.c',
]
mui_colors_test_inc = [
 inc,
]
mui_colors_test_deps = [
 mui_colors_dep,
]

if get_option('enable-binaries')
  mui_colors_test_exec = executable('mui-colors-test',
     mui_colors_test_srcs,
     dependencies: mui_colors_test_deps,
     include_directories: mui_colors_test_inc,
     install : true, 
     install_dir : '/usr/local/bin',
  )
  if get_option('enable-binary-tests')
   if get_option('enable-mui-colors-test')
    if get_option('timeout-test')
      test('mui-colors-test', timeout_bin,
        args: [
          '--preserve-status',
          get_option('timeout').to_string(),
          mui_colors_test_exec,
        ]
      )
    else
      test('mui-colors-test', mui_colors_test_exec,
        args: [
        ]
      )
    endif
   endif
  endif
endif
