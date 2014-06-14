import matplotlib.pyplot as plt

f=open('result','r')
dats = {}
for line in open('result'):
    line = f.readline()
    data = line.split("|")
    [x,y] = data[1].split(",")
    loop = (int)(data[2].split(",")[0])
    if not loop in dats:
        dats[loop]={}
        dats[loop]["x"]=[]
        dats[loop]["y"]=[]
    dats[loop]["x"].append(int(x))
    dats[loop]["y"].append(int(y))

for i in dats.keys():
    fig=plt.figure()
    plt.axis([0,1000,0,1000])
    plt.plot(dats[i]["x"],dats[i]["y"],'o')
    fig.savefig('figures/'+str(i).zfill(3)+'.png')
    plt.close(fig)
