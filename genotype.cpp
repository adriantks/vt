/* The MIT License

   Copyright (c) 2013 Adrian Tan <atks@umich.edu>

   Permission is hereby granted, free of charge, to any person obtaining a copy
   of this software and associated documentation files (the "Software"), to deal
   in the Software without restriction, including without limitation the rights
   to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
   copies of the Software, and to permit persons to whom the Software is
   furnished to do so, subject to the following conditions:

   The above copyright notice and this permission notice shall be included in
   all copies or substantial portions of the Software.

   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
   IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
   AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
   OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
   THE SOFTWARE.
*/

#include "genotype.h"

namespace
{

class Igor : Program
{
    public:

    std::string version;

    ///////////
    //options//
    ///////////
    std::string sample_id;
    std::string ivcf_file;
    std::string isam_file;
    std::string ovcf_file;
    std::string ref_fasta_file;
    bool debug;

    std::vector<GenomeInterval> intervals;

    ///////
    //i/o//
    ///////
    BCFOrderedReader *odr;
    bcf1_t *v;

    BCFOrderedWriter *odw;
    bcf1_t *ov;
    
    BAMOrderedReader *bodr;
    bam1_t *s;

    /////////
    //stats//
    /////////
    uint32_t no_snps_genotyped;
	uint32_t no_indels_genotyped;
    uint32_t noDelRefToAlt;
    uint32_t noDelAltToRef;
    uint32_t noInsRefToAlt;
    uint32_t noInsAltToRef;
    uint32_t readExtendedNo;

    /////////
    //tools//
    /////////
    LogTool lt;
    LHMM lhmm_ref, lhmm_alt;
    
    Igor(int argc, char ** argv)
    {
        //////////////////////////
        //options initialization//
        //////////////////////////
    	try
    	{
    		std::string desc =
"Genotypes variants for each sample.\n\
$path = /net/fantasia/home/atks/programs/vt\n\
e.g. $path/vt genotype -i $path/test/probes.sites.vcf -o out.vcf -b $path/test/NA19130.bam -s NA19130 -g ref.fa\n\n";

       		version = "0.5";
    		TCLAP::CmdLine cmd(desc, ' ', version);
    		TCLAP::ValueArg<std::string> arg_ivcf_file("i", "i", "Input Candidate VCF file", true, "", "string", cmd);
    		TCLAP::ValueArg<std::string> arg_isam_file("b", "b", "Input BAM file", true, "", "string", cmd);
    		TCLAP::ValueArg<std::string> arg_ovcf_file("o", "o", "Output VCF file", false, "-", "string", cmd);
    		TCLAP::ValueArg<std::string> arg_sample_id("s", "s", "Sample ID", true, "", "string", cmd);
    		TCLAP::ValueArg<std::string> arg_ref_fasta_file("r", "r", "Genome FASTA file", false, "/net/fantasia/home/atks/ref/genome/human.g1k.v37.fa", "string", cmd);
    		TCLAP::SwitchArg arg_debug("d", "d", "Debug alignments", cmd, false);

    		cmd.parse(argc, argv);

    		ivcf_file = arg_ivcf_file.getValue();
    		isam_file = arg_isam_file.getValue();
    		ovcf_file = arg_ovcf_file.getValue();
    		sample_id = arg_sample_id.getValue();
    		ref_fasta_file = arg_ref_fasta_file.getValue();
    		debug = arg_debug.getValue();
    	}
    	catch (TCLAP::ArgException &e)
    	{
    		std::cerr << "error: " << e.error() << " for arg " << e.argId() << "\n";
    		abort();
    	}

        //////////////////////
        //i/o initialization//
        //////////////////////
        //input vcf
        odr = new BCFOrderedReader(ivcf_file.c_str(), intervals);
        
        //input sam
        bodr = new BAMOrderedReader(isam_file.c_str(), intervals);
        s = bam_init1();

        //output vcf
        odw = new BCFOrderedWriter(ovcf_file.c_str());
        //added sample early, appending of HLs ensures syncing of the dictionaries.
        bcf_hdr_add_sample(odw->hdr, strdup(sample_id.c_str()));
        bcf_hdr_append(odw->hdr, "##fileformat=VCFv4.1");
        bcf_hdr_append(odw->hdr, "##FORMAT=<ID=GT,Number=1,Type=String,Description=\"Genotype\">");
        //bcf_hdr_append(odw->hdr, "##FORMAT=<ID=GL,Number=G,Type=Float,Description=\"Log likelihoods for genotypes\">");
        bcf_hdr_append(odw->hdr, "##FORMAT=<ID=PL,Number=G,Type=Integer,Description=\"Normalized, Phred-scaled likelihoods for genotypes\">");
        bcf_hdr_append(odw->hdr, "##FORMAT=<ID=DP,Number=1,Type=String,Description=\"Depth\">");
        bcf_hdr_append(odw->hdr, "##FORMAT=<ID=RQ,Number=1,Type=String,Description=\"Normalized, Phred-scaled likelihoods for alleles for each reference read\">");
        bcf_hdr_append(odw->hdr, "##FORMAT=<ID=AQ,Number=1,Type=String,Description=\"Normalized, Phred-scaled likelihoods for alleles for each alternate read\">");
        bcf_hdr_append(odw->hdr, "##FORMAT=<ID=AL,Number=1,Type=String,Description=\"Allele called for each read\">");
        bcf_hdr_append(odw->hdr, "##FORMAT=<ID=CY,Number=1,Type=String,Description=\"Cycle for the location of each variant on each read\">");
        bcf_hdr_append(odw->hdr, "##FORMAT=<ID=RL,Number=1,Type=String,Description=\"Length of each read\">");
        bcf_hdr_append(odw->hdr, "##FORMAT=<ID=MQ,Number=1,Type=String,Description=\"Normalized, Phred-scaled alignment likelihoods for each read\">");
        odw->write_hdr();
        

        //##INFO=<ID=NS,Number=1,Type=Integer,Description="Number of Samples With Data">
        //##INFO=<ID=DP,Number=1,Type=Integer,Description="Total Depth">
        //##INFO=<ID=AF,Number=A,Type=Float,Description="Allele Frequency">
        //##INFO=<ID=AA,Number=1,Type=String,Description="Ancestral Allele">
        //##INFO=<ID=DB,Number=0,Type=Flag,Description="dbSNP membership, build 129">
        //##INFO=<ID=H2,Number=0,Type=Flag,Description="HapMap2 membership">
        //##FILTER=<ID=q10,Description="Quality below 10">
        //##FILTER=<ID=s50,Description="Less than 50% of samples have data">

        ////////////////////////
        //stats initialization//
        ////////////////////////
        no_snps_genotyped = 0;
	    no_indels_genotyped = 0;
        noDelRefToAlt = 0;
        noDelAltToRef = 0;
        noInsRefToAlt = 0;
        noInsAltToRef = 0;
        readExtendedNo = 0;

        ////////////////////////
        //tools initialization//
        ////////////////////////
       
    };

