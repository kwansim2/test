/*
	reference from "http://crystalcube.co.kr/123"
*/


#include <iostream>
#include <fstream>
#include <math.h> // Included for sin function
#include "WaveHeader.h"

using namespace std; // HAHAHAHA

#define DURATION 10 // Length of WAV file
#define SAMPLE_RATE 44100
#define CHANNEL 1 // 1 means mono. To record stereo sound, header and data should be double.
#define BIT_RATE 32


// Overloadings for "file << data"
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


int main()
{
	// File open in binary mode
	ofstream f_out("testsound.wav", ios::binary | ios::out);

	// Fill in header.Riff
	WAVE_HEADER header;
	memcpy(header.Riff.ChunkID, "RIFF", 4); // write "RIFF"
	header.Riff.ChunkSize = DURATION * SAMPLE_RATE * CHANNEL * BIT_RATE / 8 + 36; // Chunksize = file size - 8 byte
	memcpy(header.Riff.Format, "WAVE", 4); // write "WAVE"
	
	// Fill in header.Fmt
	memcpy(header.Fmt.ChunkID, "fmt ", 4);
	header.Fmt.ChunkSize = 0x10; // size of folowing fmt chunk
	header.Fmt.AudioFormat = WAVE_FORMAT_PCM;
	header.Fmt.NumChannels = CHANNEL;
	header.Fmt.SampleRate = SAMPLE_RATE; // The number of sample per second
	header.Fmt.AvgByteRate = SAMPLE_RATE * CHANNEL * BIT_RATE / 8; // The number of byte used in a second
	header.Fmt.BlockAlign = CHANNEL * BIT_RATE / 8; // size of sample frame
	header.Fmt.BitPerSample = BIT_RATE; // The number of bit per sample

	// Fill in header.Data
	memcpy(header.Data.ChunkID, "data", 4); // write "data"
	header.Data.ChunkSize = DURATION * SAMPLE_RATE * CHANNEL * BIT_RATE / 8; // size of data


	// Write header into file
	f_out << header.Riff;
	f_out << header.Fmt;
	f_out << header.Data;

	// Append data into file
	short test;
	char* test2;
	double freq = 440;
	for (int i = 0; i < SAMPLE_RATE * DURATION * CHANNEL * BIT_RATE / 8; i++)
	{
		test = (short)(30000*sin(2 * 3.141592 * i * freq / SAMPLE_RATE)); // sample function to make sound
		f_out.write((char*)&test, sizeof(test)); // append data
	}

	// Close file
	f_out.close();

	return 0;
}
