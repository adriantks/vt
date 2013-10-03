/* The MIT License

   Copyright (c) 2008 Broad Institute / Massachusetts Institute of Technology
                 2011, 2012 Attractive Chaos <attractor@live.co.uk>

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

#include "hts_utils.h"

KHASH_MAP_INIT_STR(vdict, bcf_idinfo_t) typedef khash_t(vdict) vdict_t;
static bcf_idinfo_t bcf_idinfo_def = { { 15, 15, 15 }, { NULL, NULL, NULL}, -1 };

int hts_write(htsFile *fp)
{
	if (!fp->is_bin) {
		fwrite(fp->line.s, 1, fp->line.l, (FILE*)fp->fp);
	} else {
    	xbgzf_write((BGZF*) fp->fp, fp->line.s, fp->line.l);
	}
	return 0;
}


/**
 *Adds the contigs required for build version hs37b5
 */
void bcf_add_hs37d5_contig_headers(bcf_hdr_t *hdr)
{
    bcf_hdr_append(hdr, "##contig=<ID=1,length=249250621,assembly=b37>");
	bcf_hdr_append(hdr, "##contig=<ID=2,length=243199373,assembly=b37>");
	bcf_hdr_append(hdr, "##contig=<ID=3,length=198022430,assembly=b37>");
	bcf_hdr_append(hdr, "##contig=<ID=4,length=191154276,assembly=b37>");
	bcf_hdr_append(hdr, "##contig=<ID=5,length=180915260,assembly=b37>");
	bcf_hdr_append(hdr, "##contig=<ID=6,length=171115067,assembly=b37>");
	bcf_hdr_append(hdr, "##contig=<ID=7,length=159138663,assembly=b37>");
	bcf_hdr_append(hdr, "##contig=<ID=8,length=146364022,assembly=b37>");
	bcf_hdr_append(hdr, "##contig=<ID=9,length=141213431,assembly=b37>");
	bcf_hdr_append(hdr, "##contig=<ID=10,length=135534747,assembly=b37>");
	bcf_hdr_append(hdr, "##contig=<ID=11,length=135006516,assembly=b37>");
	bcf_hdr_append(hdr, "##contig=<ID=12,length=133851895,assembly=b37>");
	bcf_hdr_append(hdr, "##contig=<ID=13,length=115169878,assembly=b37>");
	bcf_hdr_append(hdr, "##contig=<ID=14,length=107349540,assembly=b37>");
	bcf_hdr_append(hdr, "##contig=<ID=15,length=102531392,assembly=b37>");
	bcf_hdr_append(hdr, "##contig=<ID=16,length=90354753,assembly=b37>");
	bcf_hdr_append(hdr, "##contig=<ID=17,length=81195210,assembly=b37>");
	bcf_hdr_append(hdr, "##contig=<ID=18,length=78077248,assembly=b37>");
	bcf_hdr_append(hdr, "##contig=<ID=19,length=59128983,assembly=b37>");
	bcf_hdr_append(hdr, "##contig=<ID=20,length=63025520,assembly=b37>");
	bcf_hdr_append(hdr, "##contig=<ID=21,length=48129895,assembly=b37>");
	bcf_hdr_append(hdr, "##contig=<ID=22,length=51304566,assembly=b37>");
	bcf_hdr_append(hdr, "##contig=<ID=X,length=155270560,assembly=b37>");
	bcf_hdr_append(hdr, "##contig=<ID=Y,length=59373566,assembly=b37>");
	bcf_hdr_append(hdr, "##contig=<ID=MT,length=16569,assembly=b37>");
	bcf_hdr_append(hdr, "##contig=<ID=GL000207.1,length=4262,assembly=b37>");
	bcf_hdr_append(hdr, "##contig=<ID=GL000226.1,length=15008,assembly=b37>");
	bcf_hdr_append(hdr, "##contig=<ID=GL000229.1,length=19913,assembly=b37>");
	bcf_hdr_append(hdr, "##contig=<ID=GL000231.1,length=27386,assembly=b37>");
	bcf_hdr_append(hdr, "##contig=<ID=GL000210.1,length=27682,assembly=b37>");
	bcf_hdr_append(hdr, "##contig=<ID=GL000239.1,length=33824,assembly=b37>");
	bcf_hdr_append(hdr, "##contig=<ID=GL000235.1,length=34474,assembly=b37>");
	bcf_hdr_append(hdr, "##contig=<ID=GL000201.1,length=36148,assembly=b37>");
	bcf_hdr_append(hdr, "##contig=<ID=GL000247.1,length=36422,assembly=b37>");
	bcf_hdr_append(hdr, "##contig=<ID=GL000245.1,length=36651,assembly=b37>");
	bcf_hdr_append(hdr, "##contig=<ID=GL000197.1,length=37175,assembly=b37>");
	bcf_hdr_append(hdr, "##contig=<ID=GL000203.1,length=37498,assembly=b37>");
	bcf_hdr_append(hdr, "##contig=<ID=GL000246.1,length=38154,assembly=b37>");
	bcf_hdr_append(hdr, "##contig=<ID=GL000249.1,length=38502,assembly=b37>");
	bcf_hdr_append(hdr, "##contig=<ID=GL000196.1,length=38914,assembly=b37>");
	bcf_hdr_append(hdr, "##contig=<ID=GL000248.1,length=39786,assembly=b37>");
	bcf_hdr_append(hdr, "##contig=<ID=GL000244.1,length=39929,assembly=b37>");
	bcf_hdr_append(hdr, "##contig=<ID=GL000238.1,length=39939,assembly=b37>");
	bcf_hdr_append(hdr, "##contig=<ID=GL000202.1,length=40103,assembly=b37>");
	bcf_hdr_append(hdr, "##contig=<ID=GL000234.1,length=40531,assembly=b37>");
	bcf_hdr_append(hdr, "##contig=<ID=GL000232.1,length=40652,assembly=b37>");
	bcf_hdr_append(hdr, "##contig=<ID=GL000206.1,length=41001,assembly=b37>");
	bcf_hdr_append(hdr, "##contig=<ID=GL000240.1,length=41933,assembly=b37>");
	bcf_hdr_append(hdr, "##contig=<ID=GL000236.1,length=41934,assembly=b37>");
	bcf_hdr_append(hdr, "##contig=<ID=GL000241.1,length=42152,assembly=b37>");
	bcf_hdr_append(hdr, "##contig=<ID=GL000243.1,length=43341,assembly=b37>");
	bcf_hdr_append(hdr, "##contig=<ID=GL000242.1,length=43523,assembly=b37>");
	bcf_hdr_append(hdr, "##contig=<ID=GL000230.1,length=43691,assembly=b37>");
	bcf_hdr_append(hdr, "##contig=<ID=GL000237.1,length=45867,assembly=b37>");
	bcf_hdr_append(hdr, "##contig=<ID=GL000233.1,length=45941,assembly=b37>");
	bcf_hdr_append(hdr, "##contig=<ID=GL000204.1,length=81310,assembly=b37>");
	bcf_hdr_append(hdr, "##contig=<ID=GL000198.1,length=90085,assembly=b37>");
	bcf_hdr_append(hdr, "##contig=<ID=GL000208.1,length=92689,assembly=b37>");
	bcf_hdr_append(hdr, "##contig=<ID=GL000191.1,length=106433,assembly=b37>");
	bcf_hdr_append(hdr, "##contig=<ID=GL000227.1,length=128374,assembly=b37>");
	bcf_hdr_append(hdr, "##contig=<ID=GL000228.1,length=129120,assembly=b37>");
	bcf_hdr_append(hdr, "##contig=<ID=GL000214.1,length=137718,assembly=b37>");
	bcf_hdr_append(hdr, "##contig=<ID=GL000221.1,length=155397,assembly=b37>");
	bcf_hdr_append(hdr, "##contig=<ID=GL000209.1,length=159169,assembly=b37>");
	bcf_hdr_append(hdr, "##contig=<ID=GL000218.1,length=161147,assembly=b37>");
	bcf_hdr_append(hdr, "##contig=<ID=GL000220.1,length=161802,assembly=b37>");
	bcf_hdr_append(hdr, "##contig=<ID=GL000213.1,length=164239,assembly=b37>");
	bcf_hdr_append(hdr, "##contig=<ID=GL000211.1,length=166566,assembly=b37>");
	bcf_hdr_append(hdr, "##contig=<ID=GL000199.1,length=169874,assembly=b37>");
	bcf_hdr_append(hdr, "##contig=<ID=GL000217.1,length=172149,assembly=b37>");
	bcf_hdr_append(hdr, "##contig=<ID=GL000216.1,length=172294,assembly=b37>");
	bcf_hdr_append(hdr, "##contig=<ID=GL000215.1,length=172545,assembly=b37>");
	bcf_hdr_append(hdr, "##contig=<ID=GL000205.1,length=174588,assembly=b37>");
	bcf_hdr_append(hdr, "##contig=<ID=GL000219.1,length=179198,assembly=b37>");
	bcf_hdr_append(hdr, "##contig=<ID=GL000224.1,length=179693,assembly=b37>");
	bcf_hdr_append(hdr, "##contig=<ID=GL000223.1,length=180455,assembly=b37>");
	bcf_hdr_append(hdr, "##contig=<ID=GL000195.1,length=182896,assembly=b37>");
	bcf_hdr_append(hdr, "##contig=<ID=GL000212.1,length=186858,assembly=b37>");
	bcf_hdr_append(hdr, "##contig=<ID=GL000222.1,length=186861,assembly=b37>");
	bcf_hdr_append(hdr, "##contig=<ID=GL000200.1,length=187035,assembly=b37>");
	bcf_hdr_append(hdr, "##contig=<ID=GL000193.1,length=189789,assembly=b37>");
	bcf_hdr_append(hdr, "##contig=<ID=GL000194.1,length=191469,assembly=b37>");
	bcf_hdr_append(hdr, "##contig=<ID=GL000225.1,length=211173,assembly=b37>");
	bcf_hdr_append(hdr, "##contig=<ID=GL000192.1,length=547496,assembly=b37>");
}