 	void print_options()
    {
        std::clog << "genotype v" << version << "\n\n";

	    std::clog << "Options: [i] Input VCF File   " << ivcf_file << "\n";
	    std::clog << "         [b] Input BAM File   " << isam_file << "\n";
	    std::clog << "         [o] Output VCF File  " << ovcf_file << "\n";
	    std::clog << "         [s] Sample ID        " << sample_id << "\n\n";
        if (intervals.size()!=0)
        {
            std::clog << "         [i] intervals                    ";
            for (uint32_t i=0; i<std::min((uint32_t)intervals.size(),(uint32_t)5); ++i)
            {
                if (i) std::clog << ", ";
                std::clog << intervals[i].to_string();
            }
            if (intervals.size()>=5)
            {
                std::clog << "  and " << (intervals.size()-5) <<  " other intervals\n";
            }   
        } 
        std::clog << "\n";

    }

    void print_stats()
    {
	    std::clog << "Stats: SNPs genotyped     " << no_snps_genotyped << "\n";
	    std::clog << "       Indels genotyped   " << no_indels_genotyped << "\n\n";
    }

 	~Igor()
    {
       odr->close();
       bodr->close();
       odw->close();
    };

    /**
     *Igor does odds jobs too.
     *The most basic model for GLs.
     */
    double log10Emission(char readBase, char probeBase, uint32_t qual)
    {
        double e = lt.pl2prob(qual);

    	if (readBase=='N' || probeBase=='N')
    	{
    		return 0;
    	}

   		return readBase!=probeBase ? log10(e/3) : log10(1-e);
    };

