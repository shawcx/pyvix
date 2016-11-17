#!/usr/bin/env python3

import sys
import argparse
import logging

try:
    import pyvix
except ImportError:
    sys.stderr.write('Missing Pyvix module\n')
    sys.exit(-1)

print('Connecting...', end=' ')
host = pyvix.connect()
print('OK')

vms = host.running()
if vms:
    print('Running virtual machines:')
    for vm in vms:
        print(' *', vm)
    print()
else:
    print('No running virtual machines')

if len(sys.argv) > 1:
    vm = host.open(sys.argv[1])

    print(dir(vm))
    print()

    print(vm.isrunning())
    print()

    vm.waitfortools()
    vm.login('user', 'password', pyvix.LOGIN_IN_GUEST_REQUIRE_INTERACTIVE_ENVIRONMENT)

    vm.run('C:\\windows\\notepad.exe', '', False)

    tasklist = vm.tasklist()
    for task in tasklist:
        print('%d: %s: %s' % task)

    vm.logout()

    vm.close()

host.disconnect()