/**
 *Set chromosome name
 */
void bcf_set_chrom(bcf_hdr_t *h, bcf1_t *v, char* chrom)
{
	vdict_t *d = (vdict_t*)h->dict[BCF_DT_CTG];
	khint_t k = kh_get(vdict, d, chrom);
	if (k == kh_end(d)) 
    {
        // Simple error recovery for chromosomes not defined in the header. It will not help when VCF header has
        // been already printed, but will enable tools like vcfcheck to proceed.
        fprintf(stderr, "[W::%s] contig '%s' is not defined in the header\n", __func__, chrom);
        kstring_t tmp = {0,0,0};
        int l;
        ksprintf(&tmp, "##contig=<ID=%s,length=2147483647>", chrom);
        bcf_hrec_t *hrec = bcf_hdr_parse_line(h,tmp.s,&l);
        free(tmp.s);
        if ( bcf_hdr_add_hrec((bcf_hdr_t*)h, hrec) ) bcf_hdr_sync((bcf_hdr_t*)h);
        k = kh_get(vdict, d, chrom);
	}
    v->rid = kh_val(d, k).id;		
};

/**
 *Gets the read sequence from a bam record
 */
void bam_get_seq_string(bam1_t *srec, kstring_t *seq)
{
    seq->l=0;
    uint8_t* s = bam_get_seq(srec);
    for (uint16_t i = 0; i < bam_get_l_qseq(srec); ++i)
    {
        kputc("=ACMGRSVTWYHKDBN"[bam_seqi(s, i)], seq);
    }
};

