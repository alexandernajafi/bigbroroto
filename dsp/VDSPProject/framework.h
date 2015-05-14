#ifndef framework_h_included
#define framework_h_included

/* Configuration */
#define DSP_SAMPLE_RATE         16000
#define DSP_BLOCK_SIZE          256
#define DSP_INPUT_GAIN          30
#define DSP_OUTPUT_ATTENUATION  10

/* Codec selection */
//#define DSP_USE_DOUBLE_RATE

/* Constants */
#define DSP_FREQUENCY           (12288000*16/2)

/* Framework interface */
typedef struct {
    _Fract left;
    _Fract right;
} sample_t;

void dsp_init(void);
void dsp_start(void);
void dsp_stop(void);
unsigned int dsp_get_keys(void);
unsigned int dsp_get_cycles(void);
void dsp_set_leds(unsigned int value);

sample_t *dsp_get_audio(void);
sample_t *dsp_get_audio_01(void);
sample_t *dsp_get_audio_23(void);

#endif
