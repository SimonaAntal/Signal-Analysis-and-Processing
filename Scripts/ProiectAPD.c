#include <utility.h>
#include <formatio.h>
#include <ansi_c.h>
#include <cvirte.h>		
#include <userint.h>
#include <analysis.h>
#include "ProiectAPD.h"

// Constants
#define PI 3.141592653589793

#define SAMPLE_RATE		0
#define NPOINTS			1

#define BUTTERWORTH 	0
#define CHEBYSHEV 		1

#define RAWDATASPECTRUM 0
#define FILTERSPECTRUM 	1

//----------------------------------------------------------------------
// Functions
double* Derivative(double* data, int n);
double* AlphaFilter(double* data, int n, double alpha);
double* MedFilter(double* data, int n, int elem);
void Activate(void);
double* ComputePowerSpectrum(double inputArray[], unsigned int start, unsigned int numberOfElements, double samplingRate, 
						 double *frequencyInterval, double *powerPeak, double *freqPeak);
double* GetWindowCoefficients(int windowOption, int numberOfElements);

//----------------------------------------------------------------------
// Global variables
int waveInfo[2]; //waveInfo[0] = sampleRate
				 //waveInfo[1] = number of elements
double sampleRate = 0.0;
int npoints = 0;

//----------------------------------------------------------------------
//PANELS
static int panelHandle;
static int freqHandle;

//PANEL 1
double *waveData = 0;
double *envelope = 0;
double *derivate = 0;
double *filterData = 0;
char fileName[80];
int bitmapID;
int deriv = -1;

int nIntervals = 10;
ssize_t *histogram = 0;
double *axisArray = 0;

int filterOption = BUTTERWORTH;
int spectrumOption = RAWDATASPECTRUM;

int startSec = 0, stopSec = 1, stopPoint = 0;

//PANEL 2
int rawSignalPlot = -1;
int filteredSignalPlot = -1;
int freqfilterOption = 0;
int windowOption = 0;

double *filteredSignal = 0;
double* windowCoeficients = 0;

int numPoints = 1024;
int startWindow = 0;
double freqPeak = 0, powerPeak = 0, df = 0;

//----------------------------------------------------------------------

int main (int argc, char *argv[])
{
	if (InitCVIRTE (0, argv, 0) == 0)
		return -1;	/* out of memory */
	if ((panelHandle = LoadPanel (0, "ProiectAPD.uir", PANEL)) < 0)
		return -1;
	if ((freqHandle = LoadPanel (0, "ProiectAPD.uir", PANEL_FREQ)) < 0)
		return -1;
	DisplayPanel (panelHandle);
	RunUserInterface ();
	DiscardPanel (panelHandle);
	return 0;
}

int CVICALLBACK SwitchPanel (int panel, int control, int event,
							 void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			if (panel == panelHandle){
				SetCtrlVal(freqHandle, PANEL_FREQ_IDC_SWITCH_PANEL, 1);
				HidePanel(panelHandle);
				DisplayPanel(freqHandle);
			}else{
				SetCtrlVal(panelHandle, PANEL_IDC_SWITCH_PANEL, 0);
				HidePanel(freqHandle);
				DisplayPanel(panelHandle);
			}
			break;
	}
	return 0;
}