/**
 *Gets the base qualities from a bam record, when N is observed, a placeholder value of 0(!, 33 adjusted) is entered
 */
void bam_get_qual_string(bam1_t *srec, kstring_t *qual, char* seq)
{
    qual->l=0;
    uint32_t offset = 0;
    uint8_t* s = bam_get_qual(srec);
    for (int32_t i = 0; i < bam_get_l_qseq(srec); ++i) 
    {
        if (seq[i]=='N')
        {
            kputc('!', qual);
            ++offset;
        }
        else
        {
            kputc(s[i-offset] + 33, qual);
        }
    }
};

/**
 *Gets the cigar string from a bam record
 */
void bam_get_cigar_string(bam1_t *srec, kstring_t *str)
{
    str->l=0;
    int32_t n_cigar_op = bam_get_n_cigar_op(srec);
    if (n_cigar_op)
    {
		uint32_t *cigar = bam_get_cigar(srec);
		for (int32_t i = 0; i < n_cigar_op; ++i)
		{
			kputw(bam_cigar_oplen(cigar[i]), str);
			kputc(bam_cigar_opchr(cigar[i]), str);
		}
	}
}

/**
 *Gets the end position of the last mapped base in the sequence.
 */
int32_t bam_get_end_pos1(bam1_t *srec)
{ 
    int32_t epos1 = bam_get_pos1(srec) - 1; 
    int32_t n_cigar_op = bam_get_n_cigar_op(srec);
    
    if (n_cigar_op)
	{
		uint32_t *cigar = bam_get_cigar(srec);

		for (int32_t i = 0; i < n_cigar_op; ++i)
		{
		    char op = bam_cigar_opchr(cigar[i]);
		    uint32_t oplen = bam_cigar_oplen(cigar[i]);

		    if (op=='M' || op=='D')
	        {
	            epos1 += oplen;
	        }
		}
	}
	
	return epos1;
};

/**
 *Gets the base in the read that is mapped to a genomic position.
 */
void bam_get_base_and_qual(bam1_t *srec, uint32_t pos1, char& base, char& qual, int32_t& rpos0)
{
    int32_t n_cigar_op = bam_get_n_cigar_op(srec);
    base = 'N';
    qual = 0;
   	rpos0 = 0;  
    
	if (n_cigar_op)
	{
	    kstring_t str;
	    str.l = str.m = 0, str.s = 0;
   
        uint8_t* s = bam_get_seq(srec);
        uint32_t *cigar = bam_get_cigar(srec); 
		
        //int32_t rlen = bam_get_l_qseq(srec);
    	uint32_t cpos1 = bam_get_pos1(srec); 
    	    
		for (int32_t i=0; i<n_cigar_op; ++i)
		{
		    char op = bam_cigar_opchr(cigar[i]);
		    str.l = 0;
			uint32_t oplen = bam_cigar_oplen(cigar[i]);

		    if (op=='M')
	        {
                if (pos1>=cpos1 && pos1<=cpos1+oplen-1)
	            {
	                rpos0 +=  pos1-cpos1;
                    base = "=ACMGRSVTWYHKDBN"[bam_seqi(s, rpos0)];
                        	            
    	            break;
	            }
	            else
                {
                    rpos0 += oplen;
                }
	        }
	        else if (op=='D')
	        {
	            if (pos1>=cpos1 && pos1<=cpos1+oplen-1)
	            {
	                rpos0 = BAM_READ_INDEX_NA;
    	            break;
	            }

	            cpos1 += oplen;
	        }
		    else if (op=='S' || op=='I')
	        {
	            rpos0 += oplen;
	        } 
		}
    	
    	int32_t offset = 0;
    	uint8_t* q = bam_get_qual(srec);
        for (int32_t i = 0; i<=rpos0; ++i) 
        {
            if ( "=ACMGRSVTWYHKDBN"[bam_seqi(s, i)]=='N')
            {
                ++offset;
            }
        }
        
        qual = q[rpos0-offset]+33;
	}
};

/**
 *Gets the base in the read that is mapped to a genomic position.
 Extracts the read sequence and aualities too.
 */
