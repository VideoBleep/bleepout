#!/usr/bin/python

import json
import round_1
import sys

def main():
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


