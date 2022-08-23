#include <Arduino.h>
#include "task3.h"
#include "events.h"

static bool compareKeys(BUTTONS *pSecret, BUTTONS *pKey)
{
    bool correct = true;
    for (uint8_t i = 0; i < 5; i++)
    {
        if (pSecret[i] != pKey[i])
        {
            correct = false;
            break;
        }
    }

    return correct;
}

void task3()
{

    enum class TaskStates
    {
        INIT,
        SLOW,
        MEDIUM,
        FAST,
        STAY_ON,
        STAY_OFF,
        WAIT_SLOW,
        WAIT_MEDIUM
    };
    static TaskStates taskState = TaskStates::INIT;
    const uint8_t redLed = 7;
    // const uint32_t BOMBINTERVAL = 1000U;
    // const uint32_t LEDCOUNTERINTERVAL = 500U;

    static uint8_t bombCounter;
    static BUTTONS secret[5] = {BUTTONS::BTN1, BUTTONS::BTN1,
                                BUTTONS::BTN2, BUTTONS::BTN2,
                                BUTTONS::BTN1};

    static BUTTONS fastKey[5] = {BUTTONS::NONE};

    // static uint8_t ledBombCountingState;
    // static uint32_t initBombTimer;
    static uint32_t initLedCounterTimer;
    static uint8_t keyCounter;

    static uint32_t lasTime;
    static bool ledStatus = false;
    static constexpr uint32_t INTERVAL = 1000;

    switch (taskState)
    {
    case TaskStates::INIT:
    {

        pinMode(redLed, OUTPUT);
        // ledBombCountingState = HIGH;
        // digitalWrite(ledBombCounting, ledBombCountingState);
        keyCounter = 0;
        taskState = TaskStates::SLOW;
        break;
    }
    case TaskStates::SLOW:
    {
        uint32_t currentTime = millis();
        if ((currentTime - lasTime) >= INTERVAL)
        {
            lasTime = currentTime;
            digitalWrite(redLed, ledStatus);
            ledStatus = !ledStatus;
        }
        if (buttonEvt.trigger == true)
        {
            buttonEvt.trigger = false;
            if (buttonEvt.whichButton == BUTTONS::BTN1)
            {
                taskState = TaskStates::WAIT_SLOW;
                Serial.print("Entró a wait_slow\n");
            }
            else if (buttonEvt.whichButton == BUTTONS::BTN2)
            {
                taskState = TaskStates::MEDIUM;
                Serial.print("entró a medium\n");
            }
        }

        break;
    }
    case TaskStates::WAIT_SLOW:
    {
        // Serial.print("slow");
    }
    case TaskStates::MEDIUM:
    {
        // Serial.print("TE AMO");
    }
    default:
    {
        break;
    }
    }
}