void bam_get_base_and_qual_and_read_and_qual(bam1_t *srec, uint32_t pos, char& base, char& qual, int32_t& rpos, kstring_t* readseq, kstring_t* readqual)
{
    bam1_core_t *c = &srec->core;
	int32_t rlen = c->l_qseq;
	uint32_t cpos = c->pos; //reference coordinates of the first mapped base
	rpos = 0; //read coordinates

	kstring_t str;
	str.l = str.m = 0, str.s = 0;
	base = 'N';
    qual = 0;

	if (c->n_cigar)
	{
		uint32_t *cigar = bam_get_cigar(srec);
		for (uint32_t i = 0; i < c->n_cigar; ++i)
		{
		    char op = bam_cigar_opchr(cigar[i]);
		    str.l = 0;
		    kputw(bam_cigar_oplen(cigar[i]), &str);
			char* stop;
			uint32_t len = strtol(str.s, &stop, 10);
		    assert(stop);

		    if (op=='M')
	        {
	            if (pos>=cpos && pos<=cpos+len-1)
	            {
	                rpos += pos-cpos;
	                break;
	            }

	            cpos += len;
	            rpos += len;
	        }
	        else if (op=='D')
	        {
	            if (pos>=cpos && pos<=cpos+len-1)
	            {
	                rpos = -1;
    	            break;
	            }

	            cpos += len;
	        }
		    else if (op=='S' || op=='I')
	        {
	            rpos += len;
	        }
		}

		//std::cout << "bpos " << bpos << "\n";
	    if (rpos>=0 && rpos<=rlen)
	    {
	        //sequence
            bam_get_seq_string(srec, readseq);
            base = readseq->s[rpos];
            
            //qual
            bam_get_qual_string(srec, readqual, readseq->s);
            qual = readqual->s[rpos];
        }
        else
        {
            rpos = BAM_READ_INDEX_NA;
        }
	}
//    std::cout << "b: " << base << "\n";
//    std::cout << "q: " << s[bpos-1] << " " << q << "\n";
//    for (uint32_t i = 0; i < c->l_qseq; ++i) std::cerr << ((char)(s[i] + 33));
};


/**
 *Check if variant is passed
 */
bool bcf_is_passed(bcf_hdr_t *h, bcf1_t *v)
{    
//    std::cerr << v->d.n_flt << ":" << v->d.flt[0] << "\n";
    return (v->d.n_flt==1 && !strcmp(h->id[BCF_DT_ID][v->d.flt[0]].key,"PASS"));    
}

/**
 *Add single sample to BCF header
 */
void bcf_hdr_add_sample(bcf_hdr_t *h, char *s)
{
    vdict_t *d = (vdict_t*)h->dict[BCF_DT_SAMPLE];
    int ret;
    int k = kh_put(vdict, d, s, &ret);
    if (ret) // absent 
    {   
        kh_val(d, k) = bcf_idinfo_def;
        kh_val(d, k).id = kh_size(d) - 1;
        int n = kh_size(d);
        h->samples = (char**) realloc(h->samples,sizeof(char*)*n);
        h->samples[n-1] = s;
        bcf_hdr_sync(h);
    } 
    else 
    {
        fprintf(stderr, "[W::%s] Duplicated sample name '%s'. Skipped.\n", __func__, s);
    }
}


/**
 *Get number of samples in bcf header
 */
int32_t bcf_hdr_get_n_sample(bcf_hdr_t *h)
{
    vdict_t *d = (vdict_t*)h->dict[BCF_DT_SAMPLE];
    return kh_size(d);
}

/**
 *Gets sequence names and lengths
 */ 
void bcf_hdr_get_seqs_and_lens(const bcf_hdr_t *h, const char**& seqs, int32_t*& lens, int *n)
{
    vdict_t *d = (vdict_t*)h->dict[BCF_DT_CTG];
    int tid, m = kh_size(d);
    seqs = (const char**) calloc(m,sizeof(const char*));
    lens = (int32_t*) calloc(m,sizeof(int32_t));
    khint_t k;
    for (k=kh_begin(d); k<kh_end(d); k++)
    {
        if ( !kh_exist(d,k) ) continue;
        tid = kh_val(d,k).id;
        assert( tid<m );
        seqs[tid] = kh_key(d,k);
        
        lens[tid] = 0;
        bcf_hrec_t *hrec = kh_val(d, k).hrec[0];
        for (int i=0; i<hrec->nkeys; ++i)
        {
            if (!strcmp(hrec->keys[i],"length"))
            {
                lens[tid] = atoi(hrec->vals[i]);
            }
        }
        assert(lens[tid]);
    }
    // sanity check: there should be no gaps
    for (tid=0; tid<m; tid++)
        assert(seqs[tid]);
    *n = m;
}


/**
 * Returns a bcf_fmt_t pointer associated with tag
 */
bcf_fmt_t *bcf_get_fmt(const bcf_hdr_t *h, bcf1_t *v, const char *tag)
{
    bcf_unpack(v, BCF_UN_FMT);

    int i, id = bcf_id2int(h, BCF_DT_ID, tag);
    if (id<0) return NULL;

    for (i=0; i<(int)v->n_fmt; ++i)
        if ( v->d.fmt[i].id==id ) return &v->d.fmt[i];

    return 0;
}

/**
 * Returns float value of integer info tag
 */
