/**
  @author Sander Schulhoff
  @email sanderschulhoff@gmail.com 
  This code is directly adapted from a PDM usage example. I modified it to have a larger buffer
  and wait for a start and stop signal on the serial port. After it stops recording sound data,
  it sends all of the data across the serial port.
**/

// includes for the arduino system and the PDM sound sensor
#include <Arduino.h>
#include <PDM.h>

// max # of samples that can be recorded
#define SAMPLES 51200

// variable to store the start/stop status to be received on the serial port
volatile int start_stop = 0;

// default number of output channels
static const char channels = 1;

// store if program has recorded ~ as many samples as fits in the buffer
bool sampleMaxReached = false;

// record total # of samples (each short in the buffer is considered a sample)
int totalSamples = 0;

// default PCM output frequency
static const int frequency = 16000;

// Buffer to read samples into, each sample is 16-bits
short sampleBuffer[SAMPLES];

// Total number of audio samples read
volatile int samplesRead;

void onPDMdata();

// do once
void setup()
{
  // open Serial port connection for reading
  Serial.begin(9600);
  Serial.flush();
  Serial.setTimeout(0.1);
  while (!Serial)
    ;
}

// do forever
void loop()
{
  //  if serial data has been sent to the serial port (from another program)
  if (Serial.available() > 0)
  {
    start_stop = Serial.read();
  }

  // FOR DEBUGGING: uncomment so that this code starts collecting data immediately regardless
  // of whether start signal is sent
  // if (start_stop == 0){
  //   start_stop = 49;
  // }

  if (start_stop == 49)
  {
    // call onPDMdata when PDM receives data
    PDM.onReceive(onPDMdata);

    // Initialize PDM with:
    // - one channel (mono mode)
    // - a 16 kHz sample rate for the Arduino Nano 33 BLE Sense
    // - a 32 kHz or 64 kHz sample rate for the Arduino Portenta Vision Shield
    if (!PDM.begin(channels, frequency))
    {
      Serial.println("Failed to start PDM!");
      while (1)
        ;
    }
    start_stop = 2;
  }
  // Wait for samples to be read
  if (samplesRead)
  {
    // start_stop = 50 when the stop code is received
    if (start_stop == 50 || sampleMaxReached)
    {
      // stop collecting data
      PDM.end();
      // print all data to the Serial port
      for (int x = 0; x < totalSamples; x++)
      {
        Serial.println(sampleBuffer[x]);
      }
      exit(1);
    }

    // Clear the temporary read count
    samplesRead = 0;
  }
}

/**
 * Callback function to process the data from the PDM microphone.
 * NOTE: This callback is executed as part of an ISR.
 * Therefore using `Serial` to print messages inside this function isn't supported.
 * */
void onPDMdata()
{
  // Query the number of available bytes
  int bytesAvailable = PDM.available();

  // Read into the sample buffer at index offset by totalSamples already collected
  PDM.read(&sampleBuffer[totalSamples], bytesAvailable);

  // 16-bit, 2 bytes per sample
  samplesRead = bytesAvailable / 2;
  totalSamples += samplesRead;
  
  // stop recording if close to SAMPLES
  if (totalSamples >= SAMPLES - 2 * samplesRead)
  {
    sampleMaxReached = true;
  }
}