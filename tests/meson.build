cpputest_subproject = subproject('cpputest', required: true)
cpputest = cpputest_subproject.get_variable('cpputest_dep')

foreach driver_name, driver_props: bts7960_instances
  driver_library = driver_props['library']
  driver_defines = driver_props['defines']
  test_defines = []

  foreach define: driver_defines
    test_defines += ['-D' + define]
  endforeach

  test_exec = executable(
    f'@driver_name@_test_executable',
    sources: 'mock_driver_test.cpp',
    dependencies: cpputest,
    link_with: driver_library,
    include_directories: bts7960_includes,
    c_args: test_defines,
    cpp_args: test_defines,
  )
  test(f'@driver_name@_test', test_exec)
endforeach