bool bcf_get_info_int(const bcf_hdr_t *h, bcf1_t *v, const char *tag, int32_t& f)
{
    bcf_unpack(v, BCF_UN_INFO);
 
    vdict_t *d = (vdict_t*)h->dict[BCF_DT_ID];
    khint_t k = kh_get(vdict, d, tag);
    if ( k == kh_end(d) ) return false;
    int32_t key =  kh_val(d, k).id;

	for (uint32_t i = 0; i < v->n_info; ++i)
	{
		bcf_info_t *z = &v->d.info[i];

        if (z->key == key)
		{
		    if (z->type==BCF_BT_INT32 || z->type==BCF_BT_INT16 || z->type==BCF_BT_INT8) 
			{
			    f = z->v1.i;
			    return true;
			}
			else
		    {
		        return false;
		    }
	    }
	}
	
	return false;
};


/**
 * Returns float value of float info tag
 */
bool bcf_get_info_float(const bcf_hdr_t *h, bcf1_t *v, const char *tag, float& f)
{
    bcf_unpack(v, BCF_UN_INFO);
 
    vdict_t *d = (vdict_t*)h->dict[BCF_DT_ID];
    khint_t k = kh_get(vdict, d, tag);
    if ( k == kh_end(d) ) return false;
    int32_t key =  kh_val(d, k).id;

	for (uint32_t i = 0; i < v->n_info; ++i)
	{
		bcf_info_t *z = &v->d.info[i];

        if (z->key == key)
		{
		    if (z->type==BCF_BT_FLOAT) 
			{
			    f = z->v1.f;
			    return true;
			}
			else
		    {
		        return false;
		    }
	    }
	}
	
	return false;
};

/**
 * Returns c string of a single values info tag
 */
bcf_fmt_t* bcf_get_format(const bcf_hdr_t *h, bcf1_t *v, const char *tag)
{
    bcf_unpack(v, BCF_UN_FMT);
    kstring_t s;
    s.l = s.m = 0;
    s.s = 0;

	for (uint32_t i = 0; i < v->n_fmt; ++i)
	{
		bcf_fmt_t *z = &v->d.fmt[i];

        s.l=0;
		kputs(h->id[BCF_DT_ID][z->id].key, &s);
				
		if (!strcmp(tag,s.s))
		{
		    s.l=0;
            free(s.s);			
			return z;
	    }
	}
	
    if (s.s) free(s.s);
    return 0;
};

/**
 * Returns c string of a single values info tag
 */
char* bcf_get_info1(const bcf_hdr_t *h, bcf1_t *v, const char *tag)
{
    bcf_unpack(v, BCF_UN_INFO);
    kstring_t s;
    s.l = s.m = 0;
    s.s = 0;
    if (v->n_info)
    {
		for (uint32_t i = 0; i < v->n_info; ++i)
		{
			bcf_info_t *z = &v->d.info[i];

            s.l=0;
			kputs(h->id[BCF_DT_ID][z->key].key, &s);
			
			//std::cerr << "INSIDE KEY " << s.s << " " << tag << "\n";
			if (!strcmp(tag,s.s))
			{
			    s.l=0;
    			bcf_fmt_array(&s, z->len, z->type, z->vptr);
    			
    			return (char*) strdup(s.s);
		    }
		}
    }

    return (char*) s.s;
};

/**
 * Adds an INFO field.
 */
int32_t bcf_add_info(bcf_hdr_t *h, bcf1_t *v, int32_t type, char *key, uint8_t *value, int32_t len)
{
    if (type == BCF_BT_CHAR)
    {
        bcf_dec_t *d = &v->d;
    
        vdict_t *dict = (vdict_t*)h->dict[BCF_DT_ID];
        khint_t k = kh_get(vdict, dict, key);
		//info tag not in header, add a header record
        if (k == kh_end(dict) || kh_val(dict, k).info[BCF_HL_INFO] == 15)
        {
            fprintf(stderr, "[W::%s] INFO '%s' is not defined in the header, assuming Type=String\n", __func__, key);
            kstring_t tmp = {0,0,0};
            int l;
            ksprintf(&tmp, "##INFO=<ID=%s,Number=1,Type=String,Description=\"Dummy\">", key);
            bcf_hrec_t *hrec = bcf_hdr_parse_line(h,tmp.s,&l);
            free(tmp.s);
            if ( bcf_hdr_add_hrec((bcf_hdr_t*)h, hrec) ) bcf_hdr_sync((bcf_hdr_t*)h);
        }
        else
        {
            //check for duplicate info fields, 
            int32_t key = kh_val(dict, k).id;
            bcf_info_t *z = NULL;
            uint32_t i = 0;
            for (i=0; i<v->n_info; ++i)
            {
                z = &d->info[i];
                if (z->key == key)
                {
                  break;  
                }    
            }
            
            if (i==v->n_info)
            {    
                ++v->n_info;
      		    hts_expand(bcf_info_t, v->n_info, d->m_info, d->info);
        	}
        	z = &d->info[i];
            z->type = type;
            z->key =  kh_val(dict, k).id;
            z->len = len;
        	z->vptr = value;
        	
        }    
    }
    
    return 1;
}

/**
 *Format variant, returns in chr:pos:ref:alts(,) form
 */
