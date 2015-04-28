#!/usr/bin/env python3
#
# The MIT License (MIT)
#
# Copyright (C) 2015 - Francois Doray <francois.pierre-doray@polymtl.ca>
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in
# all copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.

import argparse
import csv
import math
import pprint
import statistics
import numpy
import re

# Utility functions.
def GetFileValues(filename):
  values = []
  with open(filename, 'r') as filecontent:
    if 'overhead-babeltrace' in filename:
      for line in filecontent.readlines():
        match = re.match(r'(.*)real\s0m(.*)s.*', line, re.M|re.I)
        if (match):
          values.append(eval(match.group(2)))
    if 'overhead-find' in filename:
      for line in filecontent.readlines():
        match = re.match(r'(.*)real\s0m(.*)s.*', line, re.M|re.I)
        if (match):
          values.append(eval(match.group(2)))
    elif 'overhead-cpu' in filename:
      for line in filecontent.readlines():
        match = re.match(r'(.*)total time:\s*(.*)s.*', line, re.M|re.I)
        if (match):
          values.append(eval(match.group(2)))
    elif 'overhead-lighttpd' in filename:
      for line in filecontent.readlines():
        match = re.match(r'(.*)Time taken for tests:\s*(.*) seconds.*', line, re.M|re.I)
        if (match):
          values.append(eval(match.group(2)))
    else:
      first = True
      reader = csv.reader(filecontent)
      for row in reader:
        if first:
          first = False
          continue
        v = eval(row[0])
        if 'overhead-mongo' in filename:
          v = v / 1000000000
        values.append(v)
  return values

def GetXYFileValues(filename):
  x = []
  y = []
  with open(filename, 'r') as filecontent:
    first = True
    reader = csv.reader(filecontent)
    for row in reader:
      if first:
        first = False
        continue
      y.append(eval(row[0]))
      x.append(eval(row[1]))
  return {'x': x, 'y': y} 

def GetXYFileStdDev(filename):
  values = GetXYFileValues(filename)
  grouped = {}
  for i in range(0, len(values['x'])):
    if values['x'][i] not in grouped:
      grouped[values['x'][i]] = []
    grouped[values['x'][i]].append(values['y'][i])
  total_stddev = 0
  for x, v in grouped.items():
    stddev = math.sqrt(statistics.variance(v))
    if stddev > total_stddev:
      total_stddev = stddev
  return stddev

def GetFileMean(filename):
  values = GetFileValues(filename)
  return statistics.mean(values)

def GetFileStdDev(filename):
  values = GetFileValues(filename)
  return math.sqrt(statistics.variance(values))

def RemoveBase(values, base, numactions):
  values[:] = [(value - base) / numactions for value in values]
  return values

# Experiment: GETPID
def SummaryGetPid(results_dir):
  NUM_ACTIONS = 100000000
  base_mean = GetFileMean(results_dir + '/getpid-base.csv')
  emptyprobes_mean = GetFileMean(results_dir + '/getpid-emptyprobes.csv')
  bookkeeping_mean = GetFileMean(results_dir + '/getpid-bookkeeping.csv')
  signal_mean = GetFileMean(results_dir + '/getpid-signal.csv')
  base_stddev = GetFileStdDev(results_dir + '/getpid-base.csv')
  emptyprobes_stddev = GetFileStdDev(results_dir + '/getpid-emptyprobes.csv')
  bookkeeping_stddev = GetFileStdDev(results_dir + '/getpid-bookkeeping.csv')
  signal_stddev = GetFileStdDev(results_dir + '/getpid-signal.csv')

  print('EXPERIMENT: GETPID (base stddev: {0:.1f})'.format(base_stddev / base_mean))
  print('Empty probes: {0:.0f} ns (stddev={1:.1f})'.format((emptyprobes_mean - base_mean) / NUM_ACTIONS,
    100 * emptyprobes_stddev / emptyprobes_mean))
  print('Bookkeeping: {0:.0f} ns (stddev={1:.1f})'.format((bookkeeping_mean - base_mean) / NUM_ACTIONS,
    100 * bookkeeping_stddev / bookkeeping_mean))
  print('Signal: {0:.0f} ns (stddev={1:.1f})'.format((signal_mean - base_mean) / NUM_ACTIONS,
    100 * signal_stddev / signal_mean))
  print()

# Experiment: UST
def SummaryUST(results_dir):
  NUM_ACTIONS = 100000000
  base_mean = GetFileMean(results_dir + '/ust-base.csv')
  tracepoint_mean = GetFileMean(results_dir + '/ust-tracepoint.csv')
  tracepoint_stddev = GetFileStdDev(results_dir + '/ust-tracepoint.csv')

  print('EXPERIMENT: UST TRACEPOINT')
  print('UST event: {0:.0f} ns (stddev: {1:.1f})'.format((tracepoint_mean - base_mean) / NUM_ACTIONS,
    100 * tracepoint_stddev / tracepoint_mean))
  print('')

