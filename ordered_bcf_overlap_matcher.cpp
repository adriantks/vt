/* The MIT License

   Copyright (c) 2014 Adrian Tan <atks@umich.edu>

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

#include "ordered_bcf_overlap_matcher.h"

/**
 * Constructor.
 */
OrderedBCFOverlapMatcher::OrderedBCFOverlapMatcher(std::string& file)
{
    odr = new BCFOrderedReader(file);
    s = {0,0,0};
    no_regions = 0;
    current_interval.seq = "";
};

/**
 * Destructor.
 */
OrderedBCFOverlapMatcher::~OrderedBCFOverlapMatcher() {};

/**
 * Returns true if chrom:start1-end1 overlaps with a region in the file.
 */
bool OrderedBCFOverlapMatcher::overlaps_with(std::string& chrom, int32_t start1, int32_t end1)
{
    bool overlaps = false;

    //moves to new chromosome
    if (current_interval.seq!=chrom)
    {
        buffer.clear();
        current_interval.set(chrom);
        odr->jump_to_interval(current_interval);
        std::cerr << "Jumped to chromosome " << chrom << "\n";
        while (todr->read(&s))
        {
            BEDRecord br(&s);
            if (br.end1<start1) continue;
            overlaps = overlaps || (br.start1<=end1);
            buffer.push_back(br);
            if (br.start1>end1) break;
        }
    }
    else
    {
        //scythe preceding bed records
        std::list<BEDRecord>::iterator i = buffer.begin();
        while (i!=buffer.end())
        {
            if ((*i).end1<start1)
            {
                i = buffer.erase(i);
                continue;
            }

            overlaps = ((*i).start1<=end1);
            break;
        }
        
        if (!overlaps)        
        {
            if (buffer.empty())
            {    
                while (todr->read(&s))
                {
                    BEDRecord br(&s);
                    if (br.end1<start1) continue;
                    overlaps = overlaps || (br.start1<=end1);
                    buffer.push_back(br);
                    if (br.start1>end1) break;
                }
            }
        }
    }

    return overlaps;
};