void Activate(void){
	//PANEL 1
	SetCtrlAttribute(panelHandle, PANEL_GRAPH_HISTOGRAM, ATTR_DIMMED, 0);
			
	SetCtrlAttribute(panelHandle, PANEL_MIN_NUMERIC, ATTR_DIMMED, 0);
	SetCtrlAttribute(panelHandle, PANEL_MIN_INDEX_NUMERIC, ATTR_DIMMED, 0);
	SetCtrlAttribute(panelHandle, PANEL_MAX_NUMERIC, ATTR_DIMMED, 0);
	SetCtrlAttribute(panelHandle, PANEL_MAX_INDEX_NUMERIC, ATTR_DIMMED, 0);
	SetCtrlAttribute(panelHandle, PANEL_MEAN_NUMERIC, ATTR_DIMMED, 0);
	SetCtrlAttribute(panelHandle, PANEL_MEDIAN_NUMERIC, ATTR_DIMMED, 0);
	SetCtrlAttribute(panelHandle, PANEL_VARIANCE_NUMERIC, ATTR_DIMMED, 0);
	SetCtrlAttribute(panelHandle, PANEL_MIN_NUMERIC, ATTR_DIMMED, 0);
	SetCtrlAttribute(panelHandle, PANEL_ZERO_NUMERIC, ATTR_DIMMED, 0);
	
	SetCtrlAttribute(panelHandle, PANEL_FILTER_RING, ATTR_DIMMED, 0);
	SetCtrlAttribute(panelHandle, PANEL_APPLY_FILTER_BUTTON, ATTR_DIMMED, 0);
	SetCtrlAttribute(panelHandle, PANEL_MEDIAN_VALUES_RING, ATTR_DIMMED, 0);
	
	SetCtrlAttribute(panelHandle, PANEL_SKEWNESS_NUMERIC, ATTR_DIMMED, 0);
	SetCtrlAttribute(panelHandle, PANEL_KURTOSIS_NUMERIC, ATTR_DIMMED, 0);
	
	SetCtrlAttribute(panelHandle, PANEL_INTERVAL, ATTR_DIMMED, 0);
	SetCtrlAttribute(panelHandle, PANEL_PREV_BUTTON, ATTR_DIMMED, 0);
	SetCtrlAttribute(panelHandle, PANEL_NEXT_BUTTON, ATTR_DIMMED, 0);
	SetCtrlAttribute(panelHandle, PANEL_START_SEC_NUMERIC, ATTR_DIMMED, 0);
	SetCtrlAttribute(panelHandle, PANEL_STOP_SEC_NUMERIC, ATTR_DIMMED, 0);
	
	SetCtrlAttribute(panelHandle, PANEL_DERIVATE_BUTTON, ATTR_DIMMED, 0);
	
	//PANEL 2
	SetCtrlAttribute(freqHandle, PANEL_FREQ_SIGNALS_TEXT, ATTR_DIMMED, 0);
	SetCtrlAttribute(freqHandle, PANEL_FREQ_CHECKBOX_RAW_SIGNAL, ATTR_DIMMED, 0);
	
	SetCtrlAttribute(freqHandle, PANEL_FREQ_FILTER_RING, ATTR_DIMMED, 0);
	SetCtrlAttribute(freqHandle, PANEL_FREQ_APPLY_FILTER_BUTTON, ATTR_DIMMED, 0);
	
	SetCtrlAttribute(freqHandle, PANEL_FREQ_SPECTRUM_TEXT, ATTR_DIMMED, 0);
	SetCtrlAttribute(freqHandle, PANEL_FREQ_WINDOW_OPTION_RING, ATTR_DIMMED, 0);
	SetCtrlAttribute(freqHandle, PANEL_FREQ_NUM_POINTS_NUMERIC, ATTR_DIMMED, 0);
	SetCtrlAttribute(freqHandle, PANEL_FREQ_FREQ_PEAK_NUMERIC, ATTR_DIMMED, 0);
	SetCtrlAttribute(freqHandle, PANEL_FREQ_POWER_PEAK_NUMERIC, ATTR_DIMMED, 0);
	SetCtrlAttribute(freqHandle, PANEL_FREQ_START_SPECTRUM, ATTR_DIMMED, 0);
	
	SetCtrlAttribute(freqHandle, PANEL_FREQ_FREQ_VAL_NUMERIC, ATTR_DIMMED, 0);
}

double* Derivative(double* data, int n){
	int i;
	
	if (derivate)
		free(derivate);
	
	derivate = (double *) calloc(npoints, sizeof(double));
	for (i = 0; i<n-1; ++i){
		derivate[i] = (waveData[i+1] - waveData[i]); // /i+1-i;
	}
	derivate[npoints-1] = waveData[npoints-1];
	
	return derivate;
}

int CVICALLBACK Exit (int panel, int control, int event,
					  void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			// Dealocam memoria
			
			//PANEL 1
			if (waveData)
				free(waveData);
			if (envelope)
				free(envelope);
			if (histogram)
				free(histogram);
			if (axisArray)
				free(axisArray);
			if (derivate)
				free(derivate);
			
			//PANEL 2
			if (filteredSignal)
				free(filteredSignal);
			if (windowCoeficients)
				free(windowCoeficients);
			QuitUserInterface (0);
			break;
	}
	return 0;
}

int CVICALLBACK ExitPanel (int panel, int event, void *callbackData,
						   int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_GOT_FOCUS:

			break;
		case EVENT_LOST_FOCUS:

			break;
		case EVENT_CLOSE:
			// Dealocam memoria
			if (waveData)
				free(waveData);
			if (envelope)
				free(envelope);
			if (histogram)
				free(histogram);
			if (axisArray)
				free(axisArray);
			if (derivate)
				free(derivate);
			
			QuitUserInterface (0);
			break;
	}
	return 0;
}