# Experiment: LIBUNWIND
def SummaryLibunwind(results_dir):
  NUM_ACTIONS = 1

  base = GetFileMean(results_dir + '/libunwind-base.csv')
  minregs = GetXYFileValues(results_dir + '/libunwind-optimal-withcache.csv')
  minregs_nocache = GetXYFileValues(results_dir + '/libunwind-optimal-nocache.csv')
  master_nocache = GetXYFileValues(results_dir + '/libunwind-nocache.csv')

  base_stddev = GetXYFileStdDev(results_dir + '/libunwind-base.csv')
  minregs_stddev = GetXYFileStdDev(results_dir + '/libunwind-optimal-withcache.csv')
  minregs_nocache_stddev = GetXYFileStdDev(results_dir + '/libunwind-optimal-nocache.csv')
  master_nocache_stddev = GetXYFileStdDev(results_dir + '/libunwind-nocache.csv')
  
  minregs['y'] = RemoveBase(minregs['y'], base, NUM_ACTIONS)
  minregs_nocache['y'] = RemoveBase(minregs_nocache['y'], base, NUM_ACTIONS)
  master_nocache['y'] = RemoveBase(master_nocache['y'], base, NUM_ACTIONS)

  minregs['x'] = RemoveBase(minregs['x'], 0, 1)
  minregs_nocache['x'] = RemoveBase(minregs_nocache['x'], 0, 1)
  master_nocache['x'] = RemoveBase(master_nocache['x'], 0, 1)

  minregs_reg = numpy.polyfit(minregs['x'], minregs['y'], 1)
  minregs_nocache_reg = numpy.polyfit(minregs_nocache['x'], minregs_nocache['y'], 1)
  master_nocache_reg = numpy.polyfit(master_nocache['x'], master_nocache['y'], 1)

  print('EXPERIMENT: LIBUNWIND')
  print('Minimal regs, with cache: frame = {0:.0f} ns, base = {1:.0f} ns, stddev={2:.0f} ns'.format(minregs_reg[0], minregs_reg[1], minregs_stddev))
  print('Minimal regs, no cache: frame = {0:.0f} ns, base = {1:.0f} ns, stddev={2:.0f} ns'.format(minregs_nocache_reg[0], minregs_nocache_reg[1], minregs_nocache_stddev))
  print('Master, no cache : frame = {0:.0f} ns, base = {1:.0f} ns, stddev={2:.0f} ns'.format(master_nocache_reg[0], master_nocache_reg[1], master_nocache_stddev))
  print()

# Experiment: OVERHEAD
def SummaryOverhead(results_dir):
  APPS = ['cpu', 'babeltrace', 'find', 'mongo']
  TESTS = ['nothing', 'kernel', 'notracing-cpusyscalls', 'ust-cpusyscalls', 'critical-cpusyscalls']
  FANCY_TESTS = ['Base                                   ',
                 'Traditionnal tracing                   ',
                 'Generating stack events / No tracing   ',
                 'Tracing stack events                   ',
                 'Tracing stack and critical path events ']

  print('EXPERIMENT: OVERHEAD')
  results = {}

  for app in APPS:
    base = GetFileMean(results_dir + '/overhead-' + app + '/nothing.csv')
    results[app] = {}
    for test in TESTS:
      mean = GetFileMean(results_dir + '/overhead-' + app + '/' + test + '.csv')
      stddev = GetFileStdDev(results_dir + '/overhead-' + app + '/' + test + '.csv')
      overhead = ((mean / base) - 1) * 100
      results[app][test] = {'mean': mean, 'stddev': stddev, 'overhead': overhead}

  print('TABLE: TIME')
  for test_i in range(0, len(TESTS)):
    line = [FANCY_TESTS[test_i]]
    for app in APPS:
      line.append(results[app][TESTS[test_i]]['mean'])
      line.append(results[app][TESTS[test_i]]['stddev'])
    print('{0}& {1:.2f} & {2:.2f} & {3:.2f} & {4:.2f} & {5:.2f} & {6:.2f} & {7:.2f} & {8:.2f} \\\\\\hline'.format(*line))
  print()

  print('TABLE: PERCENT')
  for test_i in range(0, len(TESTS)):
    line = [FANCY_TESTS[test_i]]
    for app in APPS:
      line.append(results[app][TESTS[test_i]]['overhead'])
    print('{0}& {1:.1f} & {2:.1f} & {3:.1f} & {4:.1f} \\\\\\hline'.format(*line))
  print()

if __name__ == "__main__":
  parser = argparse.ArgumentParser(description='Generate summary from experimental results.')
  parser.add_argument('results', metavar="<path/to/results>", help='Path to results directory.')
  args = parser.parse_args()
  
  SummaryGetPid(args.results)
  SummaryUST(args.results)
  SummaryLibunwind(args.results)
  SummaryOverhead(args.results)
