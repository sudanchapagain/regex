project(
    'regex',
    'cpp',
    version: '0.1.0',
    default_options: [
        'c_std=c++23'
    ],
)

fmt_dep = dependency('fmt')
sources = files('src/main.cpp')

regex_exe = executable('regex', sources, dependencies: [fmt_dep], install: true)

test('literal exact', regex_exe, args: ['abc', 'abc'])
test('literal mismatch', regex_exe, args: ['abc', 'ab'], should_fail: true)
test('single char match', regex_exe, args: ['a', 'a'])
test('single char fail', regex_exe, args: ['a', 'b'], should_fail: true)
test('concat match', regex_exe, args: ['ab', 'ab'])
test('concat fail order', regex_exe, args: ['ab', 'ba'], should_fail: true)
test('union left', regex_exe, args: ['a|b', 'a'])
test('union right', regex_exe, args: ['a|b', 'b'])
test('union fail', regex_exe, args: ['a|b', 'c'], should_fail: true)
test('union chain a', regex_exe, args: ['a|b|c', 'a'])
test('union chain b', regex_exe, args: ['a|b|c', 'b'])
test('union chain c', regex_exe, args: ['a|b|c', 'c'])
test('union chain fail', regex_exe, args: ['a|b|c', 'd'], should_fail: true)
test('star empty', regex_exe, args: ['a*', ''])
test('star single', regex_exe, args: ['a*', 'a'])
test('star multiple', regex_exe, args: ['a*', 'aaaa'])
test('star fail', regex_exe, args: ['a*', 'b'], should_fail: true)
test('concat star 1', regex_exe, args: ['ab*', 'a'])
test('concat star 2', regex_exe, args: ['ab*', 'ab'])
test('concat star 3', regex_exe, args: ['ab*', 'abbb'])
test('concat star fail', regex_exe, args: ['ab*', 'b'], should_fail: true)
test('group match 1', regex_exe, args: ['a(b|c)', 'ab'])
test('group match 2', regex_exe, args: ['a(b|c)', 'ac'])
test('group fail', regex_exe, args: ['a(b|c)', 'a'], should_fail: true)
test('group star empty', regex_exe, args: ['a(b|c)*', 'a'])
test('group star b', regex_exe, args: ['a(b|c)*', 'ab'])
test('group star c', regex_exe, args: ['a(b|c)*', 'accc'])
test('group star mixed', regex_exe, args: ['a(b|c)*', 'abcbcbc'])
test('group star fail', regex_exe, args: ['a(b|c)*', 'ad'], should_fail: true)
test('nested group 1', regex_exe, args: ['(a|b)c', 'ac'])
test('nested group 2', regex_exe, args: ['(a|b)c', 'bc'])
test('nested group fail', regex_exe, args: ['(a|b)c', 'abc'], should_fail: true)
test('epsilon via star', regex_exe, args: ['(a|b)*', ''])
test('epsilon via star nonempty', regex_exe, args: ['(a|b)*', 'abba'])
test('full match required', regex_exe, args: ['ab', 'zab'], should_fail: true)
test('full match required 2', regex_exe, args: ['ab', 'abz'], should_fail: true)
test('missing closing paren', regex_exe, args: ['(ab', 'ab'], should_fail: true)

if get_option('buildtype') == 'debug'
    foreach test_name : [
        'literal exact',
        'literal mismatch',
        'single char match',
        'single char fail',
        'concat match',
        'concat fail order',
        'union left',
        'union right',
        'union fail',
        'union chain a',
        'union chain b',
        'union chain c',
        'union chain fail',
        'star empty',
        'star single',
        'star multiple',
        'star fail',
        'concat star 1',
        'concat star 2',
        'concat star 3',
        'concat star fail',
        'group match 1',
        'group match 2',
        'group fail',
        'group star empty',
        'group star b',
        'group star c',
        'group star mixed',
        'group star fail',
        'nested group 1',
        'nested group 2',
        'nested group fail',
        'epsilon via star',
        'epsilon via star nonempty',
        'full match required',
        'full match required 2',
        'missing closing paren',
    ]
        test(
            test_name + ' with timeout',
            regex_exe,
            args: ['test', 'test'],
            timeout: 5,
        )
    endforeach
endif
