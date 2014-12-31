#!/usr/bin/python

import json
import object_specs
import round_1
import sys
import argparse

def main():
  # parser = argparse.ArgumentParser(description='Generate configuration for Bleepout')
  # parser.add_argument('output', metavar='o', type=str)
  # parser.add_argument('round', metavar='r', type=str)
  # args = parser.parse_args(sys.argv)
  print ('argv: ', repr(sys.argv))
  # round = sys.ar
  round = sys.argv[1]
  if round == "Round1":
    config = round_1.generate()
  else:
    config = None
  if config:
    print json.dumps(config, sort_keys=True,
                     indent=2, separators=(',', ': '))


if __name__ == '__main__':
  main()


