import numpy as np
import matplotlib.pyplot as plt
from scipy.signal import butter, lfilter,cheby1
from scipy import signal
import math
import numpy
import scipy

TH=0.7
def cheby1_bandpass(low, high, fs, order):
    nyq = 0.5 * fs
    n_low = low / nyq
    n_high = high / nyq
    b, a = cheby1(order,0.2, [n_low, n_high], btype='band', analog=False)
    fig2 = plt.figure()
    plt.title('Digital filter frequency response')
    ax1 = fig2.add_subplot(211)
    w, h = signal.freqz(b,a)
    plt.plot(w/(math.pi*2)*fs, abs(h), 'b')
    plt.ylabel('Amplitude', color='b')
    plt.xlabel('Frequency [freq/sample]')
    ax2 = ax1.twinx()
    angles = np.unwrap(np.angle(h))
    plt.plot(w/(math.pi*2)*fs, angles, 'g')
    plt.ylabel('Angle (radians)', color='g')
    plt.grid()
    plt.axis('tight')
    
    
    
    plt.show()
    return b, a

def cheby1_bandpass_filter(data, low, high, fs, order):
    b, a = cheby1_bandpass(low, high, fs, order=order)
    y = lfilter(b, a, data)
    return y

def elliptic_bandpass(low, high, fs, order):
    nyq = 0.5 * fs
    n_low = low / nyq
    n_high = high / nyq
    b, a = signal.ellip(order,0.2,80, [n_low, n_high], btype='band', analog=False)
    fig2 = plt.figure()
    plt.title('Digital filter frequency response')
    ax1 = fig2.add_subplot(211)
    w, h = signal.freqz(b,a)
    plt.plot(w/(math.pi*2)*fs, abs(h), 'b')
    plt.ylabel('Amplitude', color='b')
    plt.xlabel('Frequency [freq/sample]')
    ax2 = ax1.twinx()
    angles = np.unwrap(np.angle(h))
    plt.plot(w/(math.pi*2)*fs, angles, 'g')
    plt.ylabel('Angle (radians)', color='g')
    plt.grid()
    plt.axis('tight')
    plt.show()
    return b, a

def elliptic_bandpass_filter(data, low, high, fs, order):
    b, a = elliptic_bandpass(low, high, fs, order=order)
    y = lfilter(b, a, data)
    return y
def counter(y):
    count=0
    a=0
    b=0
    c=0
    for i in range (1,data_z.size-2):
        a=y[i]
        b=y[i+1]
        c=y[i+2]
        if a<b and b>c and b>TH:
            count = count + 1
    
    print "Counter: %d" % count



# Filter requirements.
fs = 16.66       # sample rate, Hz
plt.close()
plt.close()
data = np.genfromtxt('C:\Users\mrow4a\Desktop\pedometer\Bieg1.csv', delimiter=',', skip_header=10,
                     skip_footer=10, names=['t', 'x', 'y', 'z'])
data_z = data['z'][:]

data_x = np.ones(data_z.size)
for i in range (1,data_z.size):
    data_x[i] = i*fs/data_z.size

dataz_fftb=np.fft.fft(data_z)

dataz_ifft=np.fft.ifft(dataz_fftb[:])

y = cheby1_bandpass_filter(data_z, 0.5, 4.5, fs, 10)

dataz_fft=np.fft.fft(y)

FC = np.argmax(dataz_fft[0:data_z.size/2])*fs/(data_z.size)

print  'FC %f' % FC
low=FC-0.3
high = FC+0.3
print  'low %f ' % low
print  'high %f ' % high
print "TH %f" %TH
y = cheby1_bandpass_filter(data_z, low, high, fs, 10)
counter(y)
"""y = cheby1_bandpass_filter(data_z, low, high, fs, 24)
counter(y)"""

dataz_fft=np.fft.fft(y)

fig1 = plt.figure()
ax1 = fig1.add_subplot(411)
ax1.plot(data_z[:], color='r', label='the data')

ax1 = fig1.add_subplot(412)
ax1.plot(data_x[1:data_z.size/2],abs(dataz_fftb[1:data_z.size/2]), color='r', label='the data')


ax1 = fig1.add_subplot(413)
ax1.plot(y, color='r', label='the data')

ax1 = fig1.add_subplot(414)
ax1.plot(data_x[1:data_z.size/2],abs(dataz_fft[1:data_z.size/2]), color='r', label='the data')

plt.show()
