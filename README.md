# Signal-Analysis-and-Processing

This project focuses on processing and analyzing audio signals using LabWindows/CVI and Python. The project is divided into two stages: time-domain analysis and frequency-domain analysis, each with specific tasks and visual representations.

### Time-Domain Analysis (Panel 1)
* Load WAV File
* Signal Visualization: Display the signal on a Graph control (limited to 6 seconds if the file is longer than 10 seconds).
* Statistical Analysis: Min/Max values, mean, median, variance, and zero-crossings, histogram generation
* Signal Filtering:
  * Moving average filter (16 or 32 elements)
  * First-order filter with adjustable alpha parameter (0-1 range)
* Envelope Extraction: Using Hilbert Transform
* Skewness & Kurtosis Calculation
* Graph Navigation: "Prev" and "Next" buttons for viewing different time intervals.
* First Derivative Display

### Frequency-Domain Analysis (Panel 2)
* Spectrum Visualization: Display spectrum for a time window with adjustable size (1024, 2048, 4096 points).
* FFT Calculation: Perform and display the spectrum at each timer tick or with manual control.
* Power Spectrum & Peak Estimation
* Downsampling
* Filtering & Windowing:
  * Apply two types of windows (Rectangle and Hamming) and filters (Butterworth and Chebyshev I)
  * Save spectrum and signal before and after filtering
  * Save JPEG images with timestamps
