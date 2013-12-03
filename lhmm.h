#ifndef LHMM_H
#define LHMM_H

#include "utils.h"
#include "log_tool.h"
#include <regex.h>

class LHMM
{
	public:
	const char* x;
	const char* y;
	const char* qual;
	
	    
	std::vector<std::vector<double> > X;
    std::vector<std::vector<double> > Y;
	std::vector<std::vector<double> > M;
	std::vector<std::vector<double> > I;
	std::vector<std::vector<double> > D;
	std::vector<std::vector<double> > W;
	std::vector<std::vector<double> > Z;   
	std::vector<std::vector<char> > pathX;	
    std::vector<std::vector<char> > pathY;	
    std::vector<std::vector<char> > pathM;	
    std::vector<std::vector<char> > pathD;	
    std::vector<std::vector<char> > pathI;	
    std::vector<std::vector<char> > pathW;	
    std::vector<std::vector<char> > pathZ;	
    std::vector<double> PLs; 
        
    uint32_t maxLength;
    uint32_t xlen;
	uint32_t ylen;
	std::string path;
	double maxLogOdds;
	    
    double delta;
    double epsilon;
	double tau;
    double eta;
    double logOneSixteenth;
    
//    double a;
//    double e;
//    double d;
//    double c;
    double logEta;
    double logTau;
    
    
    double tsx;
    double txx;
    double tsy;
    double txy;
    double tyy;
    double txm;
	double tym;
	double txi;
	double txd;
	double tsm;
	double tmm;
	double tim;
	double tdm;
	double tmi;
	double tmd;
	double tii;
	double tdd;
    double tyi;
    double tyd;
    double tmw;
    double tiw;
    double tdw;
    double tww;
    double tmz;
    double tiz;
    double tdz;
    double twz;
    double tzz;    
    
    double tMM;
    double tMI;
    double tMD;
    double tII;
    double tDD;
    double tDM;
    double tIM;
    
    int32_t matchStartX;
    int32_t matchEndX;
    int32_t matchStartY;
    int32_t matchEndY;
    int32_t matchedBases;
    int32_t mismatchedBases;
        
    std::vector<uint32_t> indelStartsInX;
    std::vector<uint32_t> indelEndsInX;
    std::vector<uint32_t> indelStartsInY;
    std::vector<uint32_t> indelEndsInY;    
    std::vector<uint32_t> indelStartsInPath;
    std::vector<uint32_t> indelEndsInPath;
    std::vector<char> indelStatusInPath;
          
    uint32_t noBasesAligned;

    LogTool lt;

    /*Constructor*/
	LHMM();

	bool containsIndel();

    /**
	convert PLs to probabilities.
	*/
	double pl2prob(uint32_t PL);
	
	/**
	 *Updates matchStart, matchEnd, globalMaxPath and path
	 *Updates locations of insertions and deletions
	 */
	void tracePath();
	void tracePath(std::stringstream& ss, char state, uint32_t i, uint32_t j);
	
	/**
	Align and compute genotype likelihood.
	*/
	void align(double& llk, const char* _x, const char* _y, const char* qual, bool debug=false);

    //computes log likelihood based on path given in arguments
    void computeLogLikelihood(double& llk, double& perfectllk, std::string& _path, const char* qual);

    //computes log likelihood based on path given in arguments
    void computeLogLikelihood(double& llk, std::string& _path, const char* qual);

    //computes log likelihood based on path saved
    void computeLogLikelihood(double& llk, const char* qual);

    //get path
    std::string& getPath();

    /**
     *Left align indels in an alignment
     */
    void left_align();
	
	double logEmissionOdds(char readBase, char probeBase, double e);
	
	//compute log emission based on equal error probability distribution
	double logEmission(char readBase, char probeBase, double e);
	
	double emission(char readBase, char probeBase, double e);

	std::string reverse(std::string s);
	
	void printVector(std::vector<std::vector<double> >& v, uint32_t xLen, uint32_t yLen);
	
	void printVector(std::vector<std::vector<char> >& v, uint32_t xLen, uint32_t yLen);
	
	void printVector(std::vector<std::vector<double> >& v);
	
	void printAlignment();
		
	void printAlignment(std::string& pad);
	
	void printAlignment(std::string& pad, std::stringstream& log);
	    
	double score(char a, char b);
};

#endif