#include <iostream>
#include <fstream>
#include <math.h>

using namespace std;

#define WAVE_FORMAT_UNKNOWN      0X0000;
#define WAVE_FORMAT_PCM          0X0001;
#define WAVE_FORMAT_MS_ADPCM     0X0002;
#define WAVE_FORMAT_IEEE_FLOAT   0X0003;
#define WAVE_FORMAT_ALAW         0X0006;
#define WAVE_FORMAT_MULAW        0X0007;
#define WAVE_FORMAT_IMA_ADPCM    0X0011;
#define WAVE_FORMAT_YAMAHA_ADPCM 0X0016;
#define WAVE_FORMAT_GSM          0X0031;
#define WAVE_FORMAT_ITU_ADPCM    0X0040;
#define WAVE_FORMAT_MPEG         0X0050;
#define WAVE_FORMAT_EXTENSIBLE   0XFFFE;

#define DURATION 10
#define SAMPLE_RATE 44100
#define CHANNEL 1
#define BIT_RATE 32


ofstream& operator<<(ofstream& out, const RIFF& myheader)
{
	out.write((char*)&myheader.ChunkID, sizeof(myheader.ChunkID));
	out.write((char*)&myheader.ChunkSize, sizeof(myheader.ChunkSize));
	out.write((char*)&myheader.Format, sizeof(myheader.Format));
	return out;
}

ofstream& operator<<(ofstream& out, const FMT& myheader)
{
	out.write((char*)&myheader.ChunkID, sizeof(myheader.ChunkID));
	out.write((char*)&myheader.ChunkSize, sizeof(myheader.ChunkSize));
	out.write((char*)&myheader.AudioFormat, sizeof(myheader.AudioFormat));
	out.write((char*)&myheader.NumChannels, sizeof(myheader.NumChannels));
	out.write((char*)&myheader.SampleRate, sizeof(myheader.SampleRate));
	out.write((char*)&myheader.AvgByteRate, sizeof(myheader.AvgByteRate));
	out.write((char*)&myheader.BlockAlign, sizeof(myheader.BlockAlign));
	out.write((char*)&myheader.BitPerSample, sizeof(myheader.BitPerSample));
	return out;
}

ofstream& operator<<(ofstream& out, const DATA& myheader)
{
	out.write((char*)&myheader.ChunkID, sizeof(myheader.ChunkID));
	out.write((char*)&myheader.ChunkSize, sizeof(myheader.ChunkSize));
	return out;
}


typedef struct
{
	unsigned char ChunkID[4];    // Contains the letters "RIFF" in ASCII form
	unsigned int ChunkSize;      // This is the size of the rest of the chunk following this number
	unsigned char Format[4];     // Contains the letters "WAVE" in ASCII form
} RIFF;

//-------------------------------------------
// [Channel]
// - streo     : [left][right]
// - 3 channel : [left][right][center]
// - quad      : [front left][front right][rear left][reat right]
// - 4 channel : [left][center][right][surround]
// - 6 channel : [left center][left][center][right center][right][surround]
//-------------------------------------------
typedef struct
{
	unsigned char  ChunkID[4];    // Contains the letters "fmt " in ASCII form
	unsigned int   ChunkSize;     // 16 for PCM.  This is the size of the rest of the Subchunk which follows this number.
	unsigned short AudioFormat;   // PCM = 1
	unsigned short NumChannels;   // Mono = 1, Stereo = 2, etc.
	unsigned int   SampleRate;    // 8000, 44100, etc.
	unsigned int   AvgByteRate;   // SampleRate * NumChannels * BitsPerSample/8
	unsigned short BlockAlign;    // NumChannels * BitsPerSample/8
	unsigned short BitPerSample;  // 8 bits = 8, 16 bits = 16, etc
} FMT;


typedef struct
{
	char          ChunkID[4];    // Contains the letters "data" in ASCII form
	unsigned int  ChunkSize;     // NumSamples * NumChannels * BitsPerSample/8
} DATA;

class WAVE_HEADER
{
private:
	RIFF riff;
	FMT fmt;
	DATA data;
	char* Name;
	char* SoundData;
public:
	WAVE_HEADER(char* InName); // Initialize
	void AppendData(int pressure); // Append data chunk
	void SaveWave(); // Save WAV file
};

WAVE_HEADER::WAVE_HEADER(char* InName)
{
	Name = InName;

	// Initialize RIFF chunk
	memcpy(riff.ChunkID, "RIFF", 4);
	riff.ChunkSize = 36; // riff.ChunkSize is determined after recording is over.
	memcpy(riff.Format, "WAVE", 4);

	// Initialize FMT chunk
	memcpy(fmt.ChunkID, "fmt ", 4);
	fmt.ChunkSize = 0x10;
	fmt.AudioFormat = WAVE_FORMAT_PCM;
	fmt.NumChannels = CHANNEL;
	fmt.SampleRate = SAMPLE_RATE;
	fmt.AvgByteRate = SAMPLE_RATE * CHANNEL * BIT_RATE / 8; // The number of byte used in a second
	fmt.BlockAlign = CHANNEL * BIT_RATE / 8; // size of sample frame
	fmt.BitPerSample = BIT_RATE; // The number of bit per sample

	// Initialize DATA chunk
	memcpy(data.ChunkID, "data", 4);
	//// data.ChunkSize is determined after recording is over.
}

void WAVE_HEADER::AppendData(int pressure) // Input: num of channel, sample rate, byte rate, block align, bit per sample
{
	int temp = 0;
	// need formula which calculate wave data from pressure change

	strcat(SoundData, (char*) temp);
	// riff.ChunkSize, data.ChunkSize should be incresed as data is appended
}

void WAVE_HEADER::SaveWave()
{
	// Open file
	char* fname = strcat(Name, ".wav");
	ofstream f_out(fname, ios::binary | ios::out);

	// Write header on file
	f_out << riff;
	f_out << fmt;
	f_out << data;
	f_out.write(SoundData, sizeof(SoundData));
	
	f_out.close();
}