int CVICALLBACK LoadButton (int panel, int control, int event,
							void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			double min, max, mean, median, variance, skewness, kurtosis;
			int minIndex, maxIndex;
			int i, zeroCount = 0;
			
			//executa script python pentru conversia unui fisierului .wav in .txt
			//LaunchExecutable("C:\\Users\\antal\\AppData\\Local\\Programs\\Python\\Python312\\python.exe main.py");
			
			//astept sa fie generate cele doua fisiere (modificati timpul daca este necesar)
			//Delay(2);
			
			//Setam dimmed off
			Activate();
			
			//incarc informatiile privind rata de esantionare si numarul de valori
			FileToArray("waveInfo.txt", waveInfo, VAL_INTEGER, 2, 1, VAL_GROUPS_TOGETHER, VAL_GROUPS_AS_COLUMNS, VAL_ASCII);
			sampleRate = waveInfo[SAMPLE_RATE];
			npoints = waveInfo[NPOINTS];
			
			// Daca semnalul e mai mare de 6 secunde, luam doar primele 6
			if (npoints/sampleRate > 6){
				npoints = sampleRate * 6;
			}
			
			//alocare memorie pentru numarul de puncte
			if (waveData){
				free(waveData);
			}
			if (envelope){
				free(envelope);
			}
			
			waveData = (double *) calloc(npoints, sizeof(double));
			envelope = (double *) calloc(npoints, sizeof(double));
			
			//incarcare din fisierul .txt in memorie (vector)
			FileToArray("waveData.txt", waveData, VAL_DOUBLE, npoints, 1, VAL_GROUPS_TOGETHER, VAL_GROUPS_AS_COLUMNS, VAL_ASCII);
			
			//incarcare anvelopa in memorie
			FileToArray("envelope.txt", envelope, VAL_DOUBLE, npoints, 1, VAL_GROUPS_TOGETHER, VAL_GROUPS_AS_COLUMNS, VAL_ASCII);
			
			// Afisare semnal si anvelopa pe graf
			PlotY(panelHandle, PANEL_GRAPH_RAW_DATA, waveData, npoints, VAL_DOUBLE, VAL_THIN_LINE, VAL_EMPTY_SQUARE, VAL_SOLID, VAL_CONNECTED_POINTS, VAL_RED);
			PlotY(panelHandle, PANEL_GRAPH_RAW_DATA, envelope, npoints, VAL_DOUBLE, VAL_THIN_LINE, VAL_EMPTY_SQUARE, VAL_SOLID, VAL_CONNECTED_POINTS, VAL_DK_BLUE);
			SetAxisRange(panelHandle, PANEL_GRAPH_RAW_DATA, VAL_MANUAL, 0, sampleRate, VAL_AUTOSCALE, 0, 0);
			
			// Salvare secunda curenta
			sprintf(fileName, "ProiectAPD_grafic_raw_data.jpg"); //generare nume fisier
			GetCtrlDisplayBitmap(panel, PANEL_GRAPH_RAW_DATA, 1, &bitmapID);
			SaveBitmapToJPEGFile(bitmapID, fileName, JPEG_PROGRESSIVE, 100);
			DiscardBitmap(bitmapID);
			
			// Calcul min, max, mean, median, dispersion
			MaxMin1D(waveData, npoints, &max, &maxIndex, &min, &minIndex);
			Mean(waveData, npoints, &mean);
			Median(waveData, npoints, &median);
			Variance(waveData, npoints, &mean, &variance);
			
			//calcul asimetrie si aplatizare
			Moment(waveData+5000, 256, 3, &skewness);
			Moment(waveData+5000, 256, 4, &kurtosis);
			
			//aflare zeroCount
			for (i=0; i<npoints-1; ++i){
				if (waveData[i] == 0 || (waveData[i]>0 &&waveData[i+1]<0) || (waveData[i]<0 &&waveData[i+1]>0)){
					zeroCount++;	
				}
			}
			
			// Setam valorile calculate
			SetCtrlVal(panel, PANEL_MIN_NUMERIC, min);
			SetCtrlVal(panel, PANEL_MIN_INDEX_NUMERIC, minIndex);
			SetCtrlVal(panel, PANEL_MAX_NUMERIC, max);
			SetCtrlVal(panel, PANEL_MAX_INDEX_NUMERIC, maxIndex);
			SetCtrlVal(panel, PANEL_MEAN_NUMERIC, mean);
			SetCtrlVal(panel, PANEL_MEDIAN_NUMERIC, median);
			SetCtrlVal(panel, PANEL_VARIANCE_NUMERIC, variance);
			SetCtrlVal(panel, PANEL_ZERO_NUMERIC, zeroCount);
			SetCtrlVal(panel, PANEL_SKEWNESS_NUMERIC, skewness);
			SetCtrlVal(panel, PANEL_KURTOSIS_NUMERIC, kurtosis);
			
			
			// HISTOGRAMA	
			if (histogram){
				free(histogram);
			}
			if (axisArray){
				free(axisArray);
			}
			
			histogram = (ssize_t *) calloc(nIntervals, sizeof(ssize_t));
			axisArray = (double *) calloc(nIntervals, sizeof(double));
			
			Histogram(waveData, npoints, min, max, histogram, axisArray, nIntervals);
			PlotXY(panel, PANEL_GRAPH_HISTOGRAM, axisArray, histogram, nIntervals, VAL_DOUBLE, VAL_INTEGER, VAL_VERTICAL_BAR, VAL_SOLID_CIRCLE, VAL_DASH, VAL_CONNECTED_POINTS , VAL_MAGENTA);
			
			break;
	}
	return 0;
}

int CVICALLBACK Filter (int panel, int control, int event,
						void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			GetCtrlVal(panel, PANEL_FILTER_RING, &filterOption);
			
			// Se afiseaza caseta corespunzatoare filtrului curent
			if (filterOption == 1){ // Filtrare prin el de or I
				SetCtrlAttribute(panel, PANEL_MEDIAN_VALUES_RING, ATTR_VISIBLE, 0);
				SetCtrlAttribute(panel, PANEL_ALPHA_NUMERIC, ATTR_VISIBLE, 1);
			}else{ // Filtrate prin mediere
				SetCtrlAttribute(panel, PANEL_ALPHA_NUMERIC, ATTR_VISIBLE, 0);
				SetCtrlAttribute(panel, PANEL_MEDIAN_VALUES_RING, ATTR_VISIBLE, 1);
			}
			
			break;
	}
	return 0;
}

