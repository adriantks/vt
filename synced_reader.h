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

#ifndef SYNCED_READER_H
#define SYNCED_READER_H

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <math.h>
#include <float.h>
#include <vector>
#include <map>
#include <queue>
#include <list>
#include "boost/algorithm/string/split.hpp"
#include "boost/algorithm/string/join.hpp"
#include "boost/algorithm/string/classification.hpp"
#include "boost/tokenizer.hpp"
#include "htslib/vcf.h"
#include "htslib/vcfutils.h"
#include "htslib/tbx.h"
#include "tclap/CmdLine.h"
#include "tclap/Arg.h"
#include "hts_utils.h"

/**
 * Wrapper class for the bcf object.
 */
class bcfptr
{
    public:
    int32_t file_index;
    int32_t pos1;
    bcf1_t *v;
    bcfptr():file_index(-1), pos1(-1), v(0) {}
};

/**
 * Comparator for BCFPtr class.  Used in priority_queue; ensures that 
 * records are ordered according to file order.
 */
class CompareBCFPtr
{
    public:
    bool operator()(bcfptr& a, bcfptr& b)
    {
        return a.pos1 == b.pos1 ? a.file_index >= b.file_index  : a.pos1 >= b.pos1;
    }
};

/**
 * A class for reading files in a synced fashion.
 * All variants at the same position are read and placed in a processing vector.
 *
 * This is support for the following cases:
 *   
 * 1) Contigs headers present and VCFs are indexed
 *    All contigs are merged and VCFs are accessd by contigs, thus only order is 
 *    expected within a contig. 
 * 
 * 2) First VCF is not indexed but ordered and the other VCFs are indexed
 *
 * We assume that all the compared VCFs are based on the same reference sequence.
 * If the contigs are not present, contigs can be assumed by build of assembly or 
 * the user may input the contig names.
 */
class SyncedReader
{
    public:
        
    ///////
    //i/o//
    /////// 
    std::vector<std::string> vcf_files; //file names
    std::vector<vcfFile *> vcfs; //file objects
    std::vector<BGZF *> vcfgzs; //for vcf gzs
    std::vector<bcf_hdr_t *> hdrs; // headers 
    std::vector<hts_idx_t *> idxs; // indices
    std::vector<tbx_t *> tbxs; // for tabix
    std::vector<hts_itr_t *> itrs; //iterators
    int32_t nfiles; //number of files
    int32_t neofs; //number of files read till eof
    
    //list of contigs
    //inferred from headers or from user defined list
    std::vector<std::string> intervals;
    uint32_t interval_index;    
    std::string current_interval;
    int32_t current_pos1;
        
    kstring_t s;
    
    //buffer for records
    std::vector<std::list<bcf1_t *> > buffer;
    std::list<bcf1_t *> pool;
    std::priority_queue<bcfptr, std::vector<bcfptr>, CompareBCFPtr > pq;
        
    //contains the most recent position to process
    std::priority_queue<int32_t> pqueue;
    
    //useful stuff
    std::stringstream ss;
    
    /**
     * Initialize files and intervals.
     */
    SyncedReader(std::vector<std::string> _vcf_files, std::vector<std::string> _intervals);
      
    /**
     * Returns list of files that have variants at a certain position.
     * 
     */
    bool read_next_position(std::vector<bcfptr>& current_recs);
    
    /**
     * Gets sequence name of a record
     */
    const char* get_seqname(int32_t i, bcf1_t *v);

    /**
     * Gets current 1-based position being accessed.
     */
    std::string get_current_sequence();
        
    /**
     * Gets current sequence being accessed.
     */
    int32_t get_current_pos1();
    
    private:
    /**
     * Prints buffer.
     */
    void print_buffer();
        
    /**
     * Inserts a record into pq. 
     */
    void insert_into_pq(int32_t i, bcf1_t *v);
        
    /**
     * Gets records for the most recent position and fills up the buffer for file i. 
     */
    bool get_recs(int32_t i);

    /**
     * Returns true if there are more intervals to process.
     */
    bool more_intervals();

    /**
     * Initialize buffer for next region. Returns true if successful.
     */
    bool initialize_next_region();

    /**
     * Gets record from pool, creates a new record if necessary
     */
    bcf1_t* get_bcf1_from_pool();
   
    /**
     * Returns record to pool
     */
    void store_bcf1_into_pool(bcf1_t* v);
    
    /**
     * Gets records for the most recent position and fills up the buffer from file i.
     * returns true if buffer is filled or it is not necessary to fill buffer.
     * returns false if no more records are found to fill buffer
     */
    void fill_buffer(int32_t i);
                
    /**
     * Updates pq, buffer simultaneously.
     */
    void pop_and_push_rec(bcfptr b);
};
    
#endif