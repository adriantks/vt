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

#ifndef BCF_SYNCED_READER_H
#define BCF_SYNCED_READER_H

#include <cstdlib>
#include <cstdint>
#include <cstring>
#include <cmath>
#include <cfloat>
#include <vector>
#include <map>
#include <queue>
#include <list>
#include "htslib/hts.h"
#include "htslib/vcf.h"
#include "htslib/kstring.h"
#include "htslib/tbx.h"
#include "genome_interval.h"
#include "hts_utils.h"

/**
 * Wrapper class for the bcf object.
 */
class bcfptr
{
    public:
    int32_t file_index;
    int32_t pos1;
    bcf_hdr_t *h;
    bcf1_t *v;
    bcfptr():file_index(-1), pos1(-1), v(0) {}
    bcfptr(int32_t file_index, int32_t pos1, bcf1_t *v):file_index(file_index), pos1(pos1), v(v){}
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
 * This is supported for the following cases:
 *   
 * 1) All files are indexed. 
 *
 *    Iterating through all the regions found in all the files.
 *    Sequences are easily obtained via headers and/or tabix objects. 
 *    If intervals are specified, just populate the intervals to iterate with specified intervals.
 *
 * 2) Only the first file is not indexed but ordered.
 * 
 *    a) no intervals specified
 *           first file is streamed and the rest of the files are randomly accessed.
 *
 *    b) intervals specified
 *           first file is streamed and selected via interval tree.
 *           once a record within a region is read, the other files are selected on that interval. 
 *
 * If no intervals are selected by the caller, a union of all sequences are detected
 * from the files.
 */
class BCFSyncedReader
{
    public:
        
    ///////
    //i/o//
    /////// 
    std::vector<std::string> vcf_files; //file names
    std::vector<vcfFile *> vcfs; //file objects
    std::vector<bcf_hdr_t *> hdrs; // headers 
    std::vector<hts_idx_t *> idxs; // indices
    std::vector<tbx_t *> tbxs; // for tabix
    std::vector<hts_itr_t *> itrs; //iterators
    std::vector<int32_t> ftypes; //file types
    int32_t nfiles; //number of files
    int32_t neofs; //number of files read till eof
        
    bool indexed_first_file;
    
    //list of contigs
    std::vector<GenomeInterval> intervals;
    std::map<std::string, int32_t> intervals_map;
    uint32_t intervals_index;
    bool exists_selected_intervals;
    
    //variables for keeping track of status
    std::string current_interval;
    int32_t current_rid;
    int32_t current_pos1;
    
    //variables for managing non indexed first file
    //
    bcf1_t *next_interval_v;
    std::string next_interval;
    int32_t no_more_first_file_records; //non zero if true    
        
    kstring_t s;
    
    //buffer for records in use, this is indexed by the file index 
    std::vector<std::list<bcf1_t *> > buffer;
    //empty records that can be reused
    std::list<bcf1_t *> pool;
    //contains the most recent position to process
    std::priority_queue<bcfptr, std::vector<bcfptr>, CompareBCFPtr > pq;
        
    //useful stuff
    
    /**
     * Initialize files and intervals.
     */
    BCFSyncedReader(std::vector<std::string>& _vcf_files, std::vector<GenomeInterval>& _intervals);
    
    /**
     * Returns list of files that have variants at a certain position.
     * 
     */
    bool read_next_position(std::vector<bcfptr>& current_recs);
    
    /**
     * Populate sequence names from files.
     */
    void add_interval(int32_t i);
    
    /**
     * Populate sequence names from files.
     */
    void remove_interval(std::string& interval);
        
    /**
     * Load index for the ith file, returns true if successful
     */
    bool load_index(int32_t i);
    
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
    bool initialize_next_interval();

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
};
    
#endif