int CVICALLBACK ApplyFilter (int panel, int control, int event,
							 void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		double alpha;
		int option;
		
		case EVENT_COMMIT:
			if (filterData){
				free(filterData);
			}
				
			SetCtrlAttribute(panel, PANEL_GRAPH_FILTERED_DATA, ATTR_DIMMED, 0);
			DeleteGraphPlot(panel, PANEL_GRAPH_FILTERED_DATA, -1, VAL_IMMEDIATE_DRAW);
			
			// Aplicam filtrul selectat
			if (filterOption == 1){ 
				GetCtrlVal(panel, PANEL_ALPHA_NUMERIC, &alpha);
				filterData = AlphaFilter(waveData, npoints, alpha);
			}
			else
			{				
				GetCtrlVal(panel, PANEL_APPLY_FILTER_BUTTON, &option);
				filterData = MedFilter(waveData, npoints, option==0?16:32);
			}
			
			// Afisam filtrul
			PlotY(panel, PANEL_GRAPH_FILTERED_DATA, filterData, npoints, VAL_DOUBLE, VAL_THIN_LINE, VAL_EMPTY_SQUARE, VAL_SOLID, VAL_CONNECTED_POINTS, VAL_RED);
			if (stopPoint == 0){ // Nu a fost initializat inca 
				stopPoint = stopSec*sampleRate;
			}
			SetAxisRange(panelHandle, PANEL_GRAPH_FILTERED_DATA, VAL_MANUAL, sampleRate*startSec, stopPoint, VAL_AUTOSCALE, 0, 0);
			
			// Salvare secunda curenta
			sprintf(fileName, "ProiectAPD_grafic_filtered_data.jpg"); //generare nume fisier
			GetCtrlDisplayBitmap(panel, PANEL_GRAPH_FILTERED_DATA, 1, &bitmapID);
			SaveBitmapToJPEGFile(bitmapID, fileName, JPEG_PROGRESSIVE, 100);
			DiscardBitmap(bitmapID);
			break;
	}
	return 0;
}

double* AlphaFilter(double* data, int n, double alpha){
	int i;
	double* rez = (double*) calloc(n, sizeof(double));
	
	rez[0] = data[0];
	for (i = 1; i<n; ++i){
		rez[i] = (1-alpha)*rez[i-1] + alpha*data[i];
	}
	
	return rez;
}


double* MedFilter(double* data, int n, int elem){
	int i;
	double sum = 0;
	
	double* rez = (double*) calloc(n, sizeof(double));
	
	for (i = 0; i<elem; ++i){
		sum += data[i];
		rez[i] = sum/(i+1);
	}
	for (i = elem; i<n; ++i){
		sum -= data[i-elem];
		sum += data[i];
		rez[i] = sum/elem;
	}
	
	return rez;
}

int CVICALLBACK AfisareValoare (int panel, int control, int event,
								void *callbackData, int eventData1, int eventData2)
{
	static int top = 437;
	static int left = 12;
	static char strValue[20] ={0};
	
	switch (event)
	{
		case EVENT_COMMIT:
			double x, y;
			
			// Luam coordonatele de pe graf ale mouse-ului
			GetGraphCursor(panel, PANEL_GRAPH_HISTOGRAM, 1, &x, &y);
			
			// Punem valoarea intr-o caseta de text
			sprintf(strValue, "%.2lf", y);
			SetCtrlVal(panel, PANEL_HIST_VAL, strValue);
			
			// Plasam caseta de text la coordonatele absolute ale mouse-ului
			SetCtrlAttribute(panel, PANEL_HIST_VAL, ATTR_TOP, top);
			SetCtrlAttribute(panel, PANEL_HIST_VAL, ATTR_LEFT, left);
			SetCtrlAttribute(panel, PANEL_HIST_VAL, ATTR_VISIBLE,1);
			break;
			
		case EVENT_LEFT_CLICK:
			// Coordonatele absolute ale mouse-ului
			top = eventData1;
			left = eventData2;
			break;
	}
	return 0;
}