    /**
     *Extend a read to allow for alignment.  This occurs when a variant is found at the edge of a read
     */
    bool extendRead(int32_t variantLengthDifference, int32_t plen, LHMM& lhmmRef, LHMM& lhmmAlt, const char* readSeq, const char* qual, int32_t& readLength)
    {
        bool readIsExtended=false, fivePrimeExtended = false;
        std::string extendedReadSeq, extendedQual;

        //deletion, ref probe is "longer"
        if (variantLengthDifference<0)
        {
            //extend 5' end
            if (lhmmRef.matchStartY == 1 &&
                lhmmRef.matchStartX<=plen+1)
            {
                //std::cerr << "extend 5 prime\n";
                extendedReadSeq.append(lhmmRef.x, lhmmRef.matchStartX-1);
                extendedReadSeq.append(readSeq, strlen(readSeq));

                extendedQual.append(lhmmRef.matchStartX-1, 'I');
                extendedQual.append(qual, readLength);

                fivePrimeExtended = true;
                readIsExtended = true;
            }

            //extend 3' end
            if (lhmmRef.matchEndY==readLength &&
                lhmmRef.matchEndX>=plen+1-variantLengthDifference+1)
            {
                int32_t probeLength = strlen(lhmmRef.x);
                if(!fivePrimeExtended)
                {
                    extendedReadSeq.append(readSeq, readLength);
                    extendedReadSeq.append(lhmmRef.x, lhmmRef.matchEndX, probeLength-lhmmRef.matchEndX);

                    extendedQual.append(qual, readLength);
                    extendedQual.append(probeLength-lhmmRef.matchEndX, 'I');
                }
                else
                {
                    extendedReadSeq.append(lhmmRef.x, lhmmRef.matchEndX, probeLength-lhmmRef.matchEndX);
                    extendedQual.append(probeLength-lhmmRef.matchEndX, 'I');
                }

                readIsExtended = true;
                //std::cerr << "extended read " << extendedReadSeq << "\n";
                //std::cerr << "extended qual " << extendedQual << "\n";
            }
        }
        //insertion, ref probe is "shorter"
        else
        {
            //extend 5' end
            if (lhmmAlt.matchStartY == 1 &&
                lhmmAlt.matchStartX<=plen+1)
            {
                //std::cerr << "extend 5 prime\n";
                extendedReadSeq.append(lhmmAlt.x, lhmmAlt.matchStartX-1);
                extendedReadSeq.append(readSeq, strlen(readSeq));

                extendedQual.append(lhmmAlt.matchStartX-1, 'I');
                extendedQual.append(qual, readLength);

                fivePrimeExtended = true;
                readIsExtended = true;
            }

            //extend 3' end
            if (lhmmAlt.matchEndY==readLength &&
                lhmmAlt.matchEndX>=plen+1+variantLengthDifference+1)
            {
                int32_t probeLength = strlen(lhmmAlt.x);
                if(!fivePrimeExtended)
                {
                    extendedReadSeq.append(readSeq, readLength);
                    extendedReadSeq.append(lhmmAlt.x, lhmmAlt.matchEndX, probeLength-lhmmAlt.matchEndX);

                    extendedQual.append(qual, readLength);
                    extendedQual.append(probeLength-lhmmAlt.matchEndX, 'I');
                }
                else
                {
                    extendedReadSeq.append(lhmmAlt.x, lhmmAlt.matchEndX, probeLength-lhmmAlt.matchEndX);
                    extendedQual.append(probeLength-lhmmAlt.matchEndX, 'I');
                }

                readIsExtended = true;
            }
        }

        if (readIsExtended)
        {
            readSeq = strdup(extendedReadSeq.c_str());
            qual = strdup(extendedQual.c_str());
            readLength = extendedReadSeq.size();
        }

        return readIsExtended;

    }

    void swap(double& a, double& b)
    {
        b = (a=a+b) - b;
        a -= b;
    }

    void genotype_mnp(Igor& igor)
    {
        // pick up all reads
        // realign
    }

    //print in binary values for checking purposes
    std::string print_binary(uint16_t i)
    {
        std::stringstream ss;
        for (uint32_t j=0; j<16; ++j)
        {
            ss << ((i%2)==1);
            i = i>>1;
        }

        std::string s = ss.str();
        s = std::string(s.rbegin(), s.rend());
        return s ;
    }

    void print_read_alignment(kstring_t& read, kstring_t& qual, kstring_t& cigar, 
                              kstring_t& aligned_read, kstring_t& aligned_qual, kstring_t& expanded_cigar, kstring_t& annotations,
                              uint32_t rpos)
    {
    	aligned_read.l = 0;
    	aligned_qual.l = 0;
     	annotations.l = 0;
    	
    	uint32_t j=0;
    	for (uint32_t i=0; i<expanded_cigar.l; ++i)
    	{
    	    char state = expanded_cigar.s[i];
    	    if (state=='M' || state=='S' || state=='I')
    	    {
    	        kputc(read.s[j], &aligned_read);
    	        kputc(qual.s[j], &aligned_qual);
    	        
    	        if (j==rpos)
    	        {    
    	            kputc('^', &annotations);
    	        }
    	        else
    	        {    
    	            kputc(' ', &annotations);
    	        }
    	        ++j;
    	    }
    	    else if (state=='D')
    	    {
                kputc(' ', &aligned_read);
    	        kputc(' ', &aligned_qual);
    	    }
    	}
    }

