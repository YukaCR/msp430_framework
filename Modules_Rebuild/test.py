import matplotlib.pyplot as plt
exec(open("./result.dat").read())
plt.plot(range(0,DMA1SZ),data)
plt.show()