/** 
 *  University of Calgary Solar Car Team New Recruit Exercise
 *  Copyright (C) 2015 University of Calgary Solar Car Team
 *
 *  This file is part of the 
 *  University of Calgary Solar Car Team New Recruit Exercise
 *
 *  The University of Calgary Solar Car Team New Recruit Exercise 
 *  is free software: you can redistribute it and/or modify it 
 *  under the terms of the GNU General Public License as 
 *  published by the Free Software Foundation, either version 3 
 *  of the License, or (at your option) any later version.
 *
 *  The University of Calgary Solar Car Team New Recruit Exercise 
 *  is distributed in the hope that it will be useful, but WITHOUT 
 *  ANY WARRANTY; without even the implied warranty of MERCHANTABILITY 
 *  or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General 
 *  Public License for more details.
 *
 *  You should have received a copy of the GNU General 
 *  Public License along with the University of Calgary Solar Car 
 *  Team New Recruit Exercise. If not, see 
 *  <http://www.gnu.org/licenses/>.
 *
 *  For further contact, email <software@calgarysolarcar.ca>
 */

#include "BatteryStateOfChargeService.h"
#include "BatteryData.h"
#include <QTextStream>
#include <QDebug>
#include <QFile>

namespace
{
    const double BATTERY_AMP_HOUR_CAPACITY = 123.0;
}

BatteryStateOfChargeService::BatteryStateOfChargeService(double initialStateOfChargePercent)
: initialStateOfChargePercent_(initialStateOfChargePercent)
{
 AmpHoursUsed_=BATTERY_AMP_HOUR_CAPACITY*(initialStateOfChargePercent_/100);
}
BatteryStateOfChargeService::~BatteryStateOfChargeService()
{
}

double BatteryStateOfChargeService::totalAmpHoursUsed() const
{
    return (AmpHoursUsed_);
}

bool BatteryStateOfChargeService::isCharging() const
{
    if(newCurrent_>=0)
    return false;
    else
    return true;
}

QTime BatteryStateOfChargeService::timeWhenChargedOrDepleted() const
{
  double timeLeft;
  timeLeft=(AmpHoursUsed_/newCurrent_);
  timeLeft=qAbs(timeLeft);
  timeLeft=timeLeft/2.77778e-7;

  QTime base(0,0,0);
  QTime timeOutput= base.addMSecs(timeLeft);
  return timeOutput;
}

void BatteryStateOfChargeService::addData(const BatteryData& batteryData)
{
 //Setting Values

    inCurrent_=newCurrent_;
    initialTime_=currentTime_;

    newCurrent_=batteryData.current;
    currentTime_=batteryData.time;

    if(firstRun_!=true){

    int mschangeTime=abs(initialTime_.msecsTo(currentTime_));
    double changeTime=mschangeTime*2.77778e-7;

    double avgCurrent=(newCurrent_+inCurrent_)/2;
    double pointAverageAH= avgCurrent*changeTime;

    AmpHoursUsed_=AmpHoursUsed_-pointAverageAH;
    }
    firstRun_=false;

    sumCurrent_+=inCurrent_;
}