void bcf_format_variant(bcf_hdr_t *h, bcf1_t *v, kstring_t *s) 
{
    s->l = 0;
    kputs(bcf_get_chrom(h, v), s);
    kputs(":", s);
    kputuw(bcf_get_pos1(v), s);
    kputs(":", s);
    kputs(bcf_get_alt(v, 0), s);
    kputs(":", s);
    
    for (uint32_t i=1; i<v->n_allele; ++i)
    {
        if (i>1) kputc(',', s);
        kputs(bcf_get_alt(v, i), s);
    }
}


/**
 *Set allele.
 */
void bcf_set_allele(bcf1_t *v, std::vector<std::string> alleles)
{
    bcf_dec_t *d = &v->d;
    
    kstring_t als;
    als.s = d->als; 
    als.l = 0; 
    als.m = d->m_als;
    
    hts_expand(char*, v->n_allele, d->m_allele, d->allele);
    
    int32_t offset = 0;
    for (uint32_t i=0; i<alleles.size(); ++i)
    {   
        kputsn_(alleles[i].c_str(), alleles[i].size(), &als);
        kputc('\0', &als);
        d->allele[i] = als.s + offset;
        offset += alleles[i].size()+1;        
    }
    
    d->als = als.s; 
    d->m_als = als.m;
}

/**
 gets file type, include SAM and BAM detection from file_type in htslib
*/
int zfile_type(const char *fname)
{
    int len = strlen(fname);
    if ( !strcasecmp(".vcf.gz",fname+len-7) ) return IS_VCF_GZ;
    if ( !strcasecmp(".vcf",fname+len-4) ) return IS_VCF;
    if ( !strcasecmp(".bcf",fname+len-4) ) return IS_BCF;
    if ( !strcasecmp(".bam",fname+len-4) ) return IS_BAM;
    if ( !strcasecmp(".sam",fname+len-4) ) return IS_SAM;
    if ( !strcmp("-",fname) ) return IS_STDIN;

    return 0;
};

/**
 modifies mode to have an addition b if necessary
*/
const char* modify_mode(const char* fname, char mode)
{
    return zmodify_mode(fname, mode, false);
};

/**
 * modifies mode to have an addition b if necessary
 * mode must be 'r' or 'w'
 */
const char* zmodify_mode(const char* fname, char mode, bool output_bcf)
{
    assert(mode=='r' || mode=='w');
    
    int filetype = zfile_type(fname);
    if (filetype==IS_BCF || filetype==IS_BAM)
	{
	    return mode=='w' ? "wb" : "rb";
	}
	else if (filetype==IS_VCF || filetype==IS_VCF_GZ)
	{
        return mode=='w' ? "w" : "r";
	}
	else if (filetype==IS_STDIN)
	{
        return mode=='w' ? (output_bcf? "wb" : "w") : "r";
	}
	else
    {
        return mode=='w' ? "w" : "r";
    }
};

/**
 * Synchronize dictionaries
 * When updating adding new tags, you just need to update h->dict
 * This method syncrhonizes the tags in h->dict to h->id
 */
int bcf_hdr_sync(bcf_hdr_t *h)
{
	int i;
	for (i = 0; i < 3; ++i) {
		khint_t k;
		vdict_t *d = (vdict_t*)h->dict[i];

        //allocate memory
        if ( h->n[i] < kh_size(d) )
            h->id[i] = (bcf_idpair_t*)realloc(h->id[i], kh_size(d) * sizeof(bcf_idpair_t));

		//update size
		h->n[i] = kh_size(d);
		for (k = kh_begin(d); k != kh_end(d); ++k) {
			if (!kh_exist(d, k)) continue;
			h->id[i][kh_val(d, k).id].key = kh_key(d, k);
			h->id[i][kh_val(d, k).id].val = &kh_val(d, k);
		}
	}
	return 0;
}

