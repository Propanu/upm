/*
 * Author: Jon Trulson <jtrulson@ics.com>
 * Copyright (c) 2015 Intel Corporation.
 *
 * Thanks to public domain code by Martin Liddament for some useful clues!
 * http://www.veetech.org.uk/CO2_Monitor_Sketch_2_Operation.txt
 * and sandbox electronics (http://sandboxelectronics.com/?p=147).
 *
 * This program and the accompanying materials are made available under the
 * terms of the The MIT License which is available at
 * https://opensource.org/licenses/MIT.
 *
 * SPDX-License-Identifier: MIT
 */

#include <iostream>
#include <math.h>
#include "mg811.hpp"

using namespace std;
using namespace upm;

// voltage gain of the DC amplifier
static const float dcGain = 8.5;

MG811::MG811(int pin, int dpin, float aref) :
  m_aio(pin), m_gpio(dpin)
{
  m_aRes = m_aio.getBit();
  m_aref = aref;

  m_gpio.dir(mraa::DIR_IN);

  // these are just placeholder values (coarsely measured during
  // development), you should determine the appropriate values (in
  // volts) for your environment at the specified concentrations.  Use
  // the getReferenceVoltage() method to get these values at 400ppm
  // and 1000ppm respectively.  Good luck.
  setCalibration(0.5514, 0.370);
}

MG811::~MG811()
{
}

float MG811::volts()
{
  int val = m_aio.read();

  return(float(val) * (m_aref / float(1 << m_aRes)));
}

void MG811::setCalibration(float ppm400, float ppm1000)
{
  m_zeroPointValue = ppm400;
  m_reactionValue = ppm1000;
}

float MG811::getReferenceVoltage()
{
  return (volts() / dcGain);
}

float MG811::ppm()
{
  static const float log400 = log10f(400);
  static const float log1000 = log10f(1000);

  float val = volts();

  if ((val / dcGain) >= m_zeroPointValue)
    return 0.0;
  else
    return powf(10.0, ((val/dcGain)-m_zeroPointValue) / 
                      (m_reactionValue / (log400-log1000))+log400);
}

float MG811::getConcentration()
{
  static const float log400 = log10f(400);
  static const float log1000 = log10f(1000);

  float val = volts();

  if ((val / dcGain) >= m_zeroPointValue)
    return 0.0;
  else
    return powf(10.0, ((val/dcGain)-m_zeroPointValue) / 
                      (m_reactionValue / (log400-log1000))+log400);
}

bool MG811::thresholdReached()
{
  return (m_gpio.read() ? true : false);
}
