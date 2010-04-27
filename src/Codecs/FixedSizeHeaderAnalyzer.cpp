// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
#include <Common/QuickFASTPch.h>
#include "FixedSizeHeaderAnalyzer.h"
#include <Common/Types.h>
#include <Codecs/DataSource.h>
#include <Common/ByteSwapper.h>

using namespace ::QuickFAST;
using namespace ::QuickFAST::Codecs;

FixedSizeHeaderAnalyzer::FixedSizeHeaderAnalyzer(
  size_t sizeBytes,
  bool bigEndian,
  size_t prefixBytes,
  size_t suffixBytes
  )
: prefixBytes_(prefixBytes)
, sizeBytes_(sizeBytes)
, suffixBytes_(suffixBytes)
, swapNeeded_(ByteSwapper::isBigEndian() ? !bigEndian : bigEndian)
, state_(ParsingIdle)
, blockSize_(0)
, byteCount_(0)
, testSkip_(0)
, headersParsed_(0)
{
}

FixedSizeHeaderAnalyzer::~FixedSizeHeaderAnalyzer()
{
}

bool
FixedSizeHeaderAnalyzer::analyzeHeader(DataSource & source, size_t & blockSize, bool & skip)
{
  while(state_ != ParsingComplete)
  {
    switch(state_)
    {
    case ParsingIdle:
      {
        state_ = ParsingPrefix;
        byteCount_ = 0;
//        break;
      }
    case ParsingPrefix:
      {
        while(byteCount_ < prefixBytes_)
        {
          uchar next;
          if(!source.getByte(next))
          {
            return false;
          }
          ++byteCount_;
        }
        state_ = ParsingBlockSize;
        byteCount_ = 0;
        blockSize_ = 0;
//        break;
      }
    case ParsingBlockSize:
      {
        while(byteCount_ < sizeBytes_)
        {
          uchar next;
          if(!source.getByte(next))
          {
            return false;
          }
          if(swapNeeded_)
          {
            blockSize |= (next & 0xFF) << (byteCount_ * 8);
          }
          else
          {
            blockSize_ <<= 8;
            blockSize_ |= (next & 0xFF);
          }
          ++byteCount_;
        }
        state_ = ParsingSuffix;
        byteCount_ = 0;

//        break;
      }
    case ParsingSuffix:
      {
        while(byteCount_ < suffixBytes_)
        {
          uchar next;
          if(!source.getByte(next))
          {
            return false;
          }
          ++byteCount_;
        }
        state_ = ParsingComplete;
        break;
      }
    default:
      {
        break;
      }
    }
  }
  blockSize = blockSize_;
  skip = false;
  state_ = ParsingIdle;
  if(testSkip_ != 0 && (++headersParsed_ % testSkip_ == 0))
  {
    std::cout << "SKIPPING HEADER " << headersParsed_ << std::endl;
    skip = true;
  }
  return true;
}
