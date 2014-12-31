#!/usr/bin/python

import json
import sys
import round_1
import round_2
import round_3

rounds = {
  "round1": round_1.generate,
  "round2": round_2.generate,
  "round3": round_3.generate
}

def generateRound(name, filename):
  gen = rounds[name]
  if not gen:
    raise RuntimeError("Unrecognized round: '" + name + "'")
  config = gen()
  if filename:
    json.dump(config, file(filename), sort_keys=True, indent=2, separators=(',', ': '))
  else:
    print json.dumps(config, sort_keys=True, indent=2, separators=(',', ': '))

def main():
  round = sys.argv[1].lower().strip()
  generateRound(round, None)


if __name__ == '__main__':
  main()