    /**
     Computes genotype likelihoods and writes out the record
    */
    void genotype_snp()
    {
        if (v->n_allele==2)
        {
            std::stringstream region;
            hts_itr_t *iter;
            std::map<std::string, int> read_ids;

            double log_p_rr = 0;
            double log_p_ra = 0;
            double log_p_aa = 0;

            std::stringstream rqs;
         	std::stringstream aqs;
         	std::stringstream als;
        	std::stringstream rls;
            std::stringstream cys;
        	std::stringstream mqs;

       	    rqs.str("");
    		aqs.str("");
    		als.str("");
    		rls.str("");
    		cys.str("");
    		mqs.str("");

            const char* chrom = bcf_get_chrom(odr->hdr, v);
            uint32_t pos = bcf_get_pos0(v);
            char ref = bcf_get_snp_ref(v);
            char alt = bcf_get_snp_alt(v);
            uint32_t read_no = 0;

            while(bodr->read(s))
            {
                //has secondary alignment or fail QC or is duplicate or is unmapped
                if (bam_get_flag(s) & 0x0704)
                {
                    //std::cerr << "fail flag\n";
                    continue;
                }

                //make this setable
                if(bam_get_mapq(s)<20)
    	        {
    	            //std::cerr << "low mapq\n";
    	            //ignore poor quality mappings
    	            continue;
    	        }

    	        //std::map<std::string, int> readIDs;
    			if(read_ids.find(bam_get_qname(s))==read_ids.end())
    			{
    			    //assign id to reads to ease checking of overlapping reads
    				read_ids[bam_get_qname(s)] = read_ids.size();
    			}
    			//ignore overlapping paired end
    			else
    		    {
    		        //std::cerr << "Mate pair\n";
    		        continue;
    		    }

                //get base and qual
                char base, qual; int32_t rpos;
                kstring_t readseq;
                readseq.l = readseq.m = 0; readseq.s = 0;
                kstring_t readqual;
                readqual.l = readqual.m = 0; readqual.s = 0;

    			bam_get_base_and_qual_and_read_and_qual(s, v->pos, base, qual, rpos, &readseq, &readqual);

                //fail to find a mapped base on the read
                if (rpos==BAM_READ_INDEX_NA)
                {
                    std::cerr << "rpos NA\n";
                    continue;
                }

                //ignore ambiguous bases
                if (base=='N')
                {
                    std::cerr << "N allele\n";
                    continue;
                }

                //strand
                char strand = bam_is_rev(s) ? 'R' : 'F';

                //read length
                uint32_t readLength =  bam_get_l_qseq(s);

                //cycle position
    			int32_t cycle =  strand=='R' ? bam_get_l_qseq(s) - rpos : rpos;

                uint32_t mapQual = bam_get_mapq(s);

    			//////////////////////////////////////////////////
    			//perform GL computation here, uses map alignments
    			//////////////////////////////////////////////////
                std::string refCigar = "";
                std::string altCigar = "";

     	        //compute genotype likelihood using alignment coordinates
     	        double refllk = log10Emission(ref, base, qual);
    	        double altllk = log10Emission(alt, base, qual);

                ////////////////////////////////
                //aggregate genotype likelihoods
                ////////////////////////////////
                log_p_rr = lt.log10prod(log_p_rr, refllk);
                log_p_ra = lt.log10prod(log_p_ra, lt.log10prod(-log10(2), lt.log10sum(refllk, altllk)));
                log_p_aa = lt.log10prod(log_p_aa, altllk);

                uint32_t baseqr = 0, baseqa = 0;
                char allele = strand=='F' ? 'N' : 'n';
                if (refllk>altllk)
                {
                	allele = strand=='F' ? 'R' : 'r';

                    baseqa = round(-10*(altllk-refllk));
                }
                else if (refllk<altllk)
                {
                	allele = strand=='F' ? 'A' : 'a';

              	    baseqr = round(-10*(refllk-altllk));
                }

                if (debug)
                {
                    kstring_t cigar;
                    cigar.l = cigar.m = 0; cigar.s = 0;
                    bam_get_cigar_string(s, &cigar);

                    kstring_t aligned_read;
                	aligned_read.l = aligned_read.m = 0;
                	aligned_read.s = 0;

                	kstring_t aligned_qual;
                	aligned_qual.l = aligned_qual.m = 0;
                	aligned_qual.s = 0;

                	kstring_t expanded_cigar;
                	expanded_cigar.l = expanded_cigar.m = 0;
                	expanded_cigar.s = 0;

                	kstring_t annotations;
                	annotations.l = annotations.m = 0;
                	annotations.s = 0;

                    std::string pad = "\t";
    		        std::cerr << pad << "==================\n";
    		        std::cerr << pad <<  (read_no+1) << ") " << chrom << ":" << (pos+1) << ":" << ref << ":" << alt << "\n";
                    //std::cerr << pad <<  read_name << "\n";
                    std::cerr << pad << "==================\n";
                    print_read_alignment(readseq, readqual, cigar, aligned_read, aligned_qual, expanded_cigar, annotations, rpos);

                    std::cerr << pad << "read  " << aligned_read.s  << "\n";
                    std::cerr << pad << "qual  " << aligned_qual.s  << "\n";
                    std::cerr << pad << "cigar " << expanded_cigar.s  << "\n";
                    std::cerr << pad << "anno  " << annotations.s  << "\n";
                    std::cerr << pad << "==================\n";
                    std::cerr << pad << "base   " << base  << "\n";
                    std::cerr << pad << "qual   " << (int32_t)(qual-33)  << "\n";
                    std::cerr << pad << "rpos   " << rpos  << "\n";
        			std::cerr << pad << "refllk " << refllk << "\n";
        			std::cerr << pad << "altllk " << altllk << "\n";
        			std::cerr << pad << "allele " << allele << "\n\n";
        		}

                rqs << (uint8_t)(baseqr>67? 126 : baseqr+59);
    			aqs << (uint8_t)(baseqa>67? 126 : baseqa+59);
    			als << allele;
    			uint8_t cy1 = cycle > 67 ? 126 : cycle+59;
    			uint8_t cy2 = cycle > 67 ? cycle-93+59 : 59;
    			cys << cy1 << cy2;
    			uint8_t rl1 = readLength > 67 ? 126 : readLength+59;
    			uint8_t rl2 = readLength > 67 ? readLength-93+59 : 59;
    			rls << rl1 << rl2;
    			mqs << (uint8_t) (mapQual+59);

    			++read_no;
            }

            //////////////////////////////////////////
            //compute PHRED scores and assign genotype
            //////////////////////////////////////////
    		uint32_t pl_rr = (uint32_t) round(-10*log_p_rr);
    		uint32_t pl_ra = (uint32_t) round(-10*log_p_ra);
    		uint32_t pl_aa = (uint32_t) round(-10*log_p_aa);

    		uint32_t min = pl_rr;
    		std::string bestGenotype = "0/0";
    		if (pl_ra < min)
    		{
    		    min = pl_ra;
    		    bestGenotype = "0/1";
    		}
    		if (pl_aa < min)
    		{
    		    min = pl_aa;
    		    bestGenotype = "1/1";
    		}

    		pl_rr -= min;
    		pl_ra -= min;
    		pl_aa -= min;

    		if (pl_rr+pl_ra+pl_aa==0)
    		{
    		    bestGenotype = "./.";
    	    }

    		std::stringstream ss;
    		ss << chrom << "\t" << (pos+1) << "\t.\t" << ref
    				<< "\t" << alt << "\t.\t.\t." << "\tGT:PL:DP:RQ:AQ:AL:CY:RL:MQ\t";

    		if (read_no!=0)
    		{
    			ss << bestGenotype << ":"
    			   << pl_rr << "," << pl_ra << "," << pl_aa << ":"
    		       << read_no << ":"
    		       << rqs.str() << ":"
    		       << aqs.str() << ":"
    		       << als.str() << ":"
    		       << cys.str() << ":"
    		       << rls.str() << ":"
    			   << mqs.str() ;
    		}
    		else
    		{
    			ss << "./.";
    		}

    		kstring_t str;
    		str.l = str.m = 0; str.s = 0;
    		kputs(ss.str().c_str(), &str);
    		odw->write(ov);
        }
        else
        {
            //multiallelics to be implemented
        }
    }

