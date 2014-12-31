#!/usr/bin/python

import json
import sys

def main():
  round = sys.argv[1].lower().strip()
  if round == "round1":
    import round_1
    config = round_1.generate()
  elif round == "round2":
    import round_2
    config = round_2.generate()
  else:
    config = None
  if config:
    print json.dumps(config, sort_keys=True,
                     indent=2, separators=(',', ': '))


if __name__ == '__main__':
  main()