int CVICALLBACK IntervalPrev (int panel, int control, int event,
							  void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			// Valorile casetelor
			GetCtrlVal(panel, PANEL_START_SEC_NUMERIC, &startSec);
			GetCtrlVal(panel, PANEL_STOP_SEC_NUMERIC, &stopSec);
			
			// Actualizam secundele
			startSec--; stopSec--;
			
			// Verificam sa nu iesim din intervalul de val
			if (startSec < 0){
				startSec = 0;
				stopSec = 1;
			}
			
			// Actualizam secundele
			SetCtrlVal(panel, PANEL_START_SEC_NUMERIC, startSec);
			SetCtrlVal(panel, PANEL_STOP_SEC_NUMERIC, stopSec);
			
			stopPoint = sampleRate*stopSec;
			
			// Afisam
			SetAxisRange(panelHandle, PANEL_GRAPH_RAW_DATA, VAL_MANUAL, sampleRate*startSec, stopPoint, VAL_AUTOSCALE, 0, 0);
			SetAxisRange(panelHandle, PANEL_GRAPH_FILTERED_DATA, VAL_MANUAL, sampleRate*startSec, stopPoint, VAL_AUTOSCALE, 0, 0);
			
			// Salvare secunda curenta
			sprintf(fileName, "ProiectAPD_grafic_raw_data.jpg"); //generare nume fisier
			GetCtrlDisplayBitmap(panel, PANEL_GRAPH_RAW_DATA, 1, &bitmapID);
			SaveBitmapToJPEGFile(bitmapID, fileName, JPEG_PROGRESSIVE, 100);
			DiscardBitmap(bitmapID);
			
			sprintf(fileName, "ProiectAPD_grafic_filtered_data.jpg"); //generare nume fisier
			GetCtrlDisplayBitmap(panel, PANEL_GRAPH_FILTERED_DATA, 1, &bitmapID);
			SaveBitmapToJPEGFile(bitmapID, fileName, JPEG_PROGRESSIVE, 100);
			DiscardBitmap(bitmapID);
			
			break;
	}
	return 0;
}

int CVICALLBACK IntervalNext (int panel, int control, int event,
							  void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			int stopPrev;
			
			// Valorile casetelor
			GetCtrlVal(panel, PANEL_START_SEC_NUMERIC, &startSec);
			GetCtrlVal(panel, PANEL_STOP_SEC_NUMERIC, &stopSec);
			
			//Salvam valoarea precedenta
			stopPrev = stopSec;
			
			// Actualizam secundele
			startSec++; stopSec++;
			
			// Verificam sa nu iesim din intervalul de val
			if (stopSec*sampleRate >= npoints && stopPrev*sampleRate >= npoints){ // Mai avem mai putin de o secunda de afisat
				stopSec = (npoints-1)/(int)sampleRate + 1; // ultima secunda care nu e completa
				startSec = stopSec - 1;
			}
			
			// Actualizam secundele
			SetCtrlVal(panel, PANEL_START_SEC_NUMERIC, startSec);
			SetCtrlVal(panel, PANEL_STOP_SEC_NUMERIC, stopSec);
			
			// Afisam
			if (stopSec*sampleRate >= npoints){
				stopPoint = npoints; // Afisam doar punctele ramase
			}
			else{
				stopPoint = sampleRate*stopSec;
			}
			SetAxisRange(panelHandle, PANEL_GRAPH_RAW_DATA, VAL_MANUAL, sampleRate*startSec, stopPoint, VAL_AUTOSCALE, 0, 0);
			SetAxisRange(panelHandle, PANEL_GRAPH_FILTERED_DATA, VAL_MANUAL, sampleRate*startSec, stopPoint, VAL_AUTOSCALE, 0, 0);
			
			
			// Salvare secunda curenta
			sprintf(fileName, "ProiectAPD_grafic_raw_data.jpg"); //generare nume fisier
			GetCtrlDisplayBitmap(panel, PANEL_GRAPH_RAW_DATA, 1, &bitmapID);
			SaveBitmapToJPEGFile(bitmapID, fileName, JPEG_PROGRESSIVE, 100);
			DiscardBitmap(bitmapID);
			
			sprintf(fileName, "ProiectAPD_grafic_filtered_data.jpg"); //generare nume fisier
			GetCtrlDisplayBitmap(panel, PANEL_GRAPH_FILTERED_DATA, 1, &bitmapID);
			SaveBitmapToJPEGFile(bitmapID, fileName, JPEG_PROGRESSIVE, 100);
			DiscardBitmap(bitmapID);
			
			break;
	}
	return 0;
}

int CVICALLBACK Derivate (int panel, int control, int event,
						  void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			int pressed = 0;
			GetCtrlVal(panel, PANEL_DERIVATE_BUTTON, &pressed);
			
			if (pressed){
				derivate = Derivative(waveData, npoints);
				deriv = PlotY(panel, PANEL_GRAPH_RAW_DATA, derivate, npoints, VAL_DOUBLE, VAL_THIN_LINE, VAL_EMPTY_SQUARE, VAL_SOLID, VAL_CONNECTED_POINTS, VAL_GREEN);
				
				// Salvare secunda curenta
				sprintf(fileName, "ProiectAPD_grafic_raw_data.jpg"); //generare nume fisier
				GetCtrlDisplayBitmap(panel, PANEL_GRAPH_RAW_DATA, 1, &bitmapID);
				SaveBitmapToJPEGFile(bitmapID, fileName, JPEG_PROGRESSIVE, 100);
				DiscardBitmap(bitmapID);
			}else{
				DeleteGraphPlot(panel, PANEL_GRAPH_RAW_DATA, deriv, VAL_IMMEDIATE_DRAW);
			}
			
			break;
	}
	return 0;
}



//-----------------------------------------------------------------------------------------------------------------
// ETAPA 2
//-----------------------------------------------------------------------------------------------------------------


