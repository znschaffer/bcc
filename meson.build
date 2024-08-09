project('bcc', 'c',
  version : '0.1',
  default_options : ['warning_level=3'])

# add_global_arguments('-DDEBUG', language : 'c')
exe = executable('bcc', 'bcc.c',
  install : true)

test('basic', exe)
