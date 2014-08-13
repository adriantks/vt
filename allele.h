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

#include <cstdint>
#include <iostream>
#include "htslib/kstring.h"
#include <vector>

#ifndef ALLELE_H
#define ALLELE_H

#define VT_REF      0   //dlen==0 && diff==0
#define VT_SNP      1   //min(rlen,alen)==1 && diff==1
#define VT_MNP      2   //min(rlen,alen)==diff
#define VT_INDEL    4   //diff!=0 && (rlen==1 || alen==1)
#define VT_CLUMPED  8   //all others
#define VT_SV       16  //structural variant tags

/**
 * Allele.
 *
 * Allele can be described to be a SNP or INDEL or etc. with respect to the reference
 * type - give the variant type
 * len  - difference in length between the ref and alt with positive inferring an insertion and negative inferring a deletion
 * tlen - assuming variant is VNTR, tlen is the tract length of the repeated motif of the VNTR
 */
class Allele
{
    public:

    int32_t type;
    int32_t diff;  //number of difference bases when bases are compared
    int32_t alen;  //length(alt)
    int32_t dlen;  //length(alt)-length(ref)
    int32_t tlen;  //tract length with respect to reference
    int32_t mlen;  //min shared length
    int32_t ts;    //no. of transitions
    int32_t tv;    //no. of tranversions (mlen-ts)
    int32_t ins;   //no. of insertions 
    int32_t del;   //no. of deletions 
    
    Allele(int32_t type, int32_t diff, int32_t alen, int32_t dlen, int32_t tlen, int32_t mlen, int32_t ts);

    Allele();
    
    ~Allele();

    void clear();

    void print();
};

#endif
