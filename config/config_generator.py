#!/usr/bin/python

import json
import sys
import round_1
import round_2
import round_3
import round_4
import round_5
import islands_stage
import blobs

rounds = {
  "round1": round_1.generate,
  "round2": round_2.generate,
  "round3": round_3.generate,
  "round4": round_4.generate,
  "round5": round_5.generate,
  "islands": islands_stage.generate,
  "blobs": blobs.generate
}

def generateRound(name, outdir):
  name = name.lower().strip()
  gen = rounds[name]
  if not gen:
    raise RuntimeError("Unrecognized round: '" + name + "'")
  config = gen()
  filename = outdir.rstrip('/') + '/' + name + '.json'
  print 'generating round "%s" -> %s...' % (name, filename)
  with open(filename, 'w') as f:
    json.dump(config, f, sort_keys=True, indent=2, separators=(',', ': '))

def main():
  args = sys.argv[1:]
  outdir = args[0]
  if not outdir:
    print "usage: config_generator.py OUTPUT_DIR [round1] [round2] etc"
    exit(1)
  print 'outputting to directory: %s' % (outdir,)
  if len(args) == 1:
    for name in rounds:
      generateRound(name, outdir)
  else:
    for name in args[1:]:
      generateRound(name, outdir)

if __name__ == '__main__':
  main()


