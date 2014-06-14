import matplotlib.pyplot as plt
from matplotlib import animation

f=open('results/result','r')
dats = {}
for line in open('results/result'):
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

def animate(i):
    plt.cla()
    plt.axis([0,1000,1000,0])
    plt.plot(dats[i]["x"],dats[i]["y"],'o')
    im = plt.imread("results/result-"+str(i)+".jpg")
    implot = plt.imshow(im)

fig =plt.figure()
anim = animation.FuncAnimation(fig,animate,frames=100)
anim.save('result.gif',writer='imagemagick',fps=5)
