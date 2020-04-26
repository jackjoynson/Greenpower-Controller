#include <PID_v1.h>

//#include <PID_v1\PID_v1.h>

const int ThrottlePin = A0;
const int VoltagePin = A1;
const int CurrentPin = A2;
const int PWMPin = 11;
const int CurrentBufferSize = 100;
const int VoltageBufferSize = 50;
//DECLARE CONSTANT INTEGERS

double PWMDuty = 0;
double currentLimit = 20.0;

double voltage;
double voltageBuffer[VoltageBufferSize];
double voltageRunningTotal;
bool voltageBufferFull = false;
int8_t voltageBufferPosition = 0;

double current;
double currentBuffer[CurrentBufferSize];
double currentRunningTotal;
bool currentBufferFull = false;
int8_t currentBufferPosition = 0;

bool batteryDischarged = false;
int8_t underVoltageCount = 0;

int intervalDuration = 100;
unsigned long tickTime = 0;

double dischargeDuration = 0.0;

int8_t diffDuration = 0;

bool go = false;

bool Running = false;
//DECLARE VARIABLES

PID Controller(&current, &PWMDuty, &currentLimit, 0.05, 0.9, 0.04 , DIRECT);
//SETUP OF THE proportional-integral-derivative controller
void setup()
{
	Serial.begin(115200);

	TCCR2B = TCCR2B & 0b11111000 | 0x01;

	for (int i = 0; i < CurrentBufferSize; i++)
	{
		currentBuffer[i] = 0;
	}

	for (int i = 0; i < VoltageBufferSize; i++)
	{
		voltageBuffer[i] = 0;
	}

	SetTick();

	pinMode(ThrottlePin, INPUT);
	pinMode(VoltagePin, INPUT);
	pinMode(CurrentPin, INPUT);
	pinMode(PWMPin, OUTPUT);
}

void loop()
{
	SerialReceive();
	ReadSensors();

	if (Discharged())
	{
		go = false;
		Running = false;
		batteryDischarged = true;
		digitalWrite(PWMPin, LOW);
		PWMDuty = 0;
	}
	//IF BATTERY IS DISCHARGED THEN STOP RUNNING

	if (!batteryDischarged)
	{
		if (digitalRead(ThrottlePin) == HIGH || go)
		{
			if (!Running)
			{
				Controller.SetMode(AUTOMATIC);
				Running = true;
				dischargeDuration = 0.0;
			}
		}
		else if (Running)
		{
			Controller.SetMode(MANUAL);

			digitalWrite(PWMPin, LOW);
			PWMDuty = 0;
			Running = false;
		}

		CurrentControl();
	}
	//IF BATTERIES NOT CHARGED THEN ^
	if (TickElasped())
	{
		SerialTransmit();
	}
}

void CurrentControl()
{
	if (Running)
	{
		//if (current > currentLimit)
		//{
		//	PWMDown();
		//}
		//else if (current < currentLimit)
		//{
		//	PWMUp();
		//}

		Controller.Compute();

		analogWrite(PWMPin, PWMDuty);
	}
}

bool Discharged()
{
	if (voltage < 11.0)
	{
		if (underVoltageCount > 100)
		{
			return true;
		}
		else
		{
			underVoltageCount++;
		}
	}
	else
	{
		underVoltageCount = 0;
	}

	return false;
}

void PWMUp()
{
	if (PWMDuty < 255)
	{
		PWMDuty++;
	}
	else
	{
		PWMDuty = 255;
	}
}

void PWMDown()
{
	if (PWMDuty > 0)
	{
		PWMDuty--;
	}
	else
	{
		PWMDuty = 0;
	}
}

void ReadSensors()
{
	double tempCurrent = analogRead(CurrentPin);
	CurrentAveraging((tempCurrent <= 11.0) ? 0.0 : ((tempCurrent - 11.0) / 6.74) );

	//current = analogRead(CurrentPin);
	VoltageAveraging(analogRead(VoltagePin) / 35.15);
}

void SetTick()
{
	tickTime = millis() + intervalDuration;
}

bool TickElasped()
{
	if (tickTime < millis())
	{
		SetTick();
		return true;
	}
	else
	{
		return false;
	}
}

void CurrentAveraging(double input)
{
	double removedValue = currentBuffer[currentBufferPosition];
	currentBuffer[currentBufferPosition] = input;
	currentRunningTotal += input;

	if (currentBufferFull)
	{
		currentRunningTotal -= removedValue;
		
		if (currentBufferPosition == (CurrentBufferSize - 1))
		{

			currentBufferPosition = 0;
		}
		else
		{
			currentBufferPosition++;
		}

		current = currentRunningTotal / CurrentBufferSize;

		return;
	}
	else if (currentBufferPosition == (CurrentBufferSize - 1))
	{
		currentBufferFull = true;
		currentBufferPosition = 0;

		return;
	}
	else
	{
		currentBufferPosition++;
	}
}

void VoltageAveraging(double input)
{
	double diff = voltage - input;
	if ((diff > 1.0 || diff < -1.0) && voltage != 0.0 && diffDuration < 100)
	{
		diffDuration++;
		return;
	}

	diffDuration = 0;

	double removedValue = voltageBuffer[voltageBufferPosition];
	voltageBuffer[voltageBufferPosition] = input;
	voltageRunningTotal += input;

	if (voltageBufferFull)
	{
		voltageRunningTotal -= removedValue;

		if (voltageBufferPosition == (VoltageBufferSize - 1))
		{

			voltageBufferPosition = 0;
		}
		else
		{
			voltageBufferPosition++;
		}

		voltage = voltageRunningTotal / VoltageBufferSize;

		return;
	}
	else if (voltageBufferPosition == (VoltageBufferSize - 1))
	{
		voltageBufferFull = true;
		voltageBufferPosition = 0;

		return;
	}
	else
	{
		voltageBufferPosition++;
	}
}

void SerialReceive()
{
	if (Serial.available() > 0)
	{
		char firstVal = Serial.read();
		if (firstVal == 'p' || firstVal == 'P')
		{
			go = true;
			batteryDischarged = false;
		}
		else if (firstVal == 'o' || firstVal == 'O')
		{
			go = false;
		}
		else
		{
			currentLimit = Serial.parseInt();
			//PWMDuty = Serial.parseInt();
		}
	}
}

void SerialTransmit()
{
	if (Running)
	{
		dischargeDuration += 0.1;
	}

	Serial.print(current);
	Serial.print("A, ");
	Serial.print(voltage);
	Serial.print("V, ");
	Serial.print(currentLimit);
	Serial.print("A, ");
	Serial.print(PWMDuty);
	Serial.print("%, ");
	Serial.print(dischargeDuration);
	Serial.println("s");
}
