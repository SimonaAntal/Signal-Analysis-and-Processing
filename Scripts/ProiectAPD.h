/**************************************************************************/
/* LabWindows/CVI User Interface Resource (UIR) Include File              */
/*                                                                        */
/* WARNING: Do not add to, delete from, or otherwise modify the contents  */
/*          of this include file.                                         */
/**************************************************************************/

#include <userint.h>

#ifdef __cplusplus
    extern "C" {
#endif

     /* Panels and Controls: */

#define  PANEL                            1       /* callback function: ExitPanel */
#define  PANEL_IDC_SWITCH_PANEL           2       /* control type: binary, callback function: SwitchPanel */
#define  PANEL_APPLY_FILTER_BUTTON        3       /* control type: command, callback function: ApplyFilter */
#define  PANEL_EXIT_BUTTON                4       /* control type: command, callback function: Exit */
#define  PANEL_NEXT_BUTTON                5       /* control type: command, callback function: IntervalNext */
#define  PANEL_PREV_BUTTON                6       /* control type: command, callback function: IntervalPrev */
#define  PANEL_MEAN_NUMERIC               7       /* control type: numeric, callback function: (none) */
#define  PANEL_MEDIAN_NUMERIC             8       /* control type: numeric, callback function: (none) */
#define  PANEL_DECORATION_4               9       /* control type: deco, callback function: (none) */
#define  PANEL_DECORATION_3               10      /* control type: deco, callback function: (none) */
#define  PANEL_DECORATION_2               11      /* control type: deco, callback function: (none) */
#define  PANEL_STOP_SEC_NUMERIC           12      /* control type: numeric, callback function: (none) */
#define  PANEL_START_SEC_NUMERIC          13      /* control type: numeric, callback function: (none) */
#define  PANEL_ZERO_NUMERIC               14      /* control type: numeric, callback function: (none) */
#define  PANEL_VARIANCE_NUMERIC           15      /* control type: numeric, callback function: (none) */
#define  PANEL_MAX_NUMERIC                16      /* control type: numeric, callback function: (none) */
#define  PANEL_MAX_INDEX_NUMERIC          17      /* control type: numeric, callback function: (none) */
#define  PANEL_MIN_INDEX_NUMERIC          18      /* control type: numeric, callback function: (none) */
#define  PANEL_ALPHA_NUMERIC              19      /* control type: numeric, callback function: (none) */
#define  PANEL_KURTOSIS_NUMERIC           20      /* control type: numeric, callback function: (none) */
#define  PANEL_SKEWNESS_NUMERIC           21      /* control type: numeric, callback function: (none) */
#define  PANEL_MIN_NUMERIC                22      /* control type: numeric, callback function: (none) */
#define  PANEL_GRAPH_HISTOGRAM            23      /* control type: graph, callback function: AfisareValoare */
#define  PANEL_GRAPH_FILTERED_DATA        24      /* control type: graph, callback function: (none) */
#define  PANEL_GRAPH_RAW_DATA             25      /* control type: graph, callback function: (none) */
#define  PANEL_LOAD_BUTTON                26      /* control type: command, callback function: LoadButton */
#define  PANEL_MEDIAN_VALUES_RING         27      /* control type: ring, callback function: Filter */
#define  PANEL_FILTER_RING                28      /* control type: ring, callback function: Filter */
#define  PANEL_HIST_VAL                   29      /* control type: textMsg, callback function: (none) */
#define  PANEL_INTERVAL                   30      /* control type: textMsg, callback function: (none) */
#define  PANEL_DERIVATE_BUTTON            31      /* control type: textButton, callback function: Derivate */

#define  PANEL_FREQ                       2       /* callback function: ExitPanel */
#define  PANEL_FREQ_APPLY_FILTER_BUTTON   2       /* control type: command, callback function: FreqApplyFilter */
#define  PANEL_FREQ_IDC_SWITCH_PANEL      3       /* control type: binary, callback function: SwitchPanel */
#define  PANEL_FREQ_GRAPH_FREQUENCY       4       /* control type: graph, callback function: SpectrumGraph */
#define  PANEL_FREQ_GRAPH_WAVE_DATA       5       /* control type: graph, callback function: (none) */
#define  PANEL_FREQ_EXIT_BUTTON           6       /* control type: command, callback function: Exit */
#define  PANEL_FREQ_NUM_POINTS_NUMERIC    7       /* control type: numeric, callback function: ChangeNPoints */
#define  PANEL_FREQ_FREQ_VAL_NUMERIC      8       /* control type: numeric, callback function: (none) */
#define  PANEL_FREQ_POWER_PEAK_NUMERIC    9       /* control type: numeric, callback function: (none) */
#define  PANEL_FREQ_FREQ_PEAK_NUMERIC     10      /* control type: numeric, callback function: (none) */
#define  PANEL_FREQ_DECORATION_5          11      /* control type: deco, callback function: (none) */
#define  PANEL_FREQ_DECORATION_4          12      /* control type: deco, callback function: (none) */
#define  PANEL_FREQ_DECORATION_3          13      /* control type: deco, callback function: (none) */
#define  PANEL_FREQ_WINDOW_OPTION_RING    14      /* control type: ring, callback function: SetWindow */
#define  PANEL_FREQ_GRAPH_WINDOWED_SIG    15      /* control type: graph, callback function: (none) */
#define  PANEL_FREQ_GRAPH_WINDOW          16      /* control type: graph, callback function: (none) */
#define  PANEL_FREQ_FILTER_RING           17      /* control type: ring, callback function: FreqSetFilter */
#define  PANEL_FREQ_TIMER                 18      /* control type: timer, callback function: EveryNCallbacks */
#define  PANEL_FREQ_SPECTRUM_RING         19      /* control type: ring, callback function: SetSpectrumData */
#define  PANEL_FREQ_CHECKBOX_FILTERED_SIG 20      /* control type: radioButton, callback function: FilteredSignalPlot */
#define  PANEL_FREQ_CHECKBOX_RAW_SIGNAL   21      /* control type: radioButton, callback function: RawSignalPlot */
#define  PANEL_FREQ_START_SPECTRUM        22      /* control type: textButton, callback function: ApplyPowerSpectrum */
#define  PANEL_FREQ_SPECTRUM_TEXT         23      /* control type: textMsg, callback function: (none) */
#define  PANEL_FREQ_SIGNALS_TEXT          24      /* control type: textMsg, callback function: (none) */


     /* Control Arrays: */

          /* (no control arrays in the resource file) */


     /* Menu Bars, Menus, and Menu Items: */

          /* (no menu bars in the resource file) */


     /* Callback Prototypes: */

int  CVICALLBACK AfisareValoare(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK ApplyFilter(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK ApplyPowerSpectrum(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK ChangeNPoints(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK Derivate(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK EveryNCallbacks(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK Exit(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK ExitPanel(int panel, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK Filter(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK FilteredSignalPlot(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK FreqApplyFilter(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK FreqSetFilter(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK IntervalNext(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK IntervalPrev(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK LoadButton(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK RawSignalPlot(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK SetSpectrumData(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK SetWindow(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK SpectrumGraph(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK SwitchPanel(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);


#ifdef __cplusplus
    }
#endif