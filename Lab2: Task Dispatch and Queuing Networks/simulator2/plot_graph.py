# -*- coding: utf-8 -*-
"""
Created on Thu Oct 18 20:40:11 2018

@author: kiit
"""

import matplotlib.pyplot as plt

# Data for plotting
N = [1,2,3,4,5,6,7,8,9,10,11,20]
X = [3.04,4.82,5.97,6.78,7.35,7.65,8.06,8.29,8.47,8.62,8.73,9.02]
X1 = [3.82,5.84,7.01,7.69,8.12,8.38,8.59,8.72,8.82,8.87,8.92, 9.04]
X2 = [3.73,5.96,7.47,8.51,9.32,9.94,10.44,10.85,11.18,11.47,11.73,12.95]
X3 = [3.59,5.55,6.75,7.52,8.02,8.36,8.58,8.77,8.85,8.92,8.97,9.06]
X4 = [3.65,5.82,7.25,8.26,8.99,9.54,9.97,10.34,10.62,10.84,11.07,11.88]
fig, ax = plt.subplots()
ax.plot(N, X)
ax.plot(N, X1)
ax.plot(N, X2)
ax.plot(N, X3)
ax.plot(N, X4)
ax.set(xlabel='N',ylabel='X',title='X versus N')
ax.grid()

fig.savefig("/home/kiit/Desktop/simulator2/extra_credit.png")
plt.show()