    void genotype_indel()
    {
        bool readIsExtended = false;

        if (v->n_allele==2)
        {
            std::stringstream region;
            hts_itr_t *iter;
            std::map<std::string, int> read_ids;

            double log_p_rr = 0;
            double log_p_ra = 0;
            double log_p_aa = 0;

            std::stringstream rqs;
         	std::stringstream aqs;
         	std::stringstream als;
        	std::stringstream rls;
            std::stringstream cys;
        	std::stringstream mqs;

        	rqs.str("");
    		aqs.str("");
    		als.str("");
    		rls.str("");
    		cys.str("");
    		mqs.str("");

            const char* chrom = bcf_get_chrom(odr->hdr, v);
            uint32_t pos = bcf_get_pos0(v);
            char* ref = bcf_get_ref(v);
            char* alt = bcf_get_alt(v, 1);

            //generate probe
            //uint32_t chromNo = hapgen->getChromNo(chrom);
            std::vector<std::string> candidate_alleles;
            candidate_alleles.push_back(ref);
            candidate_alleles.push_back(alt);
            uint32_t plen;
            std::vector<std::string> probes;
            
            const char* refProbe = probes[0].c_str();
    		const char* altProbe = probes[1].c_str();
    		int32_t probeLength = probes[0].size();

//            if (pos!=62812113)
//            {
//                return;
//            }
//
//            std::cerr << "ref: " << refProbe << "\n";
//            std::cerr << "alt: " << altProbe << "\n";
//            std::cerr << "len: " << probeLength << "\n";
//            std::cerr << "plen: " << plen << "\n";

            //int32_t variantStartPos = pos;
            int32_t variantLengthDifference = (int32_t)strlen(alt)-(int32_t)strlen(ref);

            
            uint32_t read_no = 0;
            while(bodr->read(s))
            {
                //has secondary alignment or fail QC or is duplicate or is unmapped
                if (bam_get_flag(s) & 0x0704)
                {
                    //std::cerr << "fail flag\n";
                    continue;
                }

                //make this setable
                if(bam_get_mapq(s)<13)
    	        {
    	            //std::cerr << "low mapq\n";
    //              ++readFilteredNo;
    	            //ignore poor quality mappings
    	            continue;
    	        }

    	        //std::map<std::string, int> readIDs;
    			if(read_ids.find(bam_get_qname(s))==read_ids.end())
    			{
    			    //assign id to reads to ease checking of overlapping reads
    				read_ids[bam_get_qname(s)] = read_ids.size();
    			}
    			//ignore overlapping paired end
    			else
    		    {
    		        //std::cerr << "Mate pair\n";
    		        continue;
    		    }

                //get base and qual
                kstring_t str;
    		    str.l = str.m = 0; str.s = 0;

                //read name
                char* read_name = bam_get_qname(s);

                //sequence
                bam_get_seq_string(s, &str);
                char* read_seq = strdup(str.s);

                //qual
                bam_get_qual_string(s, &str);
                char* qual = strdup(str.s);

                //strand
                char strand = bam_is_rev(s) ? 'R' : 'F';

                //read length
                uint32_t readLength =  bam_get_l_qseq(s);

                //map quality
                uint32_t mapQual = bam_get_mapq(s);

                int32_t cycle = 0;
    			//////////////////////////
    			//perform realignment here
    			//////////////////////////

    			//////////////////////////////////////////////////
    			//perform GL computation here, uses map alignments
    			//////////////////////////////////////////////////
                std::string refCigar = "";
                std::string altCigar = "";

                GENOTYPE:

                double refllk = 0, altllk = 0;

    		   	lhmm_ref.align(refllk, refProbe, read_seq, qual);
    	        lhmm_ref.computeLogLikelihood(refllk, lhmm_ref.getPath(), qual);

    	        lhmm_alt.align(altllk, altProbe, read_seq, qual);
                lhmm_alt.computeLogLikelihood(altllk, lhmm_alt.getPath(), qual);

                std::string pad = "\t";
    	        if (readIsExtended)
    	            pad = "\t\t";

    	        if (debug)
    	        {
    		        std::cerr << pad << "==================\n";
    		        std::cerr << pad <<  (read_no+1) << ") " << chrom << ":" << (pos+1) << ":" << ref << ":" << alt << ":" << variantLengthDifference << "\n";
                    std::cerr << pad <<  read_name << "\n";
                    std::cerr << pad << "==================\n";
                    std::cerr << pad << "ref probe     " << refProbe << " (" << plen << "/" << probeLength << ")\n";
                    std::cerr << pad << "read sequence " << read_seq  << "\n";
                    std::cerr << pad << "qual          " << qual  << "\n";
                    std::cerr << pad << "==================\n";
                    lhmm_ref.printAlignment(pad);
                    std::cerr << pad << "ref llk: " << refllk << "\n";
    		        std::cerr << pad << "expected indel location: " << plen+1 << "\n";
    		        std::cerr << pad << "==================\n";
    		        std::cerr << pad << "==================\n";
                    std::cerr << pad << "alt probe     " << altProbe << " (" << plen << ")\n";
                    std::cerr << pad << "read sequence " << read_seq  << "\n";
                    std::cerr << pad << "qual          " << qual  << "\n";
                    std::cerr << pad << "==================\n";
                    lhmm_alt.printAlignment(pad);
                    std::cerr << pad << "alt llk: " << altllk << "\n";
                    std::cerr << pad << "expected indel location: " << plen+1 << "\n";
    		        std::cerr << pad << "==================\n\n";
    	        }

                //Compare alignment segments based on position with greatest match and highest log odds score.
                //std::string& alignmentPath = lhmmRef.path;

                //check if the Insertions and Deletions are at the expected places.
                //deletion
                uint32_t ref_rpos=0, alt_rpos=0;
                if (variantLengthDifference<0)
                {
                    //deletion
                    if (!deletion_start_exists(lhmm_ref, plen+1, 0, ref_rpos) &&
                         insertion_start_exists(lhmm_alt, plen+1, 0, alt_rpos))
                    {
                        if (refllk<altllk)
                        {
                            ++noDelRefToAlt;
                            swap(refllk, altllk);
                        }

                        //cycle position
                        cycle = strand=='R' ? readLength - alt_rpos : alt_rpos;

                        //ref allele
                        if (debug)
                            std::cerr << pad << "DEL: REF ALLELE\n";
                    }
                    else if (deletion_start_exists(lhmm_ref, plen+1, 0, ref_rpos) &&
                            !insertion_start_exists(lhmm_alt, plen+1, 0, alt_rpos))
                    {
                        if (refllk>altllk)
                        {
                            ++noDelAltToRef;
                            swap(refllk, altllk);
                        }

                        //cycle position
                        cycle = strand=='R' ? readLength - ref_rpos : ref_rpos;

                        //alt allele
                        if (debug)
                            std::cerr << pad << "DEL: ALT ALLELE\n";
                    }
                    else
                    {
                        //artificially attempt to extend read (only try once)
                        if (0 && !readIsExtended)
                        {
                            //int32_t variantLengthDifference, int32_t plen, LHMM& lhmm_ref, LHMM& lhmm_alt, char* readSeq, char* qual, int32_t& readLength

                            //readIsExtended = extendRead(variantLengthDifference, (int32_t)plen, lhmm_ref, lhmm_alt, read_seq, qual, readLength);

                            if (readIsExtended)
                            {
                                ++readExtendedNo;
                                goto GENOTYPE;
                            }
                        }

                        refllk = 0;
                        altllk = 0;
                        if (debug)
                            std::cerr << pad << "Deletion not at expected location, set as ambiguous\n";
                    }
                }
                else if (variantLengthDifference>0)
                {
                    //insertion
                    if (!insertion_start_exists(lhmm_ref, plen+1, 0, ref_rpos) &&
                         deletion_start_exists(lhmm_alt, plen+1, 0, alt_rpos))
                    {
                        if (refllk<altllk)
                        {
                            ++noInsRefToAlt;

                            swap(refllk, altllk);
                        }

                        //cycle position
                        cycle = strand=='R' ? readLength - alt_rpos : alt_rpos;

                        //ref allele
                        if (debug)
                            std::cerr << pad << "INS: REF ALLELE\n";
                    }
                    else if (insertion_start_exists(lhmm_ref, plen+1, 0, ref_rpos) &&
                            !deletion_start_exists(lhmm_alt, plen+1, 0, alt_rpos))
                    {
                        if (refllk>altllk)
                        {
                            ++noInsAltToRef;
                            swap(refllk, altllk);
                        }

                        //cycle position
                        cycle = strand=='R' ? readLength - ref_rpos : ref_rpos;

                        //alt allele
                        if (debug)
                            std::cerr << pad << "INS: ALT ALLELE\n";
                    }
                    else
                    {
                        //artificially attempt to extend read (only try once)
                        if (0 && !readIsExtended)
                        {
                            //readIsExtended = extendRead(variantLengthDifference, (int32_t)plen, lhmm_ref, lhmm_alt, read_seq, qual, readLength);
                            if (readIsExtended)
                            {
    //                            std::cerr << "extended read " << readSeq << "\n";
    //                            std::cerr << "extended qual " << qual << "\n";
    //                            std::cerr << "read length " << readLength << "\n";
                                ++readExtendedNo;
                                goto GENOTYPE;
                            }
                        }

                        refllk = 0;
                        altllk = 0;
                        if (debug)
                            std::cerr << pad << "Insertion not at expected location "  << plen+1 << ", set as ambiguous\n";
                    }
                }

    	        if (debug)
    	        {
    		        std::cerr << pad << "++++++++++++++++++\n";
    	            std::cerr << pad << "reflk " << refllk << "\n";
    			    std::cerr << pad << "altlk " << altllk << "\n";
                }

                ////////////////////////////////
                //aggregate genotype likelihoods
                ////////////////////////////////
                log_p_rr = lt.log10prod(log_p_rr, refllk);
                log_p_ra = lt.log10prod(log_p_ra, lt.log10prod(-log10(2), lt.log10sum(refllk, altllk)));
                log_p_aa = lt.log10prod(log_p_aa, altllk);

                uint32_t baseqr = 0, baseqa = 0;
                char allele = strand=='F' ? 'N' : 'n';
                if (refllk>altllk)
                {
                	allele = strand=='F' ? 'R' : 'r';
                    baseqa = round(-10*(altllk-refllk));
                }
                else if (refllk<altllk)
                {
                	allele = strand=='F' ? 'A' : 'a';
              	    baseqr = round(-10*(refllk-altllk));
                }

                rqs << (uint8_t)(baseqr>67? 126 : baseqr+59);
    			aqs << (uint8_t)(baseqa>67? 126 : baseqa+59);
    			als << allele;
    			uint8_t cy1 = cycle > 67 ? 126 : cycle+59;
    			uint8_t cy2 = cycle > 67 ? cycle-67+59 : 59;
    			cys << cy1 << cy2;
    			uint8_t rl1 = readLength > 67 ? 126 : readLength+59;
    			uint8_t rl2 = readLength > 67 ? readLength-67+59 : 59;
    			rls << rl1 << rl2;
    			mqs << (uint8_t) (mapQual+59);

    			++read_no;
            }

            //////////////////////////////////////////
            //compute PHRED scores and assign genotype
            //////////////////////////////////////////
    		uint32_t pl_rr = (uint32_t) round(-10*log_p_rr);
    		uint32_t pl_ra = (uint32_t) round(-10*log_p_ra);
    		uint32_t pl_aa = (uint32_t) round(-10*log_p_aa);

    		uint32_t min = pl_rr;
    		std::string bestGenotype = "0/0";
    		if (pl_ra < min)
    		{
    		    min = pl_ra;
    		    bestGenotype = "0/1";
    		}
    		if (pl_aa < min)
    		{
    		    min = pl_aa;
    		    bestGenotype = "1/1";
    		}

    		pl_rr -= min;
    		pl_ra -= min;
    		pl_aa -= min;

    		if (pl_rr+pl_ra+pl_aa==0)
    		{
    		    bestGenotype = "./.";
    	    }

    		std::stringstream ss;
    		ss << chrom << "\t" << (pos+1) << "\t.\t" << ref
    				<< "\t" << alt << "\t.\t.\t." << "\tGT:PL:DP:RQ:AQ:AL:CY:RL:MQ\t";

            //std::cerr << "reads " << read_no << "\n";

    		if (read_no!=0)
    		{
    			ss << bestGenotype << ":"
    			   << pl_rr << "," << pl_ra << "," << pl_aa << ":"
    		       << read_no << ":"
    		       << rqs.str() << ":"
    		       << aqs.str() << ":"
    		       << als.str() << ":"
    		       << cys.str() << ":"
    		       << rls.str() << ":"
    			   << mqs.str() ;
    		}
    		else
    		{
    			ss << "./.";
    		}

    		kstring_t str;
    		str.l = str.m = 0; str.s = 0;
    		kputs(ss.str().c_str(), &str);
    		odw->write(ov);
        }
        else
        {
            //multiallelics to be implemented
        }
    }

