project(
    'regex',
    'cpp',
    version: '0.1.0',
    default_options: [
        'c_std=c++23',
        'b_sanitize=address,undefined',
    ],
)

fmt_dep = dependency('fmt')
sources = files(
    'src/main.cpp'
)

regex_exe = executable(
    'regex',
    sources,
    dependencies: [fmt_dep],
    install: true
)

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
