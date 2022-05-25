// gcc write2mp4.c ../../modules/wav.c -o vid 

#include <stdio.h>
#include <stdlib.h>

#include "../../modules/wav.h"
// #include "../../modules/filters.h"

#define W 1280 
#define H 720

int16_t* retrieve_data(struct wav_info *w);
/* prompts user to input file path, reads input data using 
   "open_file_read_data", updates and prints wav_info, converts data to
   floating point using "convert_data_to_float", and returns float data. */

int16_t* open_file_read_data(struct wav_info *w);
/* open .wav file, read int data into buffer and return stored data. */

float* convert_data_to_float(struct wav_info *w, int16_t* idata);
/* convert the int data into float using the max bits per sample stored in 
   .wav file header. */

void create_file_write_data(struct wav_info *w, float* fdata);
/* reads output file path, prints wav_info, writes header, converts floating
   point data back to int, and writes data to .wav. */

void normalize_data(float *fdata, int N);
/* finding maximum output value to "normalize" */

void create_file_write_data(struct wav_info *w, float* fdata);
/* reads output file path, prints wav_info, writes header, converts floating
   point data back to int, and writes data to .wav. */

void find_max_int(struct wav_info* w, int* max_int);
/* Find the max integer a wav file can express - essentially figure out a wav
   file's bit-depth to determine the largest integer expressable. 
   
   For example, if the bit depth was 16 bits, the maximum integer expressable
   would be 32767 */

unsigned char frame[H][W][3] = {0};

void main() {

	// initialize struct containing input data header info and pointer to 
	// input data
	struct wav_info input;
	
	int16_t *idata = retrieve_data(&input);

	if(idata == NULL) { return; }
	
	int x, y, count = 0;
	FILE *pipeout = popen("ffmpeg -y -f rawvideo -vcodec rawvideo -pix_fmt rgb24 -s 1280x720 -r 25 -i - -f mp4 -q:v 5 -an -vcodec mpeg4 output.mp4", "w");

	while(count < input.num_samples) {
		for(y=0; y<H; y++) {
			for(x=0; x<W; x++) {
				frame[y][x][0] = idata[count] % 255; // rand() % 255;
				frame[y][x][1] = idata[count] % 255; // rand() % 255;
				frame[y][x][2] = idata[count] % 255; // rand() % 255;
			}
		}
		fwrite(frame, 1, H*W*3, pipeout);
		count++;
	}

	free(idata);
	fflush(pipeout);
	pclose(pipeout);

}

int16_t* retrieve_data(struct wav_info *w) {

	int16_t *idata;

	// prompt user to enter input file path. If the file is valid, open it and
	// read the data into the idata buffer.
	printf("\n");
	printf("Enter relative file path to .wav file: ");
	idata = open_file_read_data(w);

	if(idata == NULL) { return NULL; }

	// print input file wav header info
	printf("\n");
	printf("WAV File Header Info:\n");
	printf("-----------------------------------\n");
    print_wav_info(w);
	printf("-----------------------------------\n");
	printf("\n");

	return idata;

	/*
	// initialize pointer to buffer containing input data of type float,
	// convert the integer data to float, and store it in the fdata buffer.
	// free idata's memory since we will only be using fdata from now on. 
	float *fdata;
	fdata = convert_data_to_float(w, idata);
	free(idata);

	if(fdata == NULL) { return NULL; }

	return fdata;
	*/
}

int16_t* open_file_read_data(struct wav_info *w) {

	// user input relative file path to .wav file
    char file_path[1000];
	scanf("%s", file_path); 

	// char* file_path = "data/input_signals/DeChaka_Instrumental.wav";

	// loop to check file exists. The loop will not break until a correct 
	// file path has been inputed.
	while(1) 
	{
		FILE* file;

		// if it exists, close the file and break out of the loop
		if(file = fopen(file_path, "r"))
		{
			fclose(file);
			printf("\n");
			printf("File exists\n");

			break;
		}
		// else, print an error and prompt user to try again
		else
		{
			printf("\n");
			printf("Error: File does not exist\n");

			printf("\n");
			printf("Enter relative file path again: ");
			scanf("%s", file_path); 
		}
	}
	
	// open and read .wav file. "rb" means read binary 
    FILE* fp = fopen(file_path,"rb");

	// reads the header and inputs info into the wav_info struct
	read_wav_info(w, fp);

	// allocate data based on the number of samples and number of channels and
	// store the int data in the idata buffer. 
	int16_t *idata = (int16_t *)malloc(sizeof(int16_t)*(w->num_samples*w->num_channels));
	if(idata == NULL) { return NULL; }

	read_wav_data(w, idata, fp); 

	// now that the header and the data has been read and stored locally there
	// is no more need for the file. 
	fclose(fp);

	return idata;
}