    void genotype()
    {
        while (odr->read(v))
        {
            bcf_unpack(v, BCF_UN_STR);
           // bcf_set_variant(v);

            if (v->d.var_type == VCF_SNP)
            {
                ++no_snps_genotyped;
                genotype_snp();
            }
            else if (v->d.var_type == VCF_MNP)
            {
                //genotype_mnp(igor);
            }
            else if (v->d.var_type == VCF_INDEL)
            {
                ++no_indels_genotyped;
                genotype_indel();
            }
            else
            {
                //std::cerr << ivcf_
            }
        }
    }

    private:
    bool deletion_start_exists(LHMM& lhmm, uint32_t pos, uint32_t len, uint32_t& rpos)
    {
        for (uint32_t i=0; i<lhmm.indelStatusInPath.size(); ++i)
        {
            if (lhmm.indelStatusInPath[i]=='D' &&
                lhmm.indelStartsInX[i]==pos)
            {
                rpos = lhmm.indelStartsInY[i];
                return true;
            }
        }

        return false;
    }

    bool insertion_start_exists(LHMM& lhmm, uint32_t pos, uint32_t len, uint32_t& rpos)
    {
        for (uint32_t i=0; i<lhmm.indelStatusInPath.size(); ++i)
        {
            if (lhmm.indelStatusInPath[i]=='I' &&
                lhmm.indelStartsInX[i]==pos)
            {
                rpos = lhmm.indelStartsInY[i];
                return true;
            }
        }

        return false;
    }
};

}

void genotype(int argc, char ** argv)
{
    Igor igor(argc, argv);
    igor.print_options();
    igor.genotype();
    igor.print_stats();
}

