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

    static uint8_t bombCounter;
    static BUTTONS secret[5] = {BUTTONS::BTN1, BUTTONS::BTN1,
                                BUTTONS::BTN2, BUTTONS::BTN2,
                                BUTTONS::BTN1};

    static BUTTONS fastKey[5] = {BUTTONS::NONE};

    static uint32_t initLedCounterTimer;
    static uint8_t keyCounter;
    static uint8_t lastState;

    static uint32_t lasTime;
    static bool ledStatus = false;
    static constexpr uint32_t intervalSlow = 1000;
    static constexpr uint32_t intervalMedium = 500;
    static constexpr uint32_t intervalFast = 250;

    switch (taskState)
    {
    case TaskStates::INIT:
    {
        pinMode(redLed, OUTPUT);
        keyCounter = 0;
        lastState = 0;
        digitalWrite(redLed, HIGH);
        ledStatus = true;
        taskState = TaskStates::SLOW;
        break;
    }
    case TaskStates::SLOW:
    {
        uint32_t currentTime = millis();

        if ((currentTime - lasTime) >= intervalSlow)
        {
            lasTime = currentTime;
            // digitalWrite(redLed,HIGH);
            digitalWrite(redLed, ledStatus);
            ledStatus = !ledStatus;
        }
        if (buttonEvt.trigger == true)
        {
            buttonEvt.trigger = false;
            if (buttonEvt.whichButton == BUTTONS::BTN1)
            {
                taskState = TaskStates::WAIT_SLOW;
            }
            else if (buttonEvt.whichButton == BUTTONS::BTN2)
            {
                digitalWrite(redLed, HIGH);
                ledStatus = true;
                taskState = TaskStates::MEDIUM;
            }
        }

        break;
    }
    case TaskStates::WAIT_SLOW:
    {
        uint32_t currentTime = millis();

        if ((currentTime - lasTime) >= intervalSlow)
        {
            // digitalWrite(redLed,LOW);
            digitalWrite(redLed, LOW);
            ledStatus = false;
            lastState = 1;
            taskState = TaskStates::STAY_OFF;
        }

        break;
    }
    case TaskStates::MEDIUM:
    {
        uint32_t currentTime = millis();

        if ((currentTime - lasTime) >= intervalMedium)
        {
            lasTime = currentTime;
            // digitalWrite(redLed,HIGH);
            digitalWrite(redLed, ledStatus);
            ledStatus = !ledStatus;
        }

        if (buttonEvt.trigger == true)
        {
            buttonEvt.trigger = false;
            if (buttonEvt.whichButton == BUTTONS::BTN1)
            {
                taskState = TaskStates::WAIT_MEDIUM;
            }
            else if (buttonEvt.whichButton == BUTTONS::BTN2)
            {
                digitalWrite(redLed, HIGH);
                ledStatus = true;
                taskState = TaskStates::SLOW;
            }
        }
        break;
    }
    case TaskStates::STAY_OFF:
    {

        if (buttonEvt.trigger == true)
        {
            buttonEvt.trigger = false;
            if (buttonEvt.whichButton == BUTTONS::BTN1)
            {
                digitalWrite(redLed, HIGH);
                ledStatus = true;
                taskState = TaskStates::SLOW;
            }
            else if (buttonEvt.whichButton == BUTTONS::BTN2)
            {
                digitalWrite(redLed, HIGH);
                ledStatus = true;
                taskState = TaskStates::FAST;
            }
        }
        break;
    }
    case TaskStates::FAST:
    {
        uint32_t currentTime = millis();

        if ((currentTime - lasTime) >= intervalFast)
        {
            lasTime = currentTime;
            // digitalWrite(redLed,HIGH);
            digitalWrite(redLed, ledStatus);
            ledStatus = !ledStatus;
        }

        if (buttonEvt.trigger == true)
        {
            buttonEvt.trigger = false;
            fastKey[keyCounter] = buttonEvt.whichButton;
            keyCounter++;
            if (keyCounter == 5)
            {
                keyCounter = 0;
                if (compareKeys(secret, fastKey) == true && lastState == 1)
                {
                    Serial.print("last state 1\n");
                    // digitalWrite(redLed,LOW);
                    taskState = TaskStates::STAY_OFF;
                }
                else if (compareKeys(secret, fastKey) == true && lastState == 2)
                {
                    Serial.print("last state 2\n");
                    // digitalWrite(redLed,HIGH);
                    taskState = TaskStates::STAY_ON;
                }
            }
        }
        break;
    }
    case TaskStates::WAIT_MEDIUM:
    {
        uint32_t currentTime = millis();
        if ((currentTime - lasTime) >= intervalMedium)
        {
            taskState = TaskStates::STAY_ON;
        }
        break;
    }
    case TaskStates::STAY_ON:
    {
        digitalWrite(redLed, HIGH);
        ledStatus = true;
        lastState = 2;

        if (buttonEvt.trigger == true)
        {
            buttonEvt.trigger = false;
            if (buttonEvt.whichButton == BUTTONS::BTN1)
            {
                taskState = TaskStates::MEDIUM;
            }
            else if (buttonEvt.whichButton == BUTTONS::BTN2)
            {
                taskState = TaskStates::FAST;
            }
        }
        break;
    }
    default:
    {
        break;
    }
    }
}