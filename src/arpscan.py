#!/usr/bin/python
import sys
import os
from subprocess import call
import subprocess


response = subprocess.Popen(['/usr/bin/arp-scan', '--interface=wlan0', '--localnet'],
                            stdout=subprocess.PIPE)
out, err = response.communicate()
print out

