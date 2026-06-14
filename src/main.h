#ifndef MAIN_H
#define MAIN_H

void bootIntro();
void printDistance(float distance); // prints distance in display
float getSonar();   // gets measured distance
void printDistanceOnSerial(float distance);     // prints distance on Serial
void ISR();
void measureDistance();

#endif
