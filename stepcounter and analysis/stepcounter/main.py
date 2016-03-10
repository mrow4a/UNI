__version__ = '3.0' #declare the app version. Will be used by buildozer

from kivy.app import App #for the main app
from kivy.uix.boxlayout import BoxLayout #the UI layout
from kivy.uix.label import Label #a label to show information
from plyer import accelerometer #object to read the accelerometer
from kivy.clock import Clock #clock to schedule a method
from kivy.uix.button import Button
import numpy as np

ButtonEnable=0
x=0.000
counter=0
frequency=0.000
data=[]
fs = 20.000       # sample rate, Hz
class UI(BoxLayout):#the app ui
        def __init__(self, **kwargs):
                super(UI, self).__init__(**kwargs)
                self.orientation='vertical'
                self.lblAcce = Label(text="ACCELEROMETER by mrow4a&maria!\n\nINFO for best performance:\n  1.Please make at least 10 steps\n  2.Measurement time at least 5s") #create a label at the center
                self.add_widget(self.lblAcce) #add the label at the screen
                try:
                        accelerometer.enable() #enable the accelerometer
                except:
                        self.lblAcce.text = "Failed to start accelerometer" #error
                
                self.StartStopButton = Button(ButtonText="Start")
                self.StartStopButton.text="Start"
                self.add_widget(self.StartStopButton)
                self.StartStopButton.bind(on_release=self.startstop_callback)

                
        
        def startstop_callback(self, instance):
                global ButtonEnable
                global x
                global data
                if ButtonEnable == 0 :
                        ButtonEnable = 1
                        self.StartStopButton.text="Stop"
                        Clock.schedule_interval(self.update, 1.0/20) #20 calls per second
                else :
                        ButtonEnable = 0
                        self.stepcalculation()
                        x=0
                        data=[]
                        self.StartStopButton.text="Start"
                        Clock.unschedule(self.update)

        def stepcalculation(self) :
                global frequency
                global data
                #data1 = np.genfromtxt('Slow1.csv', delimiter=',', skip_header=10,
                     #skip_footer=10, names=['t', 'x', 'y', 'z'])
                #data_z = data1['z'][:]
                data_z = data[:]
                data_x = np.ones(len(data_z))

                for i in range (1,len(data_z)):
                        data_x[i] = i*fs/len(data_z)
                dataz_fftb=np.fft.fft(data_z)

                dataz_ifft=np.fft.ifft(dataz_fftb[:])

                yb = self.cheby1_bandpass_filter1(data_z, 0.5, 5, fs, 10)
                y=yb
                dataz_fft=abs(np.fft.fft(yb))

                FC = np.argmax(dataz_fft[0:len(dataz_fft)/2])*fs/(len(dataz_fft))
                frequency=FC
                
                if FC < 1 :
                        low=0.5
                        high = 2
                        yfil=yb
                        y = self.cheby1_bandpass_filter1(yfil, low, high, fs, 9)
                elif FC < 1.4 :
                        low=0.8
                        high = 2.8
                        yfil=yb
                elif FC < 2:
                        low=FC-0.8
                        high = FC+0.8
                        yfil = self.cheby1_bandpass_filter1(yb, 0.8, high, fs, 9)
                        y = self.cheby1_bandpass_filter1(yfil, low, 5, fs, 9)
                else:
                        low=FC-0.5
                        high = FC+0.5
                        yfil = self.cheby1_bandpass_filter1(yb, 0.8, high, fs, 9)
                        y = self.cheby1_bandpass_filter1(yfil, low, 5, fs, 9)
                self.counter(y, FC)
                txt = "Frequency of step: %3.3f\nStep Counter: %3.f\nArray size: %4.f" %(frequency, counter, len(data)) 
                
                self.lblAcce.text = txt #add the correct text
        def update(self, dt):
                txt = ""
                global x
                global data
                x += 1.000
                y=accelerometer.acceleration[2]
                data.append(y)
                try:
                        txt = "Time:%2.4f\nAccelerometer:\nX = %2.4f\nY = %2.4f\nZ = %2.4f " %(x/fs,accelerometer.acceleration[0],accelerometer.acceleration[1],y)
                except:
                        txt = "Cannot read accelerometer!" #error
                self.lblAcce.text = txt #add the correct text
                
        def cheby1_bandpass1(self,low, high, fs, order):
                nyq = 0.5 * fs
                n_low = low / nyq
                n_high = high / nyq
                b, a = self.cheb11(order,0.2,80, [n_low, n_high], btype='band', analog=False)                    
                w, h = self.freqz1(b,a)    
                angles = np.unwrap(np.angle(h))    
                return b, a
        def freqz1(self, b, a=1, worN=None, whole=0, plot=None):
                b, a = map(np.atleast_1d, (b, a))
                if whole:
                        lastpoint = 2 * np.pi
                else:
                        lastpoint = np.pi
                if worN is None:
                        N = 512
                        w = np.linspace(0, lastpoint, N, endpoint=False)
                elif isinstance(worN, int):
                        N = worN
                        w = np.linspace(0, lastpoint, N, endpoint=False)
                else:
                        w = worN
                        w = np.atleast_1d(w)
                zm1 = np.exp(-1j * w)
                h = np.polyval(b[::-1], zm1) / np.polyval(a[::-1], zm1)
                if not plot is None:
                        plot(w, h)
                return w, h

        def cheby1_bandpass_filter1(self,data, low, high, fs, order):
                b, a = self.cheby1_bandpass1(low, high, fs, order=order)
                y = self.myFilter(b, a, data)
                return y

        
        def myFilter(self,b, a, X):
                n    = len(a)
                z = np.zeros(n)
                b = b / a[0]
                a = a / a[0]
                Y = np.zeros(len(X))
                for m in range (0,len(Y)):
                        Y[m] = b[0] * X[m] + z[0]
                        for i in range (1,n):
                                z[i - 1] = b[i] * X[m] + z[i] - a[i] * Y[m]
                
                #print "zafter"
                #print z       
                return Y
        
        def counter(self,y, FC):
                global counter
                count=0
                a=0
                b=0
                c=0
		cnt_stop=0
                if FC < 0.9 :
                        cnt_stop=1
		elif FC < 1.1 :
                        TH=0.4
                elif FC < 1.3 :
                        TH=0.5
                elif FC < 1.7 :
                        TH=0.6
                elif FC < 2 :
                        TH=0.8
                elif FC < 2.2 :
                        TH=1.2
                elif FC < 2.5 :
                        TH=1.5
                elif FC < 3 :
                        TH=2.5
                elif FC < 3.5 :
                        TH=3
                elif FC < 4.5 :
                        TH=3
		if cnt_stop==0 :        
                	for i in range (1,y.size-2):
                        	a=y[i]
                        	b=y[i+1]
                        	c=y[i+2]
                        	if a<b and b>c and b>TH:
                                	count = count + 1
                	counter=count
		elif cnt_stop==1 :
			counter=0
			cnt_stop=0
                
        def cheb11(self, N, rp, rs, Wn, btype='low', analog=False, output='ba'):
                return self.iirfilter1(N, Wn, rs=rs, rp=rp, btype=btype, analog=analog, output=output, ftype='cheby1')               
                
        def iirfilter1(self, N, Wn, rp, rs, btype, analog, ftype, output='ba'):  
                ftype, btype, output = [x.lower() for x in (ftype, btype, output)]
                Wn = np.asarray(Wn)
                btype = 'bandpass'
                if output not in ['ba', 'zpk']:
                        raise ValueError("'%s' is not a valid output form." % output)

                # Get analog lowpass prototype
                z, p, k = self.cheb1ap1(N, rp)
                # Pre-warp frequencies for digital filter design
                if not analog:
                        fs = 2.0
                        warped = 2 * fs * np.tan(np.pi * Wn / fs)
                else:
                        warped = Wn

                try:
                        bw = warped[1] - warped[0]
                        wo = np.sqrt(warped[0] * warped[1])
                except IndexError:
                        raise ValueError('Wn must specify start and stop frequencies')
                z, p, k = self.myzpklp2bp(z, p, k, wo=wo, bw=bw)

                # Find discrete equivalent if necessary
                if not analog:
                        z, p, k = self.myzpkbilinear(z, p, k, fs=fs)
                # Transform to proper out type (pole-zero, state-space, numer-denom)
                return self.myzpk2tf(z, p, k)

        def myzpklp2bp(self,z, p, k, wo=1.0, bw=1.0):
                z = np.atleast_1d(z)
                p = np.atleast_1d(p)
                wo = float(wo)
                bw = float(bw)
                degree = self.relative_degree(z, p)
                # Scale poles and zeros to desired bandwidth
                z_lp = z * bw/2
                p_lp = p * bw/2
                # Square root needs to produce complex result, not NaN
                z_lp = z_lp.astype(complex)
                p_lp = p_lp.astype(complex)
                # Duplicate poles and zeros and shift from baseband to +wo and -wo
                z_bp = np.concatenate((z_lp + np.sqrt(z_lp**2 - wo**2),
                z_lp - np.sqrt(z_lp**2 - wo**2)))
                p_bp = np.concatenate((p_lp + np.sqrt(p_lp**2 - wo**2),
                p_lp - np.sqrt(p_lp**2 - wo**2)))
                # Move degree zeros to origin, leaving degree zeros at infinity for BPF
                z_bp = np.append(z_bp, np.zeros(degree))
                # Cancel out gain change from frequency scaling
                k_bp = k * bw**degree
                return z_bp, p_bp, k_bp
        
        def myzpkbilinear(self, z, p, k, fs):
                z = np.atleast_1d(z)
                p = np.atleast_1d(p)
                degree = self.relative_degree(z, p)
                fs2 = 2*fs
                # Bilinear transform the poles and zeros
                z_z = (fs2 + z) / (fs2 - z)
                p_z = (fs2 + p) / (fs2 - p)
                # Any zeros that were at infinity get moved to the Nyquist frequency
                z_z = np.append(z_z, -np.ones(degree))
                # Compensate for gain change
                k_z = k * np.real(np.prod(fs2 - z) / np.prod(fs2 - p))
                return z_z, p_z, k_z

        def myzpk2tf(self, z, p, k):
                z = np.atleast_1d(z)
                k = np.atleast_1d(k)
                if len(z.shape) > 1:
                        temp = np.poly(z[0])
                        b = np.zeros((z.shape[0], z.shape[1] + 1), temp.dtype.char)
                        if len(k) == 1:
                                k = [k[0]] * z.shape[0]
                        for i in range(z.shape[0]):
                                b[i] = k[i] * poly(z[i])
                else:
                        b = k * np.poly(z)
                a = np.atleast_1d(np.poly(p))
                # Use real output if possible. Copied from numpy.poly, since
                # we can't depend on a specific version of numpy.
                if issubclass(b.dtype.type, np.complexfloating):
                        # if complex roots are all complex conjugates, the roots are real.
                        roots = np.asarray(z, complex)
                        pos_roots = np.compress(roots.imag > 0, roots)
                        neg_roots = np.conjugate(np.compress(roots.imag < 0, roots))
                        if len(pos_roots) == len(neg_roots):
                                if np.all(np.sort_complex(neg_roots) == np.sort_complex(pos_roots)):
                                        b = b.real.copy()
                if issubclass(a.dtype.type, np.complexfloating):
                        # if complex roots are all complex conjugates, the roots are real.
                        roots = np.asarray(p, complex)
                        pos_roots = np.compress(roots.imag > 0, roots)
                        neg_roots = np.conjugate(np.compress(roots.imag < 0, roots))
                        if len(pos_roots) == len(neg_roots):
                                if np.all(np.sort_complex(neg_roots) == np.sort_complex(pos_roots)):
                                        a = a.real.copy()
                return b, a

        def relative_degree(self,z, p):
                degree = len(p) - len(z)
                return degree
                                
        def cheb1ap1(self,N, rp):
                if abs(int(N)) != N:
                        raise ValueError("Filter order must be a nonnegative integer")
                elif N == 0:
                # Avoid divide-by-zero error
                # Even order filters have DC gain of -rp dB
                        return np.array([]), np.array([]), 10**(-rp/20)
                z = np.array([])
                # Ripple factor (epsilon)
                eps = np.sqrt(10 ** (0.1 * rp) - 1.0)
                mu = 1.0 / N * np.arcsinh(1 / eps)
                # Arrange poles in an ellipse on the left half of the S-plane
                m = np.arange(-N+1, N, 2)
                theta = np.pi * m / (2*N)
                p = -np.sinh(mu + 1j*theta)
                k = np.prod(-p, axis=0).real
                if N % 2 == 0:
                        k = k / np.sqrt((1 + eps * eps))
                return z, p, k
                        
class Accelerometer(App): #our app
        def build(self):
                ui = UI()# create the UI
                return ui #show it

Accelerometer().run() #start our app
