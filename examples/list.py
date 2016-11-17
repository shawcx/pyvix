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
host = pyvix.Connect()
print('OK')

vms = host.Running()
if vms:
    print('Running virtual machines:')
    for vm in vms:
        print(' *', vm)
    print()
else:
    print('No running virtual machines')

if len(sys.argv) > 1:
    vm = host.Open(sys.argv[1])

    print(dir(vm))
    print()

    print(vm.IsRunning())
    print()

    vm.WaitForTools()
    vm.Login('user', 'password', pyvix.LOGIN_IN_GUEST_REQUIRE_INTERACTIVE_ENVIRONMENT)

    vm.Run('C:\\windows\\notepad.exe', '', False)

    tasklist = vm.TaskList()
    for task in tasklist:
        print('%d: %s: %s' % task)

    vm.Logout()

    vm.Close()

host.Disconnect()