//-----------------------------------------------------------------------------------------------------------------
// Check boxes plot
int CVICALLBACK RawSignalPlot (int panel, int control, int event,
							   void *callbackData, int eventData1, int eventData2)
{
	int isChecked;
	
	switch (event)
	{
		case EVENT_COMMIT:
			GetCtrlVal(freqHandle, PANEL_FREQ_CHECKBOX_RAW_SIGNAL, &isChecked);
			if (isChecked){
				rawSignalPlot = PlotY(freqHandle, PANEL_FREQ_GRAPH_WAVE_DATA, waveData, npoints, VAL_DOUBLE, VAL_THIN_LINE, VAL_EMPTY_SQUARE, VAL_SOLID, VAL_CONNECTED_POINTS, VAL_RED);
			}else{
				DeleteGraphPlot(freqHandle, PANEL_FREQ_GRAPH_WAVE_DATA, rawSignalPlot, VAL_IMMEDIATE_DRAW);
			}
			break;
	}
	return 0;
}

int CVICALLBACK FilteredSignalPlot (int panel, int control, int event,
									void *callbackData, int eventData1, int eventData2)
{
	int isChecked;
	
	switch (event)
	{
		case EVENT_COMMIT:
			GetCtrlVal(freqHandle, PANEL_FREQ_CHECKBOX_FILTERED_SIG, &isChecked);
			if (isChecked){
				filteredSignalPlot = PlotY(freqHandle, PANEL_FREQ_GRAPH_WAVE_DATA, filteredSignal, npoints, VAL_DOUBLE, VAL_THIN_LINE, VAL_EMPTY_SQUARE, VAL_SOLID, VAL_CONNECTED_POINTS, VAL_BLUE);
			}else{
				DeleteGraphPlot(freqHandle, PANEL_FREQ_GRAPH_WAVE_DATA, filteredSignalPlot, VAL_IMMEDIATE_DRAW);
			}
			break;
	}
	return 0;
}


//-----------------------------------------------------------------------------------------------------------------
// Filters
int CVICALLBACK FreqApplyFilter (int panel, int control, int event,
								 void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{	
		case EVENT_COMMIT:
			int isChecked;
			double fe = sampleRate/2; // frecventa de esantionare
			
			SetCtrlAttribute(freqHandle, PANEL_FREQ_CHECKBOX_FILTERED_SIG, ATTR_DIMMED, 0);
			SetCtrlAttribute(freqHandle, PANEL_FREQ_SPECTRUM_RING, ATTR_DIMMED, 0);
			
			// Alocare memorie
			if (filteredSignal){
				free(filteredSignal);
				filteredSignal = 0;
			}
			filteredSignal = (double*) calloc(npoints, sizeof(double));
			
			if (freqfilterOption == BUTTERWORTH){
				Bw_LPF(waveData, npoints, sampleRate, 2*fe/3, 5, filteredSignal); // Butterworth pt 1/3 din frecventele inalte
			}else
				if (freqfilterOption == CHEBYSHEV){
					Ch_HPF(waveData, npoints, sampleRate, fe/3, 0.1, 5, filteredSignal); // Chebyshev pt 1/3 din frecventele joase
				}
			
			//Daca filtrul e afisat deja, il stergem si il afisam pe cel actual
			GetCtrlVal(freqHandle, PANEL_FREQ_CHECKBOX_FILTERED_SIG, &isChecked);
			if (isChecked){
				DeleteGraphPlot(freqHandle, PANEL_FREQ_GRAPH_WAVE_DATA, filteredSignalPlot, VAL_IMMEDIATE_DRAW);
				filteredSignalPlot = PlotY(freqHandle, PANEL_FREQ_GRAPH_WAVE_DATA, filteredSignal, npoints, VAL_DOUBLE, VAL_THIN_LINE, VAL_EMPTY_SQUARE, VAL_SOLID, VAL_CONNECTED_POINTS, VAL_BLUE);
			}
			
			break;
	}
	return 0;
}


