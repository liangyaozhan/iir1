// Usage Examples
//
// This shows you how to operate the filters
//

// This is the only include you need
#include "Iir.h"

#include <sstream>
#include <iostream>
#include <iomanip>
#include <stdio.h>

#include "wave-signal.h"
#include <fftw3.h>

int g_global_v;


template <typename filter>
void freqz( filter &ftr,
            double freq0, double freq1, double freq_step,
            int fs, int np, int skip,
            const char *filename )
{
    struct wave_signal wave_signal;
    FILE *fp;

    fp = fopen( filename, "wt" );
    if ( !fp )
    {
        return;
    }

    fprintf( fp, "freq,rms\n" );
    wave_signal_init( &wave_signal );
    wave_signal_set(&wave_signal, 1, fs, np);
    
    for ( ; freq0 <= freq1 ; freq0 += freq_step )
    {
        double rms;
        double *d;
        wave_signal_clear( &wave_signal );
        wave_signal_add( &wave_signal, 100, 0, freq0, 0 );
        d = wave_signal.p;
        for ( int k=0; k<np; k++ )
        {
            *d = ftr.filter( *d );
            d++;
        }
        rms = wave_signal_get_rms_skip( &wave_signal, skip);
        fprintf( fp, "%f,%f\n", freq0, rms );
    }
    wave_signal_destroy( &wave_signal );
    fclose( fp );
}


int main (int,char**)
{
	const int order = 6;
	Iir::Butterworth::LowPass<order> f;
	const float samplingrate = 25600; // Hz
	const float cutoff_frequency = 35; // Hz
	f.setup (order, samplingrate, cutoff_frequency);
	f.reset ();
	FILE *fimpulse = fopen("lp.dat","wt");
	for(int i=0;i<1000;i++) 
	{
		float a=0;
		if (i>=10) a = 1;
		float b = f.filter(a);
		fprintf(fimpulse,"%f\n",b);
	}
	fclose(fimpulse);
	
	printf( "%d\n", g_global_v );
	// calculate response at frequency 440 Hz
	Iir::complex_t response = f.response (440./44100);
	
	std::ostringstream os;

    freqz( f, 0, 100, 0.5, samplingrate, samplingrate*2, samplingrate, "abc.csv" );
    
	// coefficients
	int n = f.getNumStages();
	os << "numStages = " << n << "\n";
	for(int i=0; i<n; i++)
	{
		os << "a0[" << i << "] = " << f[i].getA0 () << "\n"
		   << "a1[" << i << "] = " << f[i].getA1 () << "\n"
		   << "a2[" << i << "] = " << f[i].getA2 () << "\n"
		   << "b0[" << i << "] = " << f[i].getB0 () << "\n"
		   << "b1[" << i << "] = " << f[i].getB1 () << "\n"
		   << "b2[" << i << "] = " << f[i].getB2 () << "\n";
	}
	
	std::cout << os.str();


	// bandstop filter
	// here the "direct form I" is chosen for the number crunching
	Iir::Butterworth::BandStop<order,Iir::DirectFormI> bs;
	const float center_frequency = 50;
	const float frequency_width = 5;
	bs.setup (order, samplingrate, center_frequency, frequency_width);
	bs.reset ();
	FILE *fbs = fopen("bs.dat","wt");
	for(int i=0;i<1000;i++) 
	{
		float a=0;
		if (i==10) a = 1;
		float b = bs.filter(a);
		fprintf(fbs,"%f\n",b);
	}
	fclose(fbs);
	


}