int vcf_format1_shared_to_missing(const bcf_hdr_t *h, const bcf1_t *v, kstring_t *s)
{
	//uint8_t *ptr = (uint8_t*)v->shared.s;
	int i;
	//s->l = 0;
	bcf_unpack((bcf1_t*)v, BCF_UN_ALL);
	kputs(h->id[BCF_DT_CTG][v->rid].key, s); // CHROM
	kputc('\t', s); kputw(v->pos + 1, s); // POS
	kputc('\t', s); kputs(v->d.id, s); // ID
	kputc('\t', s); // REF
	if (v->n_allele > 0) kputs(v->d.allele[0], s);
	else kputc('.', s);
	kputc('\t', s); // ALT
	if (v->n_allele > 1) {
		for (i = 1; i < v->n_allele; ++i) {
			if (i > 1) kputc(',', s);
			kputs(v->d.allele[i], s);
		}
	} else kputc('.', s);
	kputc('\t', s); // QUAL
	//if (memcmp(&v->qual, &bcf_missing_float, 4) == 0) kputc('.', s); // QUAL
	//else ksprintf(s, "%g", v->qual);
	kputc('.', s); // QUAL
	kputc('\t', s); // FILTER
//	if (v->d.n_flt) {
//		for (i = 0; i < v->d.n_flt; ++i) {
//			if (i) kputc(';', s);
//			kputs(h->id[BCF_DT_ID][v->d.flt[i]].key, s);
//		}
//	} else kputc('.', s);
	kputc('.', s); // FILTER
	kputc('\t', s); // INFO
//	if (v->n_info) {
//		for (i = 0; i < v->n_info; ++i) {
//			bcf_info_t *z = &v->d.info[i];
//			if (i) kputc(';', s);
//			kputs(h->id[BCF_DT_ID][z->key].key, s);
//			if (z->len <= 0) continue;
//			kputc('=', s);
//			if (z->len == 1) {
//				if (z->type == BCF_BT_FLOAT) ksprintf(s, "%g", z->v1.f);
//				else if (z->type != BCF_BT_CHAR) kputw(z->v1.i, s);
//				else kputc(z->v1.i, s);
//			} else bcf_fmt_array(s, z->len, z->type, z->vptr);
//		}
//	} else kputc('.', s);
	// FORMAT and individual information
    kputc('.', s); // INFO
	//ptr = (uint8_t*)v->indiv.s;
	if (v->n_sample && v->n_fmt) { // FORMAT
		int i, j, l, gt_i = -1;
		bcf_fmt_t *fmt = v->d.fmt;
        // This way it is not possible to modify the output line. Also,
        // the bcf_unpack_fmt_core has been already called above.
		//      fmt = (bcf_fmt_t*)alloca(v->n_fmt * sizeof(bcf_fmt_t));
		//      ptr = bcf_unpack_fmt_core(ptr, v->n_sample, v->n_fmt, fmt);
		for (i = 0; i < (int)v->n_fmt; ++i) {
			kputc(i? ':' : '\t', s);
			kputs(h->id[BCF_DT_ID][fmt[i].id].key, s);
			if (strcmp(h->id[BCF_DT_ID][fmt[i].id].key, "GT") == 0) gt_i = i;
		}
		for (j = 0; j < v->n_sample; ++j) {
			kputc('\t', s);
			for (i = 0; i < (int)v->n_fmt; ++i) {
				bcf_fmt_t *f = &fmt[i];
				if (i) kputc(':', s);
                if (gt_i == i) {
                    // See also bcf_format_gt in vcfquery.c, defined as static atm
                    int8_t *x = (int8_t*)(f->p + j * f->size); // FIXME: does not work with n_alt >= 64
                    for (l = 0; l < f->n && x[l] != INT8_MIN; ++l) {
                        if (l) kputc("/|"[x[l]&1], s);
                        if (x[l]>>1) kputw((x[l]>>1) - 1, s);
                        else kputc('.', s);
                    }
                    if (l == 0) kputc('.', s);
                } else bcf_fmt_array(s, f->n, f->type, f->p + j * f->size);
			}
		}
	}
	return 0;
}

int vcf_format1_format_genotypes(const bcf_hdr_t *h, const bcf1_t *v, kstring_t *s)
{
	//uint8_t *ptr = (uint8_t*)v->shared.s;
	//int i;
	//std::cerr << "ktr len: " << s->l << "\n";

//	s->l = 0;
	bcf_unpack((bcf1_t*)v, BCF_UN_ALL);
//	kputs(h->id[BCF_DT_CTG][v->rid].key, s); // CHROM
//	kputc('\t', s); kputw(v->pos + 1, s); // POS
//	kputc('\t', s); kputs(v->d.id, s); // ID
//	kputc('\t', s); // REF
//	if (v->n_allele > 0) kputs(v->d.allele[0], s);
//	else kputc('.', s);
//	kputc('\t', s); // ALT
//	if (v->n_allele > 1) {
//		for (i = 1; i < v->n_allele; ++i) {
//			if (i > 1) kputc(',', s);
//			kputs(v->d.allele[i], s);
//		}
//	} else kputc('.', s);
//	kputc('\t', s); // QUAL
//	if (memcmp(&v->qual, &bcf_missing_float, 4) == 0) kputc('.', s); // QUAL
//	else ksprintf(s, "%g", v->qual);
//	kputc('\t', s); // FILTER
//	if (v->d.n_flt) {
//		for (i = 0; i < v->d.n_flt; ++i) {
//			if (i) kputc(';', s);
//			kputs(h->id[BCF_DT_ID][v->d.flt[i]].key, s);
//		}
//	} else kputc('.', s);
//	kputc('\t', s); // INFO
//	if (v->n_info) {
//		for (i = 0; i < v->n_info; ++i) {
//			bcf_info_t *z = &v->d.info[i];
//			if (i) kputc(';', s);
//			kputs(h->id[BCF_DT_ID][z->key].key, s);
//			if (z->len <= 0) continue;
//			kputc('=', s);
//			if (z->len == 1) {
//				if (z->type == BCF_BT_FLOAT) ksprintf(s, "%g", z->v1.f);
//				else if (z->type != BCF_BT_CHAR) kputw(z->v1.i, s);
//				else kputc(z->v1.i, s);
//			} else bcf_fmt_array(s, z->len, z->type, z->vptr);
//		}
//	} else kputc('.', s);
	//FORMAT and individual information
	//ptr = (uint8_t*)v->indiv.s;
	if (v->n_sample && v->n_fmt) { // FORMAT
		int i, j, l, gt_i = -1;
		bcf_fmt_t *fmt = v->d.fmt;
        // This way it is not possible to modify the output line. Also,
        // the bcf_unpack_fmt_core has been already called above.
		//      fmt = (bcf_fmt_t*)alloca(v->n_fmt * sizeof(bcf_fmt_t));
		//      ptr = bcf_unpack_fmt_core(ptr, v->n_sample, v->n_fmt, fmt);
		for (i = 0; i < (int)v->n_fmt; ++i) {
			kputc(i? ':' : '\t', s);
			kputs(h->id[BCF_DT_ID][fmt[i].id].key, s);
			if (strcmp(h->id[BCF_DT_ID][fmt[i].id].key, "GT") == 0) gt_i = i;
		}
		for (j = 0; j < v->n_sample; ++j) {
			kputc('\t', s);
			for (i = 0; i < (int)v->n_fmt; ++i) {
				bcf_fmt_t *f = &fmt[i];
				if (i) kputc(':', s);
                if (gt_i == i) {
                    // See also bcf_format_gt in vcfquery.c, defined as static atm
                    int8_t *x = (int8_t*)(f->p + j * f->size); // FIXME: does not work with n_alt >= 64
                    for (l = 0; l < f->n && x[l] != INT8_MIN; ++l) {
                        if (l) kputc("/|"[x[l]&1], s);
                        if (x[l]>>1) kputw((x[l]>>1) - 1, s);
                        else kputc('.', s);
                    }
                    if (l == 0) kputc('.', s);
                } else bcf_fmt_array(s, f->n, f->type, f->p + j * f->size);
			}
		}
	}
	return 0;
}


