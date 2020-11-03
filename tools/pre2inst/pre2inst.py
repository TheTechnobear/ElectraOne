import json
import sys

pages = {}
overlays = {}
controlsets = {}


def main(argv): 
    print("converting : " + argv[0])
    eifdata = {'version' : 2}
    with open(argv[0]) as f:
      eprdata = json.load(f)
      parseGlobals(eprdata)

      createHeader(eifdata,eprdata)
      createCategories(eifdata, eprdata)
      createOverlays(eifdata, eprdata)
      createParameters(eifdata,eprdata)

      fn=argv[0].replace('epr','eif')

      outfile = open(fn, "w")
      outfile.write(json.dumps(eifdata, indent=4, sort_keys=True))
      outfile.close()

def parseGlobals(eprdata):
    for i in eprdata['pages'] : 
        pages[i['id']] = i['name']
        controlsets[i['id']] = []

    for i in eprdata['overlays'] : 
        overlays[i['id']] = i['items']

    for i in eprdata['groups']:
        pid = i['pageId']
        pname = i['name']
        controlsets[pid].append(pname)

def createHeader(eifdata,eprdata):
      eifdata['id'] = eprdata['name']
      eifdata['name'] = eprdata['name']
      eifdata['manufacturer']= eprdata['name']
      eifdata['manufacturerId']= eprdata['name']
      

def categoryId(pgId,csId):
    return pages[pgId]+'-'+controlsets[pgId][csId]


def createCategories(eifdata,eprdata):
    eifdata['categories'] = []
    for page in controlsets.keys():
        for i in  controlsets[page]:
            ids = pages[page]+'-'+i
            label = ids
            cs = { 'id' : ids , 'label' : label}
            eifdata['categories'].append(cs)


def createOverlays(eifdata,eprdata):
    eifdata['overlays'] = []
    for i in overlays.keys():
        o = eifdata['overlays']
        oe = { 'id' : i , 'name' : str(i), 'items' : overlays[i]}
        o.append(oe)


def createParameters(eifdata,eprdata):
    eifdata['parameters'] = []
    parameters = {}


    for i in eprdata['controls']:
        p = { }
        vals = i['values'][0]
        msg  = vals['message']
        pgId = i['pageId']
        csId = i['controlSetId'];
        pid = msg['parameterNumber'] 

        p['id'] = pid
        p['type'] = i['type']
        p['name'] = i['name']
        if 'min' in msg.keys(): 
            p['min'] = msg['min']
        if 'max' in msg.keys(): 
            p['max'] = msg['max']
        p['categoryId'] = categoryId(pgId,csId)
        if 'overlayId' in vals.keys(): 
            p['overlayId'] = vals['overlayId']
        p['msg'] = msg['type']
        if p['msg'] == 'sysex' : 
            p['data'] = msg['data']

        parameters[pid] = p

    for i in parameters.keys():
        eifdata['parameters'].append(parameters[i])


if __name__ == "__main__":
   main(sys.argv[1:])