//-----------------------------------------------------------------------------------------------------------------
// Spectrum
int CVICALLBACK ApplyPowerSpectrum (int panel, int control, int event,
									void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			int on;
			
			GetCtrlVal(freqHandle, PANEL_FREQ_START_SPECTRUM, &on);
			
			if (on){
				SetCtrlAttribute(freqHandle, PANEL_FREQ_GRAPH_FREQUENCY, ATTR_DIMMED, 0);
				
				//Facem restul de contrale dimmed pana la finalul executiei
				SetCtrlAttribute(freqHandle, PANEL_FREQ_SPECTRUM_RING, ATTR_DIMMED, 1);
				SetCtrlAttribute(freqHandle, PANEL_FREQ_NUM_POINTS_NUMERIC, ATTR_DIMMED, 1);
				
				//Activam graficele ferestrei
				SetCtrlAttribute(freqHandle, PANEL_FREQ_GRAPH_WINDOW, ATTR_DIMMED, 0);
				SetCtrlAttribute(freqHandle, PANEL_FREQ_GRAPH_WINDOWED_SIG, ATTR_DIMMED, 0);
				
				//Calculam coeficientii ferestrei si ii afisam
				if (windowCoeficients){
					free(windowCoeficients);
					windowCoeficients = 0;
				}
				windowCoeficients = GetWindowCoefficients(windowOption, numPoints);
		
				DeleteGraphPlot(freqHandle, PANEL_FREQ_GRAPH_WINDOW, -1, VAL_IMMEDIATE_DRAW);
				PlotY(freqHandle, PANEL_FREQ_GRAPH_WINDOW, windowCoeficients, numPoints, VAL_DOUBLE, VAL_CONNECTED_POINTS, VAL_SIMPLE_DOT, 
					  VAL_SOLID, VAL_CONNECTED_POINTS, VAL_RED);
				
				
				//Pornim timer-ul
				SetCtrlAttribute(freqHandle, PANEL_FREQ_TIMER, ATTR_ENABLED, 1);
			}
			else //Intrerupem executia
			{
				//Oprim timer-ul
				SetCtrlAttribute(freqHandle, PANEL_FREQ_TIMER, ATTR_ENABLED, 0);
				
				//Reactivam controalele
				if (filteredSignal){
					SetCtrlAttribute(freqHandle, PANEL_FREQ_SPECTRUM_RING, ATTR_DIMMED, 0);
				}
				SetCtrlAttribute(freqHandle, PANEL_FREQ_NUM_POINTS_NUMERIC, ATTR_DIMMED, 0);
				
				//Setam punctul inapoi la cel initial
				startWindow = 0;
				
				//Afisam inapoi intreg semnalul pe eave data
				SetAxisRange(freqHandle, PANEL_FREQ_GRAPH_WAVE_DATA, VAL_MANUAL, 0, npoints, VAL_AUTOSCALE, 0, 0);
			}
			break;
	}
	return 0;
}

int CVICALLBACK EveryNCallbacks (int panel, int control, int event,
								 void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_TIMER_TICK:
			double *spectrum = 0;
			int year, month, day, hour, minute, second;
			
			if (startWindow + numPoints < npoints){
				//Afisare semnal wave data N puncte
				SetAxisRange(freqHandle, PANEL_FREQ_GRAPH_WAVE_DATA, VAL_MANUAL, startWindow, startWindow + numPoints, VAL_AUTOSCALE, 0, 0);
				
				//Calculam spectrul
				if (spectrumOption == RAWDATASPECTRUM){
					spectrum = ComputePowerSpectrum (waveData, startWindow, numPoints, sampleRate, &df, &powerPeak, &freqPeak);
					
				}else 
					if (spectrumOption == FILTERSPECTRUM){
						spectrum = ComputePowerSpectrum (filteredSignal, startWindow, numPoints, sampleRate, &df, &powerPeak, &freqPeak);
					}
				
				SetCtrlVal(freqHandle, PANEL_FREQ_FREQ_PEAK_NUMERIC, freqPeak);
				SetCtrlVal(freqHandle, PANEL_FREQ_POWER_PEAK_NUMERIC, powerPeak);
				
				//Afisam spectrul
				DeleteGraphPlot(freqHandle, PANEL_FREQ_GRAPH_FREQUENCY, -1, VAL_IMMEDIATE_DRAW);
				PlotWaveform(freqHandle, PANEL_FREQ_GRAPH_FREQUENCY, spectrum, numPoints/2, VAL_DOUBLE, 1.0, 0.0, 0.0, df, VAL_THIN_LINE, VAL_EMPTY_SQUARE, 
							 VAL_SOLID, VAL_CONNECTED_POINTS, VAL_DK_GREEN);
				
				//Salvam imagini cu semnalul si spectrul
				GetSystemDate(&month, &day, &year);
				GetSystemTime(&hour, &minute, &second);
					
				sprintf(fileName, ".\\Image\\ProiectAPD_grafic_waveData_%02d_%02d.%02d.%04d.jpg", hour, day, month, year); //generare nume fisier
				GetCtrlDisplayBitmap(freqHandle, PANEL_FREQ_GRAPH_WAVE_DATA, 1, &bitmapID);
				SaveBitmapToJPEGFile(bitmapID, fileName, JPEG_PROGRESSIVE, 100);
				DiscardBitmap(bitmapID);
				
				if (spectrumOption == RAWDATASPECTRUM){
					sprintf(fileName, ".\\Image\\ProiectAPD_grafic_spectrum_raw_%02d_%02d.%02d.%04d.jpg", hour, day, month, year); //generare nume fisier
				}else
				{
					sprintf(fileName, ".\\Image\\ProiectAPD_grafic_spectrum_filtered_%02d_%02d.%02d.%04d.jpg", hour, day, month, year); //generare nume fisier
				}
				GetCtrlDisplayBitmap(freqHandle, PANEL_FREQ_GRAPH_FREQUENCY, 1, &bitmapID);
				SaveBitmapToJPEGFile(bitmapID, fileName, JPEG_PROGRESSIVE, 100);
				DiscardBitmap(bitmapID);
				
				if (spectrum){
					free(spectrum);
					spectrum = 0;
				}
				
				startWindow += numPoints;
			}
			else // S-a terminat executia
			{	
				//Oprim timer-ul
				SetCtrlAttribute(freqHandle, PANEL_FREQ_TIMER, ATTR_ENABLED, 0);
				
				//Reactivam controalele
				SetCtrlAttribute(freqHandle, PANEL_FREQ_SPECTRUM_RING, ATTR_DIMMED, 0);
				SetCtrlAttribute(freqHandle, PANEL_FREQ_NUM_POINTS_NUMERIC, ATTR_DIMMED, 0);
				
				//Setam punctul inapoi la cel initial
				startWindow = 0;
				
				//Afisam inapoi intreg semnalul pe eave data
				SetAxisRange(freqHandle, PANEL_FREQ_GRAPH_WAVE_DATA, VAL_MANUAL, 0, npoints, VAL_AUTOSCALE, 0, 0);
				
				//Modificam valoarea butonului Start
				SetCtrlVal(freqHandle, PANEL_FREQ_START_SPECTRUM, 0);
			}
			break;
	}
	return 0;
}

