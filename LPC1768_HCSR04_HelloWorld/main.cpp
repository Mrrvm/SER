#include "mbed.h"
#include "ultrasonic.h"

//DigitalOut led1(LED1);

void dist(int distance)
{
    printf("Distance %d mm\r\n", distance); // distance is in mm not cm
}

ultrasonic mu(p6, p7, .5, 1, &dist);    //Set the trigger pin to D8 (p6) and the echo pin to D9 (p7)
                                        //have updates every .5 seconds and a timeout after 1
                                        //second, and call dist when the distance changes

int main()
{
    
    mu.startUpdates();//start measuring the distance
    while(1)
    {
        mu.checkDistance();     //call checkDistance() as much as possible, as this is where
                                //the class checks if dist needs to be called.
        // wait(0.5);
    }
}
