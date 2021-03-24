#include "mbed.h"
#include "mbed_events.h"
using namespace std::chrono;

DigitalOut led1(LED1);
DigitalOut led2(LED3);

EventQueue printfQueue;
EventQueue eventQueue;

void print_toggle_led() {
// this runs in the lower priority thread

static int cnt = 0;
printf("Toggle LED %d!\r\n", cnt++);
}

void blink_led2() {
// this runs in the normal priority thread
led2 = !led2;
printfQueue.call(&print_toggle_led);
}



int main() {

// low priority thread for calling printf()
Thread printfThread(osPriorityLow);
printfThread.start(callback(&printfQueue, &EventQueue::dispatch_forever));

// normal priority thread for other events
Thread eventThread(osPriorityNormal);
eventThread.start(callback(&eventQueue, &EventQueue::dispatch_forever));

// call blink_led2 every second, automatically defering to the eventThread
Ticker ledTicker;
ledTicker.attach(eventQueue.event(&blink_led2), 500ms);

// button fall still runs in the ISR

while (1) {ThisThread::sleep_for(1s);}
}