int vcf_format1_genotypes(const bcf_hdr_t *h, const bcf1_t *v, kstring_t *s)
{
//	uint8_t *ptr = (uint8_t*)v->shared.s;
//	int i;
	//std::cerr << "ktr len: " << s->l << "\n";

//	s->l = 0;
	bcf_unpack((bcf1_t*)v, BCF_UN_ALL);
//	kputs(h->id[BCF_DT_CTG][v->rid].key, s); // CHROM
//	kputc('\t', s); kputw(v->pos + 1, s); // POS
//	kputc('\t', s); kputs(v->d.id, s); // ID
//	kputc('\t', s); // REF
//	if (v->n_allele > 0) kputs(v->d.allele[0], s);
//	else kputc('.', s);
//	kputc('\t', s); // ALT
//	if (v->n_allele > 1) {
//		for (i = 1; i < v->n_allele; ++i) {
//			if (i > 1) kputc(',', s);
//			kputs(v->d.allele[i], s);
//		}
//	} else kputc('.', s);
//	kputc('\t', s); // QUAL
//	if (memcmp(&v->qual, &bcf_missing_float, 4) == 0) kputc('.', s); // QUAL
//	else ksprintf(s, "%g", v->qual);
//	kputc('\t', s); // FILTER
//	if (v->d.n_flt) {
//		for (i = 0; i < v->d.n_flt; ++i) {
//			if (i) kputc(';', s);
//			kputs(h->id[BCF_DT_ID][v->d.flt[i]].key, s);
//		}
//	} else kputc('.', s);
//	kputc('\t', s); // INFO
//	if (v->n_info) {
//		for (i = 0; i < v->n_info; ++i) {
//			bcf_info_t *z = &v->d.info[i];
//			if (i) kputc(';', s);
//			kputs(h->id[BCF_DT_ID][z->key].key, s);
//			if (z->len <= 0) continue;
//			kputc('=', s);
//			if (z->len == 1) {
//				if (z->type == BCF_BT_FLOAT) ksprintf(s, "%g", z->v1.f);
//				else if (z->type != BCF_BT_CHAR) kputw(z->v1.i, s);
//				else kputc(z->v1.i, s);
//			} else bcf_fmt_array(s, z->len, z->type, z->vptr);
//		}
//	} else kputc('.', s);
	// FORMAT and individual information
//	ptr = (uint8_t*)v->indiv.s;
	if (v->n_sample && v->n_fmt) { // FORMAT
		int i, j, l, gt_i = -1;
		bcf_fmt_t *fmt = v->d.fmt;
        // This way it is not possible to modify the output line. Also,
        // the bcf_unpack_fmt_core has been already called above.
		//      fmt = (bcf_fmt_t*)alloca(v->n_fmt * sizeof(bcf_fmt_t));
		//      ptr = bcf_unpack_fmt_core(ptr, v->n_sample, v->n_fmt, fmt);
		for (i = 0; i < (int)v->n_fmt; ++i) {
//			kputc(i? ':' : '\t', s);
//			kputs(h->id[BCF_DT_ID][fmt[i].id].key, s);
			if (strcmp(h->id[BCF_DT_ID][fmt[i].id].key, "GT") == 0) gt_i = i;
		}
		for (j = 0; j < v->n_sample; ++j) {
			kputc('\t', s);
			for (i = 0; i < (int)v->n_fmt; ++i) {
				bcf_fmt_t *f = &fmt[i];
				if (i) kputc(':', s);
                if (gt_i == i) {
                    // See also bcf_format_gt in vcfquery.c, defined as static atm
                    int8_t *x = (int8_t*)(f->p + j * f->size); // FIXME: does not work with n_alt >= 64
                    for (l = 0; l < f->n && x[l] != INT8_MIN; ++l) {
                        if (l) kputc("/|"[x[l]&1], s);
                        if (x[l]>>1) kputw((x[l]>>1) - 1, s);
                        else kputc('.', s);
                    }
                    if (l == 0) kputc('.', s);
                } else bcf_fmt_array(s, f->n, f->type, f->p + j * f->size);
			}
		}
	}
	return 0;
}