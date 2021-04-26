# Simple converter from Squarp Pyramid definitions to Instrument file
# see https://squarp.community/c/def/11
# see https://electra.one
# GPL 3.0
# https://github.com/TheTechnobear/ElectraOne.git

import json
import sys

def main(argv):
  print("converting preset : " + argv[0])
  eifdata = {'version' : 2}
  with open(argv[0]) as f:
    lines = f.readlines()
    createHeader(eifdata,lines)
    createParameters(eifdata,lines)

    fn=argv[0].replace('.txt','.eif')
    print("writing instrument: " + fn)
    outfile = open(fn, "w")
    outfile.write(json.dumps(eifdata, indent=4, sort_keys=True))
    outfile.close()



def createHeader(eifdata,lines):
  s = lines[0].rstrip()
  fld, name = s.split(':')
  eifdata['id'] = name
  eifdata['name'] = name
  eifdata['manufacturer']= name
  eifdata['manufacturerId']= name
  eifdata['categories'] = []
  eifdata['overlays'] = []


def createParameters(eifdata,lines):
  # first lines are name, channel, out - ignore 
  idx = 3
  eifdata['parameters'] = []
  parameters = {}

  while idx < len(lines):
    s = lines[idx].rstrip()
    if len(s) > 0:
      vl = s.split(':')
      if vl > 1: 
        pid,name = vl
        p = { }
        p['id'] = pid
        p['type'] = 'fader'
        p['msg'] = 'cc7'
        p['name'] = name
        p['min'] = 0
        p['max'] = 127
        parameters[pid] = p
    idx += 1

  for i in parameters.keys():
    eifdata['parameters'].append(parameters[i])


if __name__ == "__main__":
  main(sys.argv[1:])
