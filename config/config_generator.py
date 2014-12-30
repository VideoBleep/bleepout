#!/usr/bin/python

import json
import object_specs
import round_1
# import argparse
# import sys

def generate():
  appConfig = object_specs.BleepoutConfig()
  appConfig.roundConfigs.append(round_1.generate())
  return appConfig

def main():
  # parser = argparse.ArgumentParser(description='Generate configuration for Bleepout')
  # parser.add_argument('output', metavar='o', type=str)
  # parser.add_argument('round', metavar='r', type=str)
  # args = parser.parse_args(sys.argv)
  # main_impl(args.output, args.round)
  appConfig = generate()
  print json.dumps(appConfig.build(), sort_keys=True,
                   indent=2, separators=(',', ': '))

if __name__ == '__main__':
  main()


