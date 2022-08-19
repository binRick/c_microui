mui_basic_test_timeout_seconds = '.8'
mui_basic_test_srcs = [
  'mui-basic-test.c',
]
mui_basic_test_inc = [
 inc,
]
mui_basic_test_deps = [
 mui_basic_dep,
]

if get_option('enable-binaries')
  mui_basic_test_exec = executable('mui-basic-test',
     mui_basic_test_srcs,
     dependencies: mui_basic_test_deps,
     include_directories: mui_basic_test_inc,
     install : true, 
     install_dir : '/usr/local/bin',
  )
  if get_option('enable-binary-tests')
   if get_option('enable-mui-basic-test')
    if get_option('timeout-test')
      test('mui-basic-test', timeout_bin,
        args: [
          '--preserve-status',
          get_option('timeout').to_string(),
          mui_basic_test_exec,
        ]
      )
    else
      test('mui-basic-test', mui_basic_test_exec,
        args: [
        ]
      )
    endif
   endif
  endif
endif
