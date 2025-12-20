project(
    'regex',
    'c',
    version: '0.1.0',
    default_options: [
        'c_std=c11',
        'warning_level=3',
        'werror=true',
        'buildtype=debug',
        'b_lundef=false',
        'b_sanitize=address,undefined',
    ],
)

cc = meson.get_compiler('c')

add_project_arguments('-D_POSIX_C_SOURCE=200809L', language: 'c')
add_project_link_arguments('-lm', '-pthread', language: 'c')

extra_warnings = [
    '-Wshadow',
    '-Wformat=2',
    '-Wconversion',
    '-Wsign-conversion',
    '-Wnull-dereference',
    '-Wdouble-promotion',
    '-Wstrict-prototypes',
    '-Wmissing-prototypes',
    '-Wold-style-definition',
    '-Wbad-function-cast',
    '-Wcast-align',
    '-Wcast-qual',
    '-Wfloat-equal',
    '-Wswitch-default',
    '-Wswitch-enum',
    '-Wundef',
    '-Wpointer-arith',
    '-Wvla',
    '-Wwrite-strings',
]

foreach warning : extra_warnings
    if cc.has_argument(warning)
        add_project_arguments(warning, language: 'c')
    endif
endforeach

sources = files('src/main.c')

regex_exe = executable('regex', sources)

test('basic match', regex_exe, args: ['test', 'test'])
test('no match', regex_exe, args: ['test', 'best'], should_fail: true)
test('wildcard', regex_exe, args: ['t.st', 'test'])

if get_option('buildtype') == 'debug'
    foreach test_name : [
        'basic match',
        'no match',
        'wildcard'
    ]
        test(
            test_name + ' with timeout',
            regex_exe,
            args: ['test', 'test'],
            timeout: 5,
        )
    endforeach
endif
