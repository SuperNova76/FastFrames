#!/usr/bin/env python
# Copyright (C) 2002-2021 CERN for the benefit of the ATLAS collaboration
"""
Compile given list of python source files.
"""

import sys
import argparse
import traceback

parser = argparse.ArgumentParser(description=__doc__)

parser.add_argument('files', metavar='FILE', nargs='*',
                    help='python source filenames')

parser.add_argument('--exit-zero', action='store_true',
                    help='Exit with status code 0 even on errors.')

args = parser.parse_args()

error = False
for f in args.files:
   try:
      compile(open(f).read(), f, 'exec')
   except Exception:
      error = True
      traceback.print_exc(0)

sys.exit(0 if args.exit_zero else error)
