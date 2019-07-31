/*
 * Author: Jon Trulson <jtrulson@ics.com>
 * Copyright (c) 2014 Intel Corporation.
 *
 * This program and the accompanying materials are made available under the
 * terms of the The MIT License which is available at
 * https://opensource.org/licenses/MIT.
 *
 * SPDX-License-Identifier: MIT
 */

#include <iostream>
#include <string>
#include <stdexcept>

#include "ta12200.hpp"

using namespace upm;
using namespace std;

TA12200::TA12200(int pin)
{
  initClock();

  if ( !(m_aio = mraa_aio_init(pin)) )
    {
      throw std::invalid_argument(std::string(__FUNCTION__) +
                                  ": mraa_aio_init() failed, invalid pin?");
      return;
    }
}

TA12200::~TA12200()
{
  mraa_aio_close(m_aio);
}

void TA12200::initClock()
{
  gettimeofday(&m_startTime, NULL);
}

uint32_t TA12200::getMillis()
{
  struct timeval elapsed, now;
  uint32_t elapse;

  // get current time
  gettimeofday(&now, NULL);

  // compute the delta since m_startTime
  if( (elapsed.tv_usec = now.tv_usec - m_startTime.tv_usec) < 0 ) 
    {
      elapsed.tv_usec += 1000000;
      elapsed.tv_sec = now.tv_sec - m_startTime.tv_sec - 1;
    } 
  else 
    {
      elapsed.tv_sec = now.tv_sec - m_startTime.tv_sec;
    }

  elapse = (uint32_t)((elapsed.tv_sec * 1000) + (elapsed.tv_usec / 1000));

  // never return 0
  if (elapse == 0)
    elapse = 1;

  return elapse;
}


int TA12200::highestValue()
{
  int hiVal = 0;
  int val;
  uint32_t start = getMillis();

  // 1 second
  while (getMillis() < (start + 1000))
    {
      val = mraa_aio_read(m_aio);
      if (val == -1) {
        return -1;
      }
      if (val > hiVal) {
        hiVal = val;
      }
    }
        
  return hiVal;
}

float TA12200::milliAmps(unsigned int val, int res)
{
  float ampCurrent;
  float effectiveVal;

  // From grove wiki page:
  // minimum_current=1/1024*5/800*2000000/1.414=8.6(mA) 
  // Only for sinusoidal alternating current
  //ampCurrent = float(val) / float(res) * 5.0 / 800.0 * 2000000.0;
  ampCurrent = float(val) / float(res) * 12500.0;
  effectiveVal = ampCurrent/1.414;
  return (effectiveVal);
}