double* ComputePowerSpectrum(double inputArray[], unsigned int start, unsigned int numberOfElements, double samplingRate, 
						 double *frequencyInterval, double *powerPeak, double *freqPeak)
{
	double* convertedSpectrum = 0;
	double* windowedSignal = 0;
	char unit[32]="V";
	WindowConst window;
	double dt = 1.0/samplingRate;
	int i;
	
	//Salvam semnalul pe care il ferestruim pentru a nu-l modifica
	windowedSignal = (double*) calloc(numberOfElements, sizeof(double));
	for(i=0; i < numberOfElements; ++i){
		windowedSignal[i] = inputArray[i + start];
	}
	
	//Calculam si afisam semnalul ferestruit
	ScaledWindowEx(windowedSignal, numberOfElements, windowOption, 0, &window);
	
	DeleteGraphPlot(freqHandle, PANEL_FREQ_GRAPH_WINDOWED_SIG, -1, VAL_IMMEDIATE_DRAW);
	PlotY(freqHandle, PANEL_FREQ_GRAPH_WINDOWED_SIG, windowedSignal, numPoints, VAL_DOUBLE, VAL_CONNECTED_POINTS, VAL_SIMPLE_DOT, 
					  VAL_SOLID, VAL_CONNECTED_POINTS, VAL_RED);
	
	
	//Calculam spectrul
	convertedSpectrum = (double*) calloc(numberOfElements/2, sizeof(double));
	
	AutoPowerSpectrum(windowedSignal, numberOfElements, dt, convertedSpectrum, frequencyInterval);
	PowerFrequencyEstimate(convertedSpectrum, numberOfElements/2, -1.0, window, *frequencyInterval, 7, freqPeak, powerPeak);
	SpectrumUnitConversion(convertedSpectrum, numberOfElements/2, SPECTRUM_POWER, SCALING_MODE_LINEAR, DISPLAY_UNIT_VRMS, *frequencyInterval, window, convertedSpectrum, unit);
	
	free(windowedSignal);
	windowedSignal = 0;
	
	return convertedSpectrum;
}

double* GetWindowCoefficients(int windowOption, int numberOfElements)
{
	double* coefficients = 0;
	int i;
	
	coefficients = (double*)calloc(numberOfElements, sizeof(double));
	if (windowOption == RECTANGLE_){
		for (i = 0; i < numberOfElements; ++i){
			coefficients[i] = 1.0;
		}
	}else
		if (windowOption == HAMMING_){
			for (i = 0; i < numberOfElements; ++i){
				coefficients[i] = 0.54 - 0.46 * cos(2 * PI * i/numberOfElements);
			}
		}
	
	return coefficients;
}


//-----------------------------------------------------------------------------------------------------------------
// Get controls
int CVICALLBACK FreqSetFilter (int panel, int control, int event,
							void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			GetCtrlVal(panel, PANEL_FREQ_FILTER_RING, &freqfilterOption);
			break;
	}
	return 0;
}

int CVICALLBACK ChangeNPoints (int panel, int control, int event,
							   void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			GetCtrlVal(freqHandle, PANEL_FREQ_NUM_POINTS_NUMERIC, &numPoints);
			break;
	}
	return 0;
}

int CVICALLBACK SetWindow (int panel, int control, int event,
						   void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			int option;
			
			GetCtrlVal(freqHandle, PANEL_FREQ_WINDOW_OPTION_RING, &option);
			if (option == 0){
				windowOption = RECTANGLE_;
			}else{
				windowOption = HAMMING_;
			}
			break;
	}
	return 0;
}

int CVICALLBACK SetSpectrumData (int panel, int control, int event,
								 void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			GetCtrlVal(panel, PANEL_FREQ_SPECTRUM_RING, &spectrumOption);
			break;
	}
	return 0;
}

int CVICALLBACK SpectrumGraph (int panel, int control, int event,
							   void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			double freq, y;
			
			GetGraphCursor(freqHandle, PANEL_FREQ_GRAPH_FREQUENCY, 1, &freq, &y);
			SetCtrlVal(freqHandle, PANEL_FREQ_FREQ_VAL_NUMERIC, (unsigned int)freq);
			break;
	}
	return 0;
}
