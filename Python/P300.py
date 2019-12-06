#!/usr/bin/env python
# coding: utf-8

# In[52]:


#Client 

import socket
import pandas as pd
import matplotlib.pyplot as plt
import time
from scipy import signal
import numpy as np


# In[53]:


sock = socket.socket()
sock.connect(('localhost', 9090))
print('Connected')

def filterSignal(sig):
    t = np.linspace(0, 1, 200, False)  # 0.2 second
    sos1 = signal.butter(10, [45,55], 'bandstop', fs=1000, output='sos')
    sos2 = signal.butter(10, [95,105], 'bandstop', fs=1000, output='sos')
    sos3 = signal.butter(1, 0.57, 'high', fs=1000, output='sos')
    sos4 = signal.butter(2, 30, 'low', fs=1000, output='sos')
    filtered = signal.sosfilt(sos1, sig)
    filtered = signal.sosfilt(sos2, filtered)
    filtered = signal.sosfilt(sos3, filtered)
    filtered = signal.sosfilt(sos4, filtered)
    return filtered

def getData():
    data = sock.recv(1000000)
    print('got data: ', data)
    str_data = data.decode("utf-8")[:-1]
    A=list(map(str, str_data.split(';')))
    result_data = []
    for i in range(len(A)):
        B=list(map(float, A[i].split(',')))
        print(len(B))
        result_data.append(B)
    df = pd.DataFrame(result_data)
    df = df.transpose()
    print(len(df))
    df.columns = ["Marker", "Detector 1", "Detector 2", "Detector 3", "Detector 4", "Detector 5", "Detector 6", "Detector 7", "Detector 8"]
    df['Time'] = range(len(df))
    df['Detector 1'] = filterSignal(df['Detector 1'])
    df['Detector 2'] = filterSignal(df['Detector 2'])
    df['Detector 3'] = filterSignal(df['Detector 3'])
    df['Detector 4'] = filterSignal(df['Detector 4'])
    df['Detector 5'] = filterSignal(df['Detector 5'])
    df['Detector 6'] = filterSignal(df['Detector 6'])
    df['Detector 7'] = filterSignal(df['Detector 7'])
    df['Detector 8'] = filterSignal(df['Detector 8'])
    return df


def getEpoch(df):
    df = df[df['Marker'] != 0]
    df.reset_index(drop=True, inplace = True)
    time = df['Time'][0] + 100
    df = df[df['Time'] <= time]
    return df


def showGraph():
    dat1=[-1000,1000]
    fig1 = plt.figure()
    ax1 = fig1.add_subplot(111)
    Ln1, = ax1.plot(dat1)
    ax1.set_xlim([0,200])

    dat2=[-1000,1000]
    ax2 = fig1.add_subplot(111)
    Ln2, = ax2.plot(dat2)
    ax2.set_xlim([0,200])
    
    dat3=[-1000,1000]
    ax3 = fig1.add_subplot(111)
    Ln3, = ax3.plot(dat3)
    ax3.set_xlim([0,200])
    
    dat4=[-1000,1000]
    ax4 = fig1.add_subplot(111)
    Ln4, = ax4.plot(dat4)
    ax4.set_xlim([0,200])
    
    dat5=[-1000,1000]
    ax5 = fig1.add_subplot(111)
    Ln5, = ax5.plot(dat5)
    ax5.set_xlim([0,200])
    
    dat6=[-1000,1000]
    ax6 = fig1.add_subplot(111)
    Ln6, = ax6.plot(dat6)
    ax6.set_xlim([0,200])
    
    dat7=[-1000,1000]
    ax7 = fig1.add_subplot(111)
    Ln7, = ax7.plot(dat7)
    ax7.set_xlim([0,200])
    
    dat8=[-1000,1000]
    ax8 = fig1.add_subplot(111)
    Ln8, = ax8.plot(dat8)
    ax8.set_xlim([0,200])
    
    plt.ion()
    plt.show()
    i = 0
    df_full = pd.DataFrame()
    df_full['Marker'] = 0
    df_full['P300'] = 0
    
    while(True):
        i+=1
        df1 = getData()
        df2 = getData()
        df3 = getData()
        df4 = getData()
        df5 = getData()
        marker = df1['Marker'][100]
        df = (df1 + df2 + df3 + df4 + df5) / 5
        df['Marker'] = marker
        df.dropna(inplace=True)
        df = getEpoch(df)
        df_full = pd.concat([df_full, pd.DataFrame(df['Detector 1']).transpose()], ignore_index = True)
        df_full['Marker'][i-1] = marker
        #print(len(df['Detector 1']), len(df_full[0]))
        dat1 = df["Detector 1"]
        dat2 = df["Detector 2"]
        dat3 = df["Detector 3"]
        dat4 = df["Detector 4"]
        dat5 = df["Detector 5"]
        dat6 = df["Detector 6"]
        dat7 = df["Detector 7"]
        dat8 = df["Detector 8"]
        Ln1.set_ydata(dat1)
        Ln2.set_ydata(dat2)
        Ln3.set_ydata(dat3)
        Ln4.set_ydata(dat4)
        Ln5.set_ydata(dat5)
        Ln6.set_ydata(dat6)
        Ln7.set_ydata(dat7)
        Ln8.set_ydata(dat8)
        Ln1.set_xdata(df["Time"])
        Ln2.set_xdata(df["Time"])
        Ln3.set_xdata(df["Time"])
        Ln4.set_xdata(df["Time"])
        Ln5.set_xdata(df["Time"])
        Ln6.set_xdata(df["Time"])
        Ln7.set_xdata(df["Time"])
        Ln8.set_xdata(df["Time"])

        plt.pause(0.1)
        if (i == 5):
            #print(df_full)
            df_full.to_csv('data.csv', index = False)
            df_full.to_excel('data.xlsx', index = False)
            return


# In[63]:


showGraph()



# In[15]:


sock.close()

