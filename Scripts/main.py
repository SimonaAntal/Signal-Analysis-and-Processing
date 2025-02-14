import numpy as np
from scipy.signal import hilbert


from scipy.io import wavfile
samplerate, data = wavfile.read('FisiereWAV/10.wav')

#Selectia unui singur canal
if data.ndim > 1:
    data = data[:,0]

#Downsampling
downsampling = 4
data = data[::downsampling]
samplerate = samplerate//downsampling

wavFileInfo = open("waveInfo.txt", "w")
wavFileInfo.write(str(samplerate)+'\n')
wavFileInfo.write(str(data.size)+'\n')
wavFileInfo.close()

print(samplerate)
print(data.size)
print(data)

np.savetxt("waveData.txt", data, fmt="%2.0f")

#calculam anvelopa
analytic_signal = hilbert(data)
amplitude_envelope = np.abs(analytic_signal)

np.savetxt("envelope.txt", amplitude_envelope, fmt="%2.0f")