float* convert_data_to_float(struct wav_info *w, int16_t* idata) {
/* find the largest expressable integer based on the .wav formatting, 
   specifically the amount of bits per sample */

	uint32_t max_int;
	find_max_int(w, &max_int);
	
	// allocate memory for float data buffer
	float *fdata = (float *)malloc(sizeof(float)*(w->num_samples*w->num_channels));

	if(fdata == NULL) { return NULL; }

	/*
	float *fdata;
	if(STATE == 6 && SAME_NUM_CHANNELS == 0 && w->num_channels == 1)
	{
		fdata = (float *)malloc(sizeof(float)*(w->num_samples*2));
	}
	else
	{
		fdata = (float *)malloc(sizeof(float)*(w->num_samples*w->num_channels));
	}
	*/

	// divide each data point by max_int to convert the data into floats
	// ranging from -1 to 1.
	for(int i=0; i<w->num_samples*w->num_channels; i++) {

		fdata[i] = ((float)idata[i]/(float)max_int);
	}

	return fdata;
}

void normalize_data(float *fdata, int N) {
/* Finding maximum output value to "normalize" */

	float record = -1000.0;
	for (int i=0; i<N; i++)
	{
		// printf("%f\n", output_fdata[i]);

		if (abs(fdata[i]) > record)
		{
			record = abs(fdata[i]);
		}
	}
	// printf("%f\n", record); 

	for (int i=0; i<N; i++)
	{
		// printf("%f\n", output_fdata[i]);
		fdata[i] /= record + 1.0;	// "normalizing" the data
	}
}

void create_file_write_data(struct wav_info *w, float *fdata) {

	// user input relative file path to .wav file
    char file_path[1000];
	scanf("%s", file_path); 

	// set output file path and print the .wav file header info
	printf("\n");
	printf("---------------------------------------------\n");
	printf("Preparing to write to %s:\n", file_path);
    print_wav_info(w);
	printf("---------------------------------------------\n");
	printf("\n");

	// open the output file and write the header info to the .wav file
    FILE* fp = fopen(file_path,"wb");
	if(!fp) {
        fprintf(stderr,"Error opening %s for output!\n",fp);
    }

    write_wav_hdr(w,fp);

	// compute the largest expressable integer based on the amount of bits
	// per sample.
	int max_int;
	find_max_int(w, &max_int);

	float max_float = (float)max_int;
	//printf("%f\n", max_output_float);

	// write the 2-channel data to a buffer of length 2, converting the float
	// data to int before storing it in the buffer. Afterwards, write the 2-
	// channel sampel using the write_sample() function.
	int_fast16_t sample[2];
	for(int i=0; i < w->num_samples; i++) 
	{
		for(int j=0; j < w->num_channels; j++)
		{
			sample[j] = (int_fast16_t)(fdata[i+(w->num_samples*j)]*max_float);
		}
		write_sample(w, fp, sample);
	}

	fclose(fp);
}


void find_max_int(struct wav_info* w, int* max_int) {
/* determine maximum integer expressable in w.bits_per_sample bits, assuming
twos complement encoding of signed integers */

    switch(w->bits_per_sample) {
    case 8:
      *max_int=0x7f;		/* 127 */
      break;
    case 16:
      *max_int=0x7fff;		/* 32767 */
      break;
    case 24:
      *max_int=0x7fffff;	/* 8388607 */
      break;
    case 32:
      *max_int=0x7fffffff;	/* 2147483647 */
      break;
    default:
      printf("error: bits_per_sample must be 8,16,24, or 32.\n");
    }